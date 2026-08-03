#include "Stdafx.hpp"
#include "UI/Window.hpp"
#include "UI/Theme.hpp"
#include "UI/Widgets/Button.hpp"
#include "UI/Fonts/IconsLucide.h"
#include "UI/Pages/MousePage.hpp"
#include "UI/Pages/KeyboardPage.hpp"
#include "UI/Pages/SystemPage.hpp"
#include "UI/Pages/NetworkPage.hpp"
#include "UI/Pages/GpuPage.hpp"
#include "UI/Pages/AudioPage.hpp"
#include "UI/Pages/PrivacyPage.hpp"
#include "UI/Pages/DebloatPage.hpp"
#include "UI/Pages/LogsPage.hpp"
#include "UI/Pages/InfoPage.hpp"
#include "Modules/MouseModule.hpp"
#include "Modules/KeyboardModule.hpp"
#include "Modules/SystemModule.hpp"
#include "Modules/NetworkModule.hpp"
#include "Modules/GpuModule.hpp"
#include "Modules/AudioModule.hpp"
#include "Modules/PrivacyModule.hpp"
#include "Modules/DebloatModule.hpp"

namespace UI
{
	Window::Window(
		const wchar_t* title,
		const ImVec2& size,
		std::shared_ptr<App::Settings> settings,
		std::shared_ptr<App::Logger> logger,
		std::shared_ptr<App::Intl> intl
	)
		: title_( title ),
		  settings_( std::move( settings ) ),
		  logger_( std::move( logger ) ),
		  intl_( std::move( intl ) )
	{
		register_services( );
		register_tabs( );
		tab_manager_.set_font_manager( font_manager_ );
		tab_manager_.set_intl( intl_ );

		const int title_len = WideCharToMultiByte( CP_UTF8, 0, title, -1, nullptr, 0, nullptr, nullptr );
		if ( title_len > 0 )
		{
			title_utf8_.resize( title_len - 1 );
			WideCharToMultiByte( CP_UTF8, 0, title, -1, title_utf8_.data( ), title_len, nullptr, nullptr );
		}

		Theme::set_mode( settings_->theme( ) == App::ThemeMode::Light ? Theme::Mode::Light : Theme::Mode::Dark );

		bg_brush_ = CreateSolidBrush( Theme::background_gdi( ) );

		wc_ =
		{
			.cbSize = sizeof( wc_ ),
			.style = CS_HREDRAW | CS_VREDRAW,
			.lpfnWndProc = wnd_proc,
			.hInstance = GetModuleHandleW( nullptr ),
			.hCursor = LoadCursor( nullptr, IDC_ARROW ),
			.hbrBackground = bg_brush_,
			.lpszClassName = title_.c_str( ),
		};

		if ( !RegisterClassExW( &wc_ ) )
			throw std::runtime_error( "Failed to register window class." );

		ImGui_ImplWin32_EnableDpiAwareness( );

		const float dpi_scale = static_cast<float>( GetDpiForSystem( ) ) / 96.f;
		const int width       = static_cast<int>( size.x * dpi_scale + 0.5f );
		const int height      = static_cast<int>( size.y * dpi_scale + 0.5f );

		const int pos_x = ( GetSystemMetrics( SM_CXSCREEN ) - width ) / 2;
		const int pos_y = ( GetSystemMetrics( SM_CYSCREEN ) - height ) / 2;

		hwnd_ = CreateWindowExW(
			WS_EX_APPWINDOW,
			wc_.lpszClassName,
			title_.c_str( ),
			WS_POPUP | WS_CLIPCHILDREN,
			pos_x,
			pos_y,
			width,
			height,
			nullptr,
			nullptr,
			wc_.hInstance,
			this
		);

		if ( !hwnd_ )
			throw std::runtime_error( "Failed to create window." );

		Theme::apply_chrome( hwnd_ );

		d3d_device_ = std::make_unique<D3D11Device>( hwnd_ );

		initialize_imgui( );

		ShowWindow( hwnd_, SW_SHOW );
		UpdateWindow( hwnd_ );

		if ( logger_ )
			logger_->info( "App", "window ready" );
	}

	Window::~Window( )
	{
		close( );

		shutdown_imgui( );

		if ( hwnd_ )
		{
			DestroyWindow( hwnd_ );
			hwnd_ = nullptr;
		}

		if ( bg_brush_ )
		{
			DeleteObject( bg_brush_ );
			bg_brush_ = nullptr;
		}

		if ( wc_.lpszClassName )
			UnregisterClassW( wc_.lpszClassName, wc_.hInstance );
	}

	void Window::register_services( )
	{
		hub_.register_services(
			settings_,
			logger_,
			intl_,
			std::make_shared<Modules::MouseModule>( ),
			std::make_shared<Modules::KeyboardModule>( ),
			std::make_shared<Modules::SystemModule>( ),
			std::make_shared<Modules::NetworkModule>( ),
			std::make_shared<Modules::GpuModule>( ),
			std::make_shared<Modules::AudioModule>( ),
			std::make_shared<Modules::PrivacyModule>( ),
			std::make_shared<Modules::DebloatModule>( )
		);
	}

	void Window::register_tabs( )
	{
		const auto logger = hub_.get<App::Logger>( );
		const auto intl   = hub_.get<App::Intl>( );

		tab_manager_.add_tab( "Mouse", ICON_LC_MOUSE, std::make_unique<Pages::MousePage>( hub_.get<Modules::MouseModule>( ), logger, intl ) );
		tab_manager_.add_tab( "Keyboard", ICON_LC_KEYBOARD,
		                      std::make_unique<Pages::KeyboardPage>( hub_.get<Modules::KeyboardModule>( ), logger, intl ) );
		tab_manager_.add_tab( "System", ICON_LC_SETTINGS, std::make_unique<Pages::SystemPage>( hub_.get<Modules::SystemModule>( ), logger, intl ) );
		tab_manager_.add_tab( "Network", ICON_LC_NETWORK, std::make_unique<Pages::NetworkPage>( hub_.get<Modules::NetworkModule>( ), logger, intl ) );
		tab_manager_.add_tab( "GPU", ICON_LC_CPU, std::make_unique<Pages::GpuPage>( hub_.get<Modules::GpuModule>( ), logger, intl ) );
		tab_manager_.add_tab( "Audio", ICON_LC_VOLUME_2, std::make_unique<Pages::AudioPage>( hub_.get<Modules::AudioModule>( ), logger, intl ) );
		tab_manager_.add_tab( "Privacy", ICON_LC_SHIELD, std::make_unique<Pages::PrivacyPage>( hub_.get<Modules::PrivacyModule>( ), logger, intl ) );
		tab_manager_.add_tab( "Debloat", ICON_LC_SPARKLES,
		                      std::make_unique<Pages::DebloatPage>( hub_.get<Modules::DebloatModule>( ), logger, intl ) );
		tab_manager_.add_tab( "Logs", ICON_LC_CLIPBOARD_LIST, std::make_unique<Pages::LogsPage>( logger, intl ) );
		tab_manager_.add_tab( "Info", ICON_LC_INFO, std::make_unique<Pages::InfoPage>( hub_.get<App::Settings>( ), intl, logger ) );
	}

	void Window::initialize_imgui( )
	{
		ImGui::CreateContext( );

		ImGuiIO& io    = ImGui::GetIO( );
		io.IniFilename = nullptr;
		io.LogFilename = nullptr;

		ImGui_ImplWin32_Init( hwnd_ );

		Theme::set_dpi_scale( ImGui_ImplWin32_GetDpiScaleForHwnd( hwnd_ ) );
		Theme::apply( );

		ImGui_ImplDX11_Init( d3d_device_->get_device( ), d3d_device_->get_device_context( ) );

		font_manager_.build_font_atlas( );

		io.FontDefault = font_manager_.get( Managers::FontType::Regular, Theme::px( 16.f ) );
	}

	void Window::shutdown_imgui( )
	{
		ImGui_ImplDX11_Shutdown( );
		ImGui_ImplWin32_Shutdown( );
		ImGui::DestroyContext( );
	}

	void Window::sync_theme( )
	{
		const auto desired = settings_->theme( ) == App::ThemeMode::Light ? Theme::Mode::Light : Theme::Mode::Dark;

		if ( Theme::mode( ) == desired )
			return;

		Theme::set_mode( desired );
		Theme::apply( );
		Theme::apply_chrome( hwnd_ );

		if ( bg_brush_ )
		{
			DeleteObject( bg_brush_ );
			bg_brush_ = CreateSolidBrush( Theme::background_gdi( ) );
		}
	}

	void Window::render_title_bar( )
	{
		const float BTN = Theme::px( 32.f );
		const float GAP = Theme::px( 6.f );
		const float PAD = Theme::px( 14.f );

		ImGui::PushStyleColor( ImGuiCol_ChildBg, Theme::Background );
		ImGui::PushStyleVar( ImGuiStyleVar_ChildRounding, 0.f );
		ImGui::PushStyleVar( ImGuiStyleVar_ChildBorderSize, 0.f );

		ImGui::BeginChild( "##title_bar", ImVec2( 0.f, Theme::HeaderH ), ImGuiChildFlags_None,
		                   ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse );

		const ImVec2 p0 = ImGui::GetWindowPos( );
		const ImVec2 p1 = { p0.x + ImGui::GetWindowWidth( ), p0.y + Theme::HeaderH };

		ImDrawList* dl = ImGui::GetWindowDrawList( );

		const float ornament_w = ( std::min )( Theme::px( 260.f ), ImGui::GetWindowWidth( ) * 0.48f );

		Theme::draw_header_ornament( dl, { p1.x - ornament_w, p0.y }, p1, Theme::Primary );

		dl->AddLine(
			{ p0.x, p1.y - 0.5f },
			{ p1.x, p1.y - 0.5f },
			Theme::separator_col( ),
			Theme::SeparatorThickness
		);

		const float icon_size = Theme::px( 16.f );

		ImGui::SetCursorPos( { PAD, ( Theme::HeaderH - icon_size ) * 0.5f } );

		const ImVec2 icon_pos = ImGui::GetCursorScreenPos( );

		dl->AddText( ImGui::GetFont( ), icon_size, icon_pos, Theme::col( Theme::Primary ), ICON_LC_GAUGE );

		ImFont* title_font = font_manager_.get( Managers::FontType::Medium, 16.f );

		ImGui::PushFont( title_font );

		ImGui::SetCursorPos( { PAD + icon_size + Theme::px( 8.f ), ( Theme::HeaderH - ImGui::GetTextLineHeight( ) ) * 0.5f } );

		ImGui::TextUnformatted( title_utf8_.c_str( ) );

		ImGui::PopFont( );

		ImGui::SameLine( ImGui::GetWindowWidth( ) - BTN * 2.f - GAP - PAD );
		ImGui::SetCursorPosY( ( Theme::HeaderH - BTN ) * 0.5f );

		ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, { GAP, 0.f } );

		if ( Widgets::IconButton( ICON_LC_MINUS "##minimize" ) )
			ShowWindow( hwnd_, SW_MINIMIZE );

		ImGui::SameLine( );
		ImGui::SetCursorPosY( ( Theme::HeaderH - BTN ) * 0.5f );

		if ( Widgets::IconButton( ICON_LC_X "##close" ) )
			close( );

		ImGui::PopStyleVar( );

		if ( ImGui::IsWindowHovered( ImGuiHoveredFlags_ChildWindows ) &&
		     !ImGui::IsAnyItemHovered( ) &&
		     ImGui::IsMouseClicked( ImGuiMouseButton_Left ) )
		{
			ReleaseCapture( );
			SendMessageW( hwnd_, WM_NCLBUTTONDOWN, HTCAPTION, 0 );
		}

		ImGui::EndChild( );
		ImGui::PopStyleVar( 2 );
		ImGui::PopStyleColor( );
	}

	void Window::run( )
	{
		MSG msg = {};

		while ( running_.load( std::memory_order_acquire ) )
		{
			while ( PeekMessageW( &msg, nullptr, 0U, 0U, PM_REMOVE ) )
			{
				TranslateMessage( &msg );
				DispatchMessageW( &msg );

				if ( msg.message == WM_QUIT )
					running_.store( false, std::memory_order_release );
			}

			if ( !running_.load( std::memory_order_acquire ) )
				break;

			if ( !IsWindowVisible( hwnd_ ) )
			{
				WaitMessage( );
				continue;
			}

			sync_theme( );

			font_manager_.rebuild_font_atlas_if_needed( );

			ImGui_ImplDX11_NewFrame( );
			ImGui_ImplWin32_NewFrame( );
			ImGui::NewFrame( );

			ImGui::SetNextWindowPos( { 0.f, 0.f }, ImGuiCond_Always );
			ImGui::SetNextWindowSize( ImGui::GetIO( ).DisplaySize, ImGuiCond_Always );

			constexpr auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
			                       ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
			                       ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar |
			                       ImGuiWindowFlags_NoScrollWithMouse;

			ImGui::PushStyleVar( ImGuiStyleVar_WindowRounding, Theme::Radius );
			ImGui::PushStyleVar( ImGuiStyleVar_WindowBorderSize, 0.f );

			ImGui::Begin( "##root", nullptr, flags );

			render_title_bar( );

			ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2( 0.f, 0.f ) );

			tab_manager_.render( );

			ImGui::PopStyleVar( );

			const ImVec2 p0 = ImGui::GetWindowPos( );
			const ImVec2 p1 = { p0.x + ImGui::GetWindowWidth( ), p0.y + ImGui::GetWindowHeight( ) };

			ImGui::GetWindowDrawList( )->AddRect(
				{ p0.x + 0.5f, p0.y + 0.5f },
				{ p1.x - 0.5f, p1.y - 0.5f },
				Theme::col( Theme::Border ),
				Theme::Radius,
				0,
				1.f
			);

			ImGui::End( );
			ImGui::PopStyleVar( 2 );

			ImGui::Render( );

			auto* rtv = d3d_device_->get_render_target_view( );
			d3d_device_->get_device_context( )->OMSetRenderTargets( 1, &rtv, nullptr );

			const float clear[ 4 ] = { Theme::Background.x, Theme::Background.y, Theme::Background.z, 1.f };
			d3d_device_->get_device_context( )->ClearRenderTargetView( rtv, clear );

			ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData( ) );
			d3d_device_->get_swap_chain( )->Present( 1, 0 );
		}
	}

	LRESULT CALLBACK Window::wnd_proc( const HWND hwnd,
	                                   const UINT msg,
	                                   const WPARAM wparam,
	                                   const LPARAM lparam )
	{
		if ( msg == WM_NCCREATE )
		{
			const auto* create = std::bit_cast<CREATESTRUCTW*>( lparam );
			SetWindowLongPtrW( hwnd,
			                   GWLP_USERDATA,
			                   std::bit_cast<LONG_PTR>( create->lpCreateParams ) );
			return DefWindowProcW( hwnd, msg, wparam, lparam );
		}

		if ( ImGui_ImplWin32_WndProcHandler( hwnd, msg, wparam, lparam ) )
			return true;

		auto* self = std::bit_cast<Window*>( GetWindowLongPtrW( hwnd, GWLP_USERDATA ) );

		switch ( msg )
		{
			case WM_ERASEBKGND:
				return 1;

			case WM_NCCALCSIZE:
				if ( wparam == TRUE )
					return 0;
				return DefWindowProcW( hwnd, msg, wparam, lparam );

			case WM_SIZE:
				if ( self && self->d3d_device_ && wparam != SIZE_MINIMIZED )
					self->d3d_device_->resize( LOWORD( lparam ), HIWORD( lparam ) );

				Theme::apply_chrome( hwnd );
				return 0;

			case WM_DESTROY:
				if ( self )
					self->close( );

				PostQuitMessage( 0 );
				return 0;

			default:
				return DefWindowProcW( hwnd, msg, wparam, lparam );
		}
	}
}

#include "Stdafx.hpp"
#include "UI/Window.hpp"
#include "UI/Pages/MousePage.hpp"
#include "UI/Pages/KeyboardPage.hpp"
#include "UI/Pages/SystemPage.hpp"
#include "UI/Pages/NetworkPage.hpp"
#include "UI/Pages/GpuPage.hpp"
#include "UI/Pages/AudioPage.hpp"
#include "Modules/MouseModule.hpp"
#include "Modules/KeyboardModule.hpp"
#include "Modules/SystemModule.hpp"
#include "Modules/NetworkModule.hpp"
#include "Modules/GpuModule.hpp"
#include "Modules/AudioModule.hpp"

namespace UI
{
	Window::Window( const wchar_t* title, const ImVec2& size )
		: title_( title )
	{
		register_services( );
		register_tabs( );

		wc_ =
		{
			.cbSize = sizeof( wc_ ),
			.style = CS_HREDRAW | CS_VREDRAW,
			.lpfnWndProc = wnd_proc,
			.hInstance = GetModuleHandleW( nullptr ),
			.hCursor = LoadCursor( nullptr, IDC_ARROW ),
			.hbrBackground = GetSysColorBrush( COLOR_WINDOW ),
			.lpszClassName = title_.c_str( ),
		};

		if ( !RegisterClassExW( &wc_ ) )
			throw std::runtime_error( "Failed to register window class." );

		ImGui_ImplWin32_EnableDpiAwareness( );

		const int width  = static_cast<int>( size.x );
		const int height = static_cast<int>( size.y );

		const int pos_x = ( GetSystemMetrics( SM_CXSCREEN ) - width ) / 2;
		const int pos_y = ( GetSystemMetrics( SM_CYSCREEN ) - height ) / 2;

		hwnd_ = CreateWindowExW(
			0,
			wc_.lpszClassName,
			title_.c_str( ),
			WS_OVERLAPPEDWINDOW,
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

		d3d_device_ = std::make_unique<D3D11Device>( hwnd_ );

		initialize_imgui( );

		ShowWindow( hwnd_, SW_SHOW );
		UpdateWindow( hwnd_ );
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

		if ( wc_.lpszClassName )
			UnregisterClassW( wc_.lpszClassName, wc_.hInstance );
	}

	void Window::register_services( )
	{
		hub_.register_services(
			std::make_shared<Modules::MouseModule>( ),
			std::make_shared<Modules::KeyboardModule>( ),
			std::make_shared<Modules::SystemModule>( ),
			std::make_shared<Modules::NetworkModule>( ),
			std::make_shared<Modules::GpuModule>( ),
			std::make_shared<Modules::AudioModule>( )
		);
	}

	void Window::register_tabs( )
	{
		tab_manager_.add_tab( "Mouse", std::make_unique<Pages::MousePage>( hub_.get<Modules::MouseModule>( ) ) );
		tab_manager_.add_tab( "Keyboard", std::make_unique<Pages::KeyboardPage>( hub_.get<Modules::KeyboardModule>( ) ) );
		tab_manager_.add_tab( "System", std::make_unique<Pages::SystemPage>( hub_.get<Modules::SystemModule>( ) ) );
		tab_manager_.add_tab( "Network", std::make_unique<Pages::NetworkPage>( hub_.get<Modules::NetworkModule>( ) ) );
		tab_manager_.add_tab( "GPU", std::make_unique<Pages::GpuPage>( hub_.get<Modules::GpuModule>( ) ) );
		tab_manager_.add_tab( "Audio", std::make_unique<Pages::AudioPage>( hub_.get<Modules::AudioModule>( ) ) );
	}

	void Window::initialize_imgui( ) const
	{
		ImGui::CreateContext( );

		ImGuiIO& io    = ImGui::GetIO( );
		io.IniFilename = nullptr;
		io.LogFilename = nullptr;

		ImGui::StyleColorsDark( );

		ImGui_ImplWin32_Init( hwnd_ );
		ImGui_ImplDX11_Init( d3d_device_->get_device( ), d3d_device_->get_device_context( ) );
	}

	void Window::shutdown_imgui( )
	{
		ImGui_ImplDX11_Shutdown( );
		ImGui_ImplWin32_Shutdown( );
		ImGui::DestroyContext( );
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

			ImGui_ImplDX11_NewFrame( );
			ImGui_ImplWin32_NewFrame( );
			ImGui::NewFrame( );

			ImGui::SetNextWindowPos( { 0.f, 0.f }, ImGuiCond_Always );
			ImGui::SetNextWindowSize( ImGui::GetIO( ).DisplaySize, ImGuiCond_Always );

			constexpr auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
			                       ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
			                       ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar |
			                       ImGuiWindowFlags_NoScrollWithMouse;

			ImGui::Begin( "##root", nullptr, flags );

			tab_manager_.render( );

			ImGui::End( );

			ImGui::Render( );

			auto* rtv = d3d_device_->get_render_target_view( );
			d3d_device_->get_device_context( )->OMSetRenderTargets( 1, &rtv, nullptr );

			constexpr float clear[ 4 ] = { 0.1f, 0.1f, 0.1f, 1.f };
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
			case WM_SIZE:
				if ( self && self->d3d_device_ && wparam != SIZE_MINIMIZED )
					self->d3d_device_->resize( LOWORD( lparam ), HIWORD( lparam ) );

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

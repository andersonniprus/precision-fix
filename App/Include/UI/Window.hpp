#pragma once

#include "UI/D3D11Device.hpp"
#include "UI/Managers/TabManager.hpp"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND, UINT, WPARAM, LPARAM );

namespace UI
{
	class Window
	{
	public:
		explicit Window( const wchar_t*, const ImVec2& );
		~Window( );

		Window( const Window& )            = delete;
		Window& operator=( const Window& ) = delete;

		void run( );

		void close( ) noexcept
		{
			running_.store( false, std::memory_order_release );
		}

		[[nodiscard]] HWND hwnd( ) const noexcept
		{
			return hwnd_;
		}

	private:
		void register_services( );
		void register_tabs( );

		void initialize_imgui( ) const;
		static void shutdown_imgui( );

		static LRESULT CALLBACK wnd_proc( HWND, UINT, WPARAM, LPARAM );

		HWND hwnd_ {};
		WNDCLASSEXW wc_ {};
		std::wstring title_;

		std::unique_ptr<D3D11Device> d3d_device_;

		ServiceHub hub_;
		Managers::TabManager tab_manager_;

		std::atomic<bool> running_ { true };
	};
}

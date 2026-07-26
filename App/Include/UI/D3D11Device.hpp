#pragma once

namespace UI
{
	class D3D11Device
	{
	public:
		explicit D3D11Device( HWND );
		~D3D11Device( );

		D3D11Device( const D3D11Device& )            = delete;
		D3D11Device& operator=( const D3D11Device& ) = delete;

		void create_render_target( );
		void cleanup_render_target( );
		void resize( UINT, UINT );

		[[nodiscard]] ID3D11Device* get_device( ) const
		{
			return device_.Get( );
		}

		[[nodiscard]] ID3D11DeviceContext* get_device_context( ) const
		{
			return device_context_.Get( );
		}

		[[nodiscard]] IDXGISwapChain* get_swap_chain( ) const
		{
			return swap_chain_.Get( );
		}

		[[nodiscard]] ID3D11RenderTargetView* get_render_target_view( ) const
		{
			return render_target_view_.Get( );
		}

	private:
		Microsoft::WRL::ComPtr<ID3D11Device> device_;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> device_context_;
		Microsoft::WRL::ComPtr<IDXGISwapChain> swap_chain_;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> render_target_view_;
	};
}

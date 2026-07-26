#include "Stdafx.hpp"
#include "UI/D3D11Device.hpp"

namespace
{
	void throw_if_failed( const HRESULT hr, const char* message )
	{
		if ( FAILED( hr ) )
			throw std::runtime_error(
				std::format( "{} (hr=0x{:08X})", message, static_cast<std::uint32_t>( hr ) ) );
	}
}

namespace UI
{
	D3D11Device::D3D11Device( const HWND hwnd )
	{
		DXGI_SWAP_CHAIN_DESC sd   = {};
		sd.BufferCount            = 2;
		sd.BufferDesc.Format      = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate = { .Numerator = 0, .Denominator = 1 };
		sd.BufferUsage            = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow           = hwnd;
		sd.SampleDesc             = { .Count = 1, .Quality = 0 };
		sd.Windowed               = TRUE;
		sd.SwapEffect             = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		sd.Flags                  = 0;

		constexpr D3D_FEATURE_LEVEL feature_levels[ ] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_0
		};

		D3D_FEATURE_LEVEL selected_level;

		const auto create_device = [ & ]( const D3D_DRIVER_TYPE driver_type )
		{
			return D3D11CreateDeviceAndSwapChain(
				nullptr,
				driver_type,
				nullptr,
				0,
				feature_levels,
				std::size( feature_levels ),
				D3D11_SDK_VERSION,
				&sd,
				&swap_chain_,
				&device_,
				&selected_level,
				&device_context_
			);
		};

		HRESULT hr = create_device( D3D_DRIVER_TYPE_HARDWARE );

		if ( FAILED( hr ) )
		{
			sd.SwapEffect             = DXGI_SWAP_EFFECT_DISCARD;
			sd.Flags                  = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
			sd.BufferDesc.RefreshRate = { .Numerator = 60, .Denominator = 1 };

			hr = create_device( D3D_DRIVER_TYPE_HARDWARE );
		}

		if ( FAILED( hr ) )
			hr = create_device( D3D_DRIVER_TYPE_WARP );

		throw_if_failed( hr, "Failed to create D3D11 device and swap chain" );

		if ( Microsoft::WRL::ComPtr<IDXGISwapChain3> sc3; SUCCEEDED( swap_chain_.As( &sc3 ) ) )
			sc3->SetColorSpace1( DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709 );

		create_render_target( );
	}

	D3D11Device::~D3D11Device( )
	{
		cleanup_render_target( );
	}

	void D3D11Device::create_render_target( )
	{
		Microsoft::WRL::ComPtr<ID3D11Texture2D> back_buffer;
		HRESULT hr = swap_chain_->GetBuffer( 0, IID_PPV_ARGS( &back_buffer ) );

		throw_if_failed( hr, "Failed to get back buffer" );

		hr = device_->CreateRenderTargetView( back_buffer.Get( ), nullptr, &render_target_view_ );

		throw_if_failed( hr, "Failed to create render target view" );
	}

	void D3D11Device::cleanup_render_target( )
	{
		render_target_view_.Reset( );
	}

	void D3D11Device::resize( const UINT width, const UINT height )
	{
		if ( width == 0 || height == 0 )
			return;

		cleanup_render_target( );

		const HRESULT hr = swap_chain_->ResizeBuffers( 0, width, height, DXGI_FORMAT_UNKNOWN, 0 );
		throw_if_failed( hr, "Failed to resize swap chain buffers" );

		create_render_target( );
	}
}

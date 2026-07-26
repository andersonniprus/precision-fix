#include "Stdafx.hpp"
#include "Modules/AudioModule.hpp"

#include <initguid.h>
#include <mmdeviceapi.h>
#include <propsys.h>
#include <functiondiscoverykeys_devpkey.h>

namespace
{
	using Microsoft::WRL::ComPtr;

	class ComGuard
	{
	public:
		ComGuard( ) noexcept
			: result_( CoInitializeEx( nullptr, COINIT_APARTMENTTHREADED ) )
		{
		}

		~ComGuard( )
		{
			if ( SUCCEEDED( result_ ) )
				CoUninitialize( );
		}

		[[nodiscard]] bool ok( ) const noexcept
		{
			return SUCCEEDED( result_ ) || result_ == RPC_E_CHANGED_MODE;
		}

	private:
		HRESULT result_;
	};

	[[nodiscard]] Core::Error map_hresult( const HRESULT hr ) noexcept
	{
		if ( hr == E_ACCESSDENIED )
			return Core::Error::AccessDenied;

		if ( hr == E_NOTFOUND )
			return Core::Error::NotFound;

		return Core::Error::Unknown;
	}

	[[nodiscard]] Core::Result<ComPtr<IMMDeviceEnumerator>> create_enumerator( )
	{
		ComPtr<IMMDeviceEnumerator> enumerator;

		const HRESULT hr = CoCreateInstance(
			__uuidof( MMDeviceEnumerator), nullptr, CLSCTX_ALL, IID_PPV_ARGS( &enumerator )
		);

		if ( FAILED( hr ) )
			return std::unexpected( map_hresult( hr ) );

		return enumerator;
	}

	[[nodiscard]] Core::Status set_sysfx( const ComPtr<IMMDevice>& device, const bool disabled )
	{
		ComPtr<IPropertyStore> store;

		if ( const HRESULT hr = device->OpenPropertyStore( STGM_READWRITE, &store ); FAILED( hr ) )
			return std::unexpected( map_hresult( hr ) );

		PROPVARIANT value;
		PropVariantInit( &value );
		value.vt    = VT_UI4;
		value.ulVal = disabled ? 1u : 0u;

		const HRESULT hr = store->SetValue( PKEY_AudioEndpoint_Disable_SysFx, value );
		PropVariantClear( &value );

		if ( FAILED( hr ) )
			return std::unexpected( map_hresult( hr ) );

		return {};
	}
}

namespace Modules
{
	AudioModule::AudioModule( )
	{
		refresh_all( );
	}

	Core::Result<bool> AudioModule::load_enhancements( )
	{
		ComGuard com;

		if ( !com.ok( ) )
			return std::unexpected( Core::Error::Unknown );

		const auto enumerator = create_enumerator( );

		if ( !enumerator )
			return std::unexpected( enumerator.error( ) );

		ComPtr<IMMDevice> device;

		if ( const HRESULT hr = ( *enumerator )->GetDefaultAudioEndpoint( eRender, eConsole, &device ); FAILED( hr ) )
			return std::unexpected( map_hresult( hr ) );

		ComPtr<IPropertyStore> store;

		if ( const HRESULT hr = device->OpenPropertyStore( STGM_READ, &store ); FAILED( hr ) )
			return std::unexpected( map_hresult( hr ) );

		PROPVARIANT value;
		PropVariantInit( &value );

		const HRESULT hr = store->GetValue( PKEY_AudioEndpoint_Disable_SysFx, &value );

		bool disabled = false;

		if ( SUCCEEDED( hr ) && value.vt == VT_UI4 )
			disabled = value.ulVal != 0;

		PropVariantClear( &value );

		if ( FAILED( hr ) )
			return std::unexpected( map_hresult( hr ) );

		return !disabled;
	}

	Core::Status AudioModule::apply_enhancements( const bool& enabled )
	{
		ComGuard com;

		if ( !com.ok( ) )
			return std::unexpected( Core::Error::Unknown );

		const auto enumerator = create_enumerator( );

		if ( !enumerator )
			return std::unexpected( enumerator.error( ) );

		ComPtr<IMMDeviceCollection> devices;

		if ( const HRESULT hr = ( *enumerator )->EnumAudioEndpoints( eAll, DEVICE_STATE_ACTIVE, &devices ); FAILED( hr ) )
			return std::unexpected( map_hresult( hr ) );

		UINT device_count = 0;
		devices->GetCount( &device_count );

		for ( UINT i = 0; i < device_count; ++i )
		{
			ComPtr<IMMDevice> device;

			if ( FAILED( devices->Item( i, &device ) ) )
				continue;

			if ( const auto status = set_sysfx( device, !enabled ); !status )
				return status;
		}

		return {};
	}
}

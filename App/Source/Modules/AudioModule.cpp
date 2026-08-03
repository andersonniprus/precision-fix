#include "Stdafx.hpp"
#include "Modules/AudioModule.hpp"
#include "Utils/Com.hpp"
#include "Utils/Registry.hpp"

#include <initguid.h>
#include <mmdeviceapi.h>
#include <propsys.h>
#include <functiondiscoverykeys_devpkey.h>

namespace
{
	using Microsoft::WRL::ComPtr;

	// {b3f8fa53-0004-438e-9003-51a46e139bfc},3 — allow exclusive mode
	// {b3f8fa53-0004-438e-9003-51a46e139bfc},4 — give exclusive mode priority
	constexpr wchar_t exclusive_allow[ ]    = L"{b3f8fa53-0004-438e-9003-51a46e139bfc},3";
	constexpr wchar_t exclusive_priority[ ] = L"{b3f8fa53-0004-438e-9003-51a46e139bfc},4";
	constexpr wchar_t mmdevices_render[ ]   = LR"(SOFTWARE\Microsoft\Windows\CurrentVersion\MMDevices\Audio\Render)";

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

	[[nodiscard]] Core::Result<std::vector<std::wstring>> render_property_keys( )
	{
		const auto devices = Utils::Registry::enumerate_subkeys( HKEY_LOCAL_MACHINE, mmdevices_render );

		if ( !devices )
			return std::unexpected( devices.error( ) );

		std::vector<std::wstring> keys;
		keys.reserve( devices->size( ) );

		for ( const auto& id : *devices )
		{
			keys.push_back( std::wstring { mmdevices_render } + L'\\' + id + L"\\Properties" );
		}

		return keys;
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
		const Utils::Com::Guard com;

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
		const Utils::Com::Guard com;

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

	Core::Result<bool> AudioModule::load_exclusive_mode( )
	{
		const auto keys = render_property_keys( );

		if ( !keys )
			return std::unexpected( keys.error( ) );

		if ( keys->empty( ) )
			return true;

		const auto value = Utils::Registry::read_dword( HKEY_LOCAL_MACHINE, keys->front( ).c_str( ), exclusive_allow );

		if ( !value )
			return true;

		return *value == 0;
	}

	Core::Status AudioModule::apply_exclusive_mode( const bool& enabled )
	{
		const auto keys = render_property_keys( );

		if ( !keys )
			return std::unexpected( keys.error( ) );

		if ( keys->empty( ) )
			return std::unexpected( Core::Error::NotFound );

		const std::uint32_t deny = enabled ? 0u : 1u;

		for ( const auto& key : *keys )
		{
			if ( const auto status = Utils::Registry::write_dword( HKEY_LOCAL_MACHINE, key.c_str( ), exclusive_allow, deny ); !status )
				return status;

			if ( const auto status = Utils::Registry::write_dword( HKEY_LOCAL_MACHINE, key.c_str( ), exclusive_priority, deny ); !status )
				return status;
		}

		return {};
	}
}

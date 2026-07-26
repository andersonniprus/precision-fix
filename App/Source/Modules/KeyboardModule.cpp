#include "Stdafx.hpp"
#include "Modules/KeyboardModule.hpp"
#include "Utils/Power.hpp"
#include "Utils/Registry.hpp"

namespace
{
	constexpr std::uint32_t max_repeat_delay = 3;
	constexpr std::uint32_t max_repeat_rate  = 31;

	constexpr wchar_t queue_size_key[ ]  = LR"(SYSTEM\CurrentControlSet\Services\kbdclass\Parameters)";
	constexpr wchar_t usb_service_key[ ] = LR"(SYSTEM\CurrentControlSet\Services\USB)";
}

namespace Modules
{
	KeyboardModule::KeyboardModule( )
	{
		refresh_all( );
	}

	Core::Result<bool> KeyboardModule::load_filter_keys( )
	{
		FILTERKEYS filter_keys { .cbSize = sizeof( FILTERKEYS ) };

		if ( !SystemParametersInfoW( SPI_GETFILTERKEYS, sizeof( filter_keys ), &filter_keys, 0 ) )
			return std::unexpected( Core::Error::Unknown );

		return ( filter_keys.dwFlags & FKF_FILTERKEYSON ) != 0;
	}

	Core::Status KeyboardModule::apply_filter_keys( const bool& enabled )
	{
		FILTERKEYS filter_keys { .cbSize = sizeof( FILTERKEYS ) };

		if ( !SystemParametersInfoW( SPI_GETFILTERKEYS, sizeof( filter_keys ), &filter_keys, 0 ) )
			return std::unexpected( Core::Error::Unknown );

		filter_keys.dwFlags = enabled ? ( filter_keys.dwFlags | FKF_FILTERKEYSON ) : ( filter_keys.dwFlags & ~FKF_FILTERKEYSON );

		if ( !SystemParametersInfoW( SPI_SETFILTERKEYS, sizeof( filter_keys ), &filter_keys, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE ) )
			return std::unexpected( Core::Error::Unknown );

		return {};
	}

	Core::Result<bool> KeyboardModule::load_sticky_keys( )
	{
		STICKYKEYS sticky_keys { .cbSize = sizeof( STICKYKEYS ) };

		if ( !SystemParametersInfoW( SPI_GETSTICKYKEYS, sizeof( sticky_keys ), &sticky_keys, 0 ) )
			return std::unexpected( Core::Error::Unknown );

		return ( sticky_keys.dwFlags & SKF_STICKYKEYSON ) != 0;
	}

	Core::Status KeyboardModule::apply_sticky_keys( const bool& enabled )
	{
		STICKYKEYS sticky_keys { .cbSize = sizeof( STICKYKEYS ) };

		if ( !SystemParametersInfoW( SPI_GETSTICKYKEYS, sizeof( sticky_keys ), &sticky_keys, 0 ) )
			return std::unexpected( Core::Error::Unknown );

		sticky_keys.dwFlags = enabled ? ( sticky_keys.dwFlags | SKF_STICKYKEYSON ) : ( sticky_keys.dwFlags & ~SKF_STICKYKEYSON );

		if ( !SystemParametersInfoW( SPI_SETSTICKYKEYS, sizeof( sticky_keys ), &sticky_keys, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE ) )
			return std::unexpected( Core::Error::Unknown );

		return {};
	}

	Core::Result<bool> KeyboardModule::load_toggle_keys( )
	{
		TOGGLEKEYS toggle_keys { .cbSize = sizeof( TOGGLEKEYS ) };

		if ( !SystemParametersInfoW( SPI_GETTOGGLEKEYS, sizeof( toggle_keys ), &toggle_keys, 0 ) )
			return std::unexpected( Core::Error::Unknown );

		return ( toggle_keys.dwFlags & TKF_TOGGLEKEYSON ) != 0;
	}

	Core::Status KeyboardModule::apply_toggle_keys( const bool& enabled )
	{
		TOGGLEKEYS toggle_keys { .cbSize = sizeof( TOGGLEKEYS ) };

		if ( !SystemParametersInfoW( SPI_GETTOGGLEKEYS, sizeof( toggle_keys ), &toggle_keys, 0 ) )
			return std::unexpected( Core::Error::Unknown );

		toggle_keys.dwFlags = enabled ? ( toggle_keys.dwFlags | TKF_TOGGLEKEYSON ) : ( toggle_keys.dwFlags & ~TKF_TOGGLEKEYSON );

		if ( !SystemParametersInfoW( SPI_SETTOGGLEKEYS, sizeof( toggle_keys ), &toggle_keys, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE ) )
			return std::unexpected( Core::Error::Unknown );

		return {};
	}

	Core::Result<std::uint32_t> KeyboardModule::load_repeat_delay( )
	{
		int delay {};

		if ( !SystemParametersInfoW( SPI_GETKEYBOARDDELAY, 0, &delay, 0 ) )
			return std::unexpected( Core::Error::Unknown );

		return static_cast<std::uint32_t>( delay );
	}

	Core::Status KeyboardModule::apply_repeat_delay( const std::uint32_t& value )
	{
		if ( value > max_repeat_delay )
			return std::unexpected( Core::Error::InvalidValue );

		if ( !SystemParametersInfoW( SPI_SETKEYBOARDDELAY, value, nullptr, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE ) )
			return std::unexpected( Core::Error::Unknown );

		return {};
	}

	Core::Result<std::uint32_t> KeyboardModule::load_repeat_rate( )
	{
		DWORD speed {};

		if ( !SystemParametersInfoW( SPI_GETKEYBOARDSPEED, 0, &speed, 0 ) )
			return std::unexpected( Core::Error::Unknown );

		return static_cast<std::uint32_t>( speed );
	}

	Core::Status KeyboardModule::apply_repeat_rate( const std::uint32_t& value )
	{
		if ( value > max_repeat_rate )
			return std::unexpected( Core::Error::InvalidValue );

		if ( !SystemParametersInfoW( SPI_SETKEYBOARDSPEED, value, nullptr, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE ) )
			return std::unexpected( Core::Error::Unknown );

		return {};
	}

	Core::Result<bool> KeyboardModule::load_usb_selective_suspend( )
	{
		const auto value = Utils::Power::read_ac_value( Utils::Power::usb_settings_subgroup, Utils::Power::usb_selective_suspend );

		if ( !value )
			return std::unexpected( value.error( ) );

		return *value != 0;
	}

	Core::Status KeyboardModule::apply_usb_selective_suspend( const bool& enabled )
	{
		return Utils::Power::write_ac_value( Utils::Power::usb_settings_subgroup, Utils::Power::usb_selective_suspend, enabled ? 1u : 0u );
	}

	Core::Result<std::uint32_t> KeyboardModule::load_data_queue_size( )
	{
		return Utils::Registry::read_dword( HKEY_LOCAL_MACHINE, queue_size_key, L"KeyboardDataQueueSize" );
	}

	Core::Status KeyboardModule::apply_data_queue_size( const std::uint32_t& value )
	{
		if ( value == 0 || value > 100 )
			return std::unexpected( Core::Error::InvalidValue );

		return Utils::Registry::write_dword( HKEY_LOCAL_MACHINE, queue_size_key, L"KeyboardDataQueueSize", value );
	}

	Core::Result<bool> KeyboardModule::load_usb_selective_suspend_global( )
	{
		const auto value = Utils::Registry::read_dword( HKEY_LOCAL_MACHINE, usb_service_key, L"DisableSelectiveSuspend" );

		if ( !value )
			return true;

		return *value == 0;
	}

	Core::Status KeyboardModule::apply_usb_selective_suspend_global( const bool& enabled )
	{
		return Utils::Registry::write_dword( HKEY_LOCAL_MACHINE, usb_service_key, L"DisableSelectiveSuspend", enabled ? 0u : 1u );
	}
}

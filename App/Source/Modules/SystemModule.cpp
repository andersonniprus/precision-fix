#include "Stdafx.hpp"
#include "Modules/SystemModule.hpp"
#include "Utils/Registry.hpp"
#include "Utils/Power.hpp"

namespace
{
	constexpr wchar_t priority_control_key[ ]   = LR"(SYSTEM\CurrentControlSet\Control\PriorityControl)";
	constexpr wchar_t multimedia_profile_key[ ] = LR"(SOFTWARE\Microsoft\Windows NT\CurrentVersion\Multimedia\SystemProfile)";
	constexpr wchar_t prefetch_key[ ]           = LR"(SYSTEM\CurrentControlSet\Control\Session Manager\Memory Management\PrefetchParameters)";
	constexpr wchar_t power_throttling_key[ ]   = LR"(SYSTEM\CurrentControlSet\Control\Power\PowerThrottling)";
	constexpr wchar_t power_key[ ]              = LR"(SYSTEM\CurrentControlSet\Control\Power)";
	constexpr wchar_t kernel_key[ ]             = LR"(SYSTEM\CurrentControlSet\Control\Session Manager\Kernel)";

	constexpr std::uint32_t prefetch_enabled  = 3;
	constexpr std::uint32_t prefetch_disabled = 0;
}

namespace Modules
{
	SystemModule::SystemModule( )
	{
		refresh_all( );
	}

	Core::Result<std::uint32_t> SystemModule::load_priority_separation( )
	{
		return Utils::Registry::read_dword( HKEY_LOCAL_MACHINE, priority_control_key, L"Win32PrioritySeparation" );
	}

	Core::Status SystemModule::apply_priority_separation( const std::uint32_t& value )
	{
		return Utils::Registry::write_dword( HKEY_LOCAL_MACHINE, priority_control_key, L"Win32PrioritySeparation", value );
	}

	Core::Result<std::uint32_t> SystemModule::load_responsiveness( )
	{
		return Utils::Registry::read_dword( HKEY_LOCAL_MACHINE, multimedia_profile_key, L"SystemResponsiveness" );
	}

	Core::Status SystemModule::apply_responsiveness( const std::uint32_t& value )
	{
		return Utils::Registry::write_dword( HKEY_LOCAL_MACHINE, multimedia_profile_key, L"SystemResponsiveness", value );
	}

	Core::Result<std::uint32_t> SystemModule::load_network_throttling( )
	{
		return Utils::Registry::read_dword( HKEY_LOCAL_MACHINE, multimedia_profile_key, L"NetworkThrottlingIndex" );
	}

	Core::Status SystemModule::apply_network_throttling( const std::uint32_t& value )
	{
		return Utils::Registry::write_dword( HKEY_LOCAL_MACHINE, multimedia_profile_key, L"NetworkThrottlingIndex", value );
	}

	Core::Result<bool> SystemModule::load_prefetch( )
	{
		const auto value = Utils::Registry::read_dword( HKEY_LOCAL_MACHINE, prefetch_key, L"EnablePrefetcher" );

		if ( !value )
			return std::unexpected( value.error( ) );

		return *value != prefetch_disabled;
	}

	Core::Status SystemModule::apply_prefetch( const bool& enabled )
	{
		return Utils::Registry::write_dword(
			HKEY_LOCAL_MACHINE, prefetch_key, L"EnablePrefetcher", enabled ? prefetch_enabled : prefetch_disabled
		);
	}

	Core::Result<bool> SystemModule::load_power_throttling( )
	{
		const auto value = Utils::Registry::read_dword( HKEY_LOCAL_MACHINE, power_throttling_key, L"PowerThrottlingOff" );

		if ( !value )
			return true;

		return *value == 0;
	}

	Core::Status SystemModule::apply_power_throttling( const bool& enabled )
	{
		return Utils::Registry::write_dword( HKEY_LOCAL_MACHINE, power_throttling_key, L"PowerThrottlingOff", enabled ? 0u : 1u );
	}

	Core::Result<bool> SystemModule::load_timer_coalescing( )
	{
		const auto value = Utils::Registry::read_dword( HKEY_LOCAL_MACHINE, power_key, L"CoalescingTimerInterval" );

		if ( !value )
			return true;

		return *value != 0;
	}

	Core::Status SystemModule::apply_timer_coalescing( const bool& enabled )
	{
		if ( enabled )
			return Utils::Registry::delete_value( HKEY_LOCAL_MACHINE, power_key, L"CoalescingTimerInterval" );

		return Utils::Registry::write_dword( HKEY_LOCAL_MACHINE, power_key, L"CoalescingTimerInterval", 0 );
	}

	Core::Result<bool> SystemModule::load_processor_idle_states( )
	{
		const auto value = Utils::Power::read_ac_value( Utils::Power::processor_settings_subgroup, Utils::Power::processor_idle_disable );

		if ( !value )
			return std::unexpected( value.error( ) );

		return *value == 0;
	}

	Core::Status SystemModule::apply_processor_idle_states( const bool& enabled )
	{
		return Utils::Power::write_ac_value(
			Utils::Power::processor_settings_subgroup, Utils::Power::processor_idle_disable, enabled ? 0u : 1u
		);
	}

	Core::Result<bool> SystemModule::load_tsx( )
	{
		const auto value = Utils::Registry::read_dword( HKEY_LOCAL_MACHINE, kernel_key, L"DisableTsx" );

		if ( !value )
			return true;

		return *value == 0;
	}

	Core::Status SystemModule::apply_tsx( const bool& enabled )
	{
		return Utils::Registry::write_dword( HKEY_LOCAL_MACHINE, kernel_key, L"DisableTsx", enabled ? 0u : 1u );
	}

	Core::Result<std::uint32_t> SystemModule::load_foreground_lock_timeout( )
	{
		DWORD value {};

		if ( !SystemParametersInfoW( SPI_GETFOREGROUNDLOCKTIMEOUT, 0, &value, 0 ) )
			return std::unexpected( Core::Error::Unknown );

		return static_cast<std::uint32_t>( value );
	}

	Core::Status SystemModule::apply_foreground_lock_timeout( const std::uint32_t& value )
	{
		const auto data = reinterpret_cast<PVOID>( static_cast<UINT_PTR>( value ) );

		if ( !SystemParametersInfoW( SPI_SETFOREGROUNDLOCKTIMEOUT, 0, data, SPIF_SENDCHANGE ) )
			return std::unexpected( Core::Error::Unknown );

		return {};
	}

	Core::Result<std::uint32_t> SystemModule::load_menu_show_delay( )
	{
		int value {};

		if ( !SystemParametersInfoW( SPI_GETMENUSHOWDELAY, 0, &value, 0 ) )
			return std::unexpected( Core::Error::Unknown );

		return static_cast<std::uint32_t>( value );
	}

	Core::Status SystemModule::apply_menu_show_delay( const std::uint32_t& value )
	{
		if ( !SystemParametersInfoW( SPI_SETMENUSHOWDELAY, value, nullptr, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE ) )
			return std::unexpected( Core::Error::Unknown );

		return {};
	}

	Core::Result<bool> SystemModule::load_ui_animations( )
	{
		BOOL enabled = TRUE;

		if ( !SystemParametersInfoW( SPI_GETMENUANIMATION, 0, &enabled, 0 ) )
			return std::unexpected( Core::Error::Unknown );

		return enabled != FALSE;
	}

	Core::Status SystemModule::apply_ui_animations( const bool& enabled )
	{
		constexpr UINT actions[ ] = {
			SPI_SETMENUANIMATION, SPI_SETCOMBOBOXANIMATION, SPI_SETTOOLTIPANIMATION, SPI_SETCURSORSHADOW, SPI_SETDROPSHADOW,
		};

		const auto data = reinterpret_cast<PVOID>( static_cast<UINT_PTR>( enabled ? TRUE : FALSE ) );

		for ( const auto action : actions )
		{
			if ( !SystemParametersInfoW( action, 0, data, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE ) )
				return std::unexpected( Core::Error::Unknown );
		}

		return {};
	}
}

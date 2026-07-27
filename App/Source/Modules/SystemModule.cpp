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

	constexpr wchar_t explorer_advanced_key[ ]     = LR"(SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\Advanced)";
	constexpr wchar_t personalize_key[ ]           = LR"(SOFTWARE\Microsoft\Windows\CurrentVersion\Themes\Personalize)";
	constexpr wchar_t session_manager_power_key[ ] = LR"(SYSTEM\CurrentControlSet\Control\Session Manager\Power)";
	constexpr wchar_t filesystem_key[ ]            = LR"(SYSTEM\CurrentControlSet\Control\FileSystem)";
	constexpr wchar_t system_restore_key[ ]        = LR"(SOFTWARE\Microsoft\Windows NT\CurrentVersion\SystemRestore)";
	constexpr wchar_t pushnotifications_key[ ]     = LR"(SOFTWARE\Microsoft\Windows\CurrentVersion\PushNotifications)";
	constexpr wchar_t desktop_key[ ]               = LR"(Control Panel\Desktop)";
	constexpr wchar_t control_key[ ]               = LR"(SYSTEM\CurrentControlSet\Control)";
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

	Core::Result<bool> SystemModule::load_show_file_extensions( )
	{
		const auto value = Utils::Registry::read_dword( HKEY_CURRENT_USER, explorer_advanced_key, L"HideFileExt" );

		if ( !value )
			return false;

		return *value == 0;
	}

	Core::Status SystemModule::apply_show_file_extensions( const bool& enabled )
	{
		return Utils::Registry::write_dword( HKEY_CURRENT_USER, explorer_advanced_key, L"HideFileExt", enabled ? 0u : 1u );
	}

	Core::Result<bool> SystemModule::load_dark_mode( )
	{
		const auto value = Utils::Registry::read_dword( HKEY_CURRENT_USER, personalize_key, L"AppsUseLightTheme" );

		if ( !value )
			return false;

		return *value == 0;
	}

	Core::Status SystemModule::apply_dark_mode( const bool& enabled )
	{
		return Utils::Registry::write_dword( HKEY_CURRENT_USER, personalize_key, L"AppsUseLightTheme", enabled ? 0u : 1u );
	}

	Core::Result<bool> SystemModule::load_hibernate_enabled( )
	{
		const auto value = Utils::Registry::read_dword( HKEY_LOCAL_MACHINE, power_key, L"HibernateEnabled" );

		if ( !value )
			return true;

		return *value != 0;
	}

	Core::Status SystemModule::apply_hibernate_enabled( const bool& enabled )
	{
		return Utils::Registry::write_dword( HKEY_LOCAL_MACHINE, power_key, L"HibernateEnabled", enabled ? 1u : 0u );
	}

	Core::Result<bool> SystemModule::load_fast_startup( )
	{
		const auto value = Utils::Registry::read_dword( HKEY_LOCAL_MACHINE, session_manager_power_key, L"HiberbootEnabled" );

		if ( !value )
			return true;

		return *value != 0;
	}

	Core::Status SystemModule::apply_fast_startup( const bool& enabled )
	{
		return Utils::Registry::write_dword( HKEY_LOCAL_MACHINE, session_manager_power_key, L"HiberbootEnabled", enabled ? 1u : 0u );
	}

	Core::Result<bool> SystemModule::load_sleep_diagnostics( )
	{
		const auto value = Utils::Registry::read_dword( HKEY_LOCAL_MACHINE, session_manager_power_key, L"SleepStudyDisabled" );

		if ( !value )
			return true;

		return *value == 0;
	}

	Core::Status SystemModule::apply_sleep_diagnostics( const bool& enabled )
	{
		return Utils::Registry::write_dword( HKEY_LOCAL_MACHINE, session_manager_power_key, L"SleepStudyDisabled", enabled ? 0u : 1u );
	}

	Core::Result<bool> SystemModule::load_energy_estimation( )
	{
		const auto value = Utils::Registry::read_dword( HKEY_LOCAL_MACHINE, power_key, L"EnergyEstimationEnabled" );

		if ( !value )
			return true;

		return *value != 0;
	}

	Core::Status SystemModule::apply_energy_estimation( const bool& enabled )
	{
		return Utils::Registry::write_dword( HKEY_LOCAL_MACHINE, power_key, L"EnergyEstimationEnabled", enabled ? 1u : 0u );
	}

	Core::Result<bool> SystemModule::load_modern_standby( )
	{
		const auto value = Utils::Registry::read_dword( HKEY_LOCAL_MACHINE, power_key, L"CsEnabled" );

		if ( !value )
			return true;

		return *value != 0;
	}

	Core::Status SystemModule::apply_modern_standby( const bool& enabled )
	{
		return Utils::Registry::write_dword( HKEY_LOCAL_MACHINE, power_key, L"CsEnabled", enabled ? 1u : 0u );
	}

	Core::Result<std::uint32_t> SystemModule::load_svchost_split_threshold( )
	{
		const auto value = Utils::Registry::read_dword( HKEY_LOCAL_MACHINE, control_key, L"SvcHostSplitThresholdInKB" );

		if ( value )
			return *value;

		if ( value.error( ) != Core::Error::NotFound )
			return std::unexpected( value.error( ) );

		MEMORYSTATUSEX status { .dwLength = sizeof( status ) };

		if ( !GlobalMemoryStatusEx( &status ) )
			return std::unexpected( Core::Error::Unknown );

		return static_cast<std::uint32_t>( status.ullTotalPhys / 1024 );
	}

	Core::Status SystemModule::apply_svchost_split_threshold( const std::uint32_t& value )
	{
		return Utils::Registry::write_dword( HKEY_LOCAL_MACHINE, control_key, L"SvcHostSplitThresholdInKB", value );
	}

	Core::Result<std::uint32_t> SystemModule::load_disable_last_access_timestamps( )
	{
		const auto value = Utils::Registry::read_dword( HKEY_LOCAL_MACHINE, filesystem_key, L"NtfsDisableLastAccessUpdate" );

		if ( !value )
			return 2;

		return *value;
	}

	Core::Status SystemModule::apply_disable_last_access_timestamps( const std::uint32_t& value )
	{
		if ( value > 3 )
			return std::unexpected( Core::Error::InvalidValue );

		return Utils::Registry::write_dword( HKEY_LOCAL_MACHINE, filesystem_key, L"NtfsDisableLastAccessUpdate", value );
	}

	Core::Result<std::uint32_t> SystemModule::load_disable_8dot3_name_creation( )
	{
		const auto value = Utils::Registry::read_dword( HKEY_LOCAL_MACHINE, filesystem_key, L"NtfsDisable8dot3NameCreation" );

		if ( !value )
			return 2;

		return *value;
	}

	Core::Status SystemModule::apply_disable_8dot3_name_creation( const std::uint32_t& value )
	{
		if ( value > 3 )
			return std::unexpected( Core::Error::InvalidValue );

		return Utils::Registry::write_dword( HKEY_LOCAL_MACHINE, filesystem_key, L"NtfsDisable8dot3NameCreation", value );
	}

	Core::Result<std::uint32_t> SystemModule::load_restore_point_frequency( )
	{
		const auto value = Utils::Registry::read_dword( HKEY_LOCAL_MACHINE, system_restore_key, L"SystemRestorePointCreationFrequency" );

		if ( !value )
			return 1440;

		return *value;
	}

	Core::Status SystemModule::apply_restore_point_frequency( const std::uint32_t& value )
	{
		return Utils::Registry::write_dword( HKEY_LOCAL_MACHINE, system_restore_key, L"SystemRestorePointCreationFrequency", value );
	}

	Core::Result<bool> SystemModule::load_toast_notifications( )
	{
		const auto value = Utils::Registry::read_dword( HKEY_CURRENT_USER, pushnotifications_key, L"ToastEnabled" );

		if ( !value )
			return true;

		return *value != 0;
	}

	Core::Status SystemModule::apply_toast_notifications( const bool& enabled )
	{
		return Utils::Registry::write_dword( HKEY_CURRENT_USER, pushnotifications_key, L"ToastEnabled", enabled ? 1u : 0u );
	}

	Core::Result<bool> SystemModule::load_fast_app_termination( )
	{
		const auto value = Utils::Registry::read_sz( HKEY_CURRENT_USER, desktop_key, L"AutoEndTasks" );

		if ( !value )
			return false;

		return *value == L"1";
	}

	Core::Status SystemModule::apply_fast_app_termination( const bool& enabled )
	{
		if ( !enabled )
		{
			if ( const auto status = Utils::Registry::delete_value( HKEY_CURRENT_USER, desktop_key, L"AutoEndTasks" ); !status )
				return status;

			if ( const auto status = Utils::Registry::delete_value( HKEY_CURRENT_USER, desktop_key, L"HungAppTimeout" ); !status )
				return status;

			if ( const auto status = Utils::Registry::delete_value( HKEY_CURRENT_USER, desktop_key, L"WaitToKillAppTimeout" ); !status )
				return status;

			return Utils::Registry::delete_value( HKEY_CURRENT_USER, desktop_key, L"LowLevelHooksTimeout" );
		}

		if ( const auto status = Utils::Registry::write_sz( HKEY_CURRENT_USER, desktop_key, L"AutoEndTasks", L"1" ); !status )
			return status;

		if ( const auto status = Utils::Registry::write_sz( HKEY_CURRENT_USER, desktop_key, L"HungAppTimeout", L"1000" ); !status )
			return status;

		if ( const auto status = Utils::Registry::write_sz( HKEY_CURRENT_USER, desktop_key, L"WaitToKillAppTimeout", L"1000" ); !status )
			return status;

		return Utils::Registry::write_sz( HKEY_CURRENT_USER, desktop_key, L"LowLevelHooksTimeout", L"1000" );
	}
}

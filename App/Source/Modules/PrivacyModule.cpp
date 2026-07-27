#include "Stdafx.hpp"
#include "Modules/PrivacyModule.hpp"
#include "Utils/Registry.hpp"
#include "Utils/TaskScheduler.hpp"

namespace
{
	struct RegistryEntry
	{
		const wchar_t* key;
		const wchar_t* value;
		std::uint32_t tweak;
	};

	[[nodiscard]] Core::Status apply_bundle( const HKEY root, const std::span<const RegistryEntry> entries, const bool enabled )
	{
		for ( const auto& entry : entries )
		{
			const auto status = enabled
			                    ? Utils::Registry::delete_value( root, entry.key, entry.value )
			                    : Utils::Registry::write_dword( root, entry.key, entry.value, entry.tweak );

			if ( !status )
				return status;
		}

		return {};
	}

	[[nodiscard]] Core::Result<bool> load_bundle( const HKEY root, const std::span<const RegistryEntry> entries )
	{
		const auto& first = entries.front( );
		const auto value  = Utils::Registry::read_dword( root, first.key, first.value );

		if ( !value )
			return true;

		return *value != first.tweak;
	}

	constexpr RegistryEntry telemetry_entries[ ] = {
		{ LR"(SOFTWARE\Policies\Microsoft\Windows\DataCollection)", L"AllowTelemetry", 0 },
	};

	constexpr RegistryEntry advertising_id_entries[ ] = {
		{ LR"(SOFTWARE\Microsoft\Windows\CurrentVersion\AdvertisingInfo)", L"Enabled", 0 },
	};

	constexpr RegistryEntry activity_feed_entries[ ] = {
		{ LR"(SOFTWARE\Policies\Microsoft\Windows\System)", L"EnableActivityFeed", 0 },
		{ LR"(SOFTWARE\Policies\Microsoft\Windows\System)", L"PublishUserActivities", 0 },
		{ LR"(SOFTWARE\Policies\Microsoft\Windows\System)", L"UploadUserActivities", 0 },
	};

	constexpr RegistryEntry tailored_experiences_entries[ ] = {
		{ LR"(SOFTWARE\Microsoft\Windows\CurrentVersion\Privacy)", L"TailoredExperiencesWithDiagnosticDataEnabled", 0 },
	};

	constexpr RegistryEntry game_dvr_hkcu_entries[ ] = {
		{ LR"(System\GameConfigStore)", L"GameDVR_Enabled", 0 },
		{ LR"(Software\Microsoft\Windows\CurrentVersion\GameDVR)", L"AppCaptureEnabled", 0 },
		{ LR"(Software\Microsoft\Windows\CurrentVersion\GameDVR)", L"AudioCaptureEnabled", 0 },
		{ LR"(Software\Microsoft\Windows\CurrentVersion\GameDVR)", L"CursorCaptureEnabled", 0 },
		{ LR"(Software\Microsoft\Windows\CurrentVersion\GameDVR)", L"HistoricalCaptureEnabled", 0 },
	};

	constexpr RegistryEntry game_dvr_hklm_entries[ ] = {
		{ LR"(Software\Policies\Microsoft\Windows\GameDVR)", L"AllowgameDVR", 0 },
	};

	constexpr RegistryEntry content_suggestions_entries[ ] = {
		{ LR"(SOFTWARE\Microsoft\Windows\CurrentVersion\ContentDeliveryManager)", L"ContentDeliveryAllowed", 0 },
		{ LR"(SOFTWARE\Microsoft\Windows\CurrentVersion\ContentDeliveryManager)", L"OemPreInstalledAppsEnabled", 0 },
		{ LR"(SOFTWARE\Microsoft\Windows\CurrentVersion\ContentDeliveryManager)", L"PreInstalledAppsEnabled", 0 },
		{ LR"(SOFTWARE\Microsoft\Windows\CurrentVersion\ContentDeliveryManager)", L"PreInstalledAppsEverEnabled", 0 },
		{ LR"(SOFTWARE\Microsoft\Windows\CurrentVersion\ContentDeliveryManager)", L"SilentInstalledAppsEnabled", 0 },
		{ LR"(SOFTWARE\Microsoft\Windows\CurrentVersion\ContentDeliveryManager)", L"SubscribedContentEnabled", 0 },
	};

	constexpr RegistryEntry error_reporting_entries[ ] = {
		{ LR"(SOFTWARE\Microsoft\Windows\Windows Error Reporting)", L"Disabled", 1 },
		{ LR"(SOFTWARE\Policies\Microsoft\Windows\Windows Error Reporting)", L"Disabled", 1 },
		{ LR"(SOFTWARE\Policies\Microsoft\Windows\Windows Error Reporting)", L"DoReport", 0 },
		{ LR"(SOFTWARE\Policies\Microsoft\Windows\Windows Error Reporting)", L"LoggingDisabled", 1 },
	};

	constexpr RegistryEntry diagnostic_execution_entries[ ] = {
		{ LR"(SOFTWARE\Policies\Microsoft\Windows\AppCompat)", L"AITEnable", 0 },
		{ LR"(SOFTWARE\Policies\Microsoft\Windows\AppCompat)", L"DisableInventory", 1 },
		{ LR"(SOFTWARE\Policies\Microsoft\Windows\AppCompat)", L"DisableUAR", 1 },
	};

	constexpr RegistryEntry location_services_entries[ ] = {
		{ LR"(SOFTWARE\Policies\Microsoft\Windows\LocationAndSensors)", L"DisableLocation", 1 },
		{ LR"(SOFTWARE\Policies\Microsoft\Windows\LocationAndSensors)", L"DisableLocationScripting", 1 },
		{ LR"(SOFTWARE\Policies\Microsoft\Windows\LocationAndSensors)", L"DisableSensors", 1 },
		{ LR"(SOFTWARE\Policies\Microsoft\Windows\LocationAndSensors)", L"DisableWindowsLocationProvider", 1 },
	};

	constexpr RegistryEntry feedback_prompts_entries[ ] = {
		{ LR"(SOFTWARE\Policies\Microsoft\Assistance\Client\1.0)", L"NoActiveHelp", 1 },
		{ LR"(SOFTWARE\Policies\Microsoft\Assistance\Client\1.0)", L"NoExplicitFeedback", 1 },
	};

	constexpr RegistryEntry news_and_interests_entries[ ] = {
		{ LR"(SOFTWARE\Policies\Microsoft)", L"AllowNewsAndInterests", 0 },
	};

	constexpr RegistryEntry windows_feeds_entries[ ] = {
		{ LR"(SOFTWARE\Policies\Microsoft\Windows\Windows Feeds)", L"EnableFeeds", 0 },
	};

	constexpr RegistryEntry setting_sync_entries[ ] = {
		{ LR"(SOFTWARE\Microsoft\Windows\CurrentVersion\SettingSync)", L"SyncPolicy", 5 },
		{ LR"(SOFTWARE\Microsoft\Windows\CurrentVersion\SettingSync\Groups\Accessibility)", L"Enabled", 0 },
		{ LR"(SOFTWARE\Microsoft\Windows\CurrentVersion\SettingSync\Groups\AppSync)", L"Enabled", 0 },
		{ LR"(SOFTWARE\Microsoft\Windows\CurrentVersion\SettingSync\Groups\BrowserSettings)", L"Enabled", 0 },
		{ LR"(SOFTWARE\Microsoft\Windows\CurrentVersion\SettingSync\Groups\DesktopTheme)", L"Enabled", 0 },
		{ LR"(SOFTWARE\Microsoft\Windows\CurrentVersion\SettingSync\Groups\Language)", L"Enabled", 0 },
		{ LR"(SOFTWARE\Microsoft\Windows\CurrentVersion\SettingSync\Groups\PackageState)", L"Enabled", 0 },
		{ LR"(SOFTWARE\Microsoft\Windows\CurrentVersion\SettingSync\Groups\Personalization)", L"Enabled", 0 },
		{ LR"(SOFTWARE\Microsoft\Windows\CurrentVersion\SettingSync\Groups\StartLayout)", L"Enabled", 0 },
		{ LR"(SOFTWARE\Microsoft\Windows\CurrentVersion\SettingSync\Groups\Windows)", L"Enabled", 0 },
	};

	constexpr const wchar_t* diagnostic_task_paths[ ] = {
		LR"(\Microsoft\Windows\Application Experience\Microsoft Compatibility Appraiser)",
		LR"(\Microsoft\Windows\Application Experience\ProgramDataUpdater)",
		LR"(\Microsoft\Windows\Application Experience\StartupAppTask)",
		LR"(\Microsoft\Windows\Application Experience\AitAgent)",
		LR"(\Microsoft\Windows\Autochk\Proxy)",
		LR"(\Microsoft\Windows\Customer Experience Improvement Program\Consolidator)",
		LR"(\Microsoft\Windows\Customer Experience Improvement Program\UsbCeip)",
		LR"(\Microsoft\Windows\Customer Experience Improvement Program\KernelCeipTask)",
		LR"(\Microsoft\Windows\Customer Experience Improvement Program\BthSQM)",
		LR"(\Microsoft\Windows\DiskDiagnostic\Microsoft-Windows-DiskDiagnosticDataCollector)",
		LR"(\Microsoft\Windows\DiskDiagnostic\Microsoft-Windows-DiskDiagnosticResolver)",
		LR"(\Microsoft\Windows\DiskFootprint\Diagnostics)",
		LR"(\Microsoft\Windows\Maintenance\WinSAT)",
		LR"(\Microsoft\Windows\NetTrace\GatherNetworkInfo)",
		LR"(\Microsoft\Windows\PI\Sqm-Tasks)",
		LR"(\Microsoft\Windows\Power Efficiency Diagnostics\AnalyzeSystem)",
		LR"(\Microsoft\Windows\Windows Error Reporting\QueueReporting)",
		LR"(\Microsoft\Windows\CloudExperienceHost\CreateObjectTask)",
		LR"(\Microsoft\Windows\Device Information\Device)",
	};
}

namespace Modules
{
	PrivacyModule::PrivacyModule( )
	{
		refresh_all( );
	}

	Core::Result<bool> PrivacyModule::load_telemetry( )
	{
		return load_bundle( HKEY_LOCAL_MACHINE, telemetry_entries );
	}

	Core::Status PrivacyModule::apply_telemetry( const bool& enabled )
	{
		return apply_bundle( HKEY_LOCAL_MACHINE, telemetry_entries, enabled );
	}

	Core::Result<bool> PrivacyModule::load_advertising_id( )
	{
		return load_bundle( HKEY_CURRENT_USER, advertising_id_entries );
	}

	Core::Status PrivacyModule::apply_advertising_id( const bool& enabled )
	{
		return apply_bundle( HKEY_CURRENT_USER, advertising_id_entries, enabled );
	}

	Core::Result<bool> PrivacyModule::load_activity_feed( )
	{
		return load_bundle( HKEY_LOCAL_MACHINE, activity_feed_entries );
	}

	Core::Status PrivacyModule::apply_activity_feed( const bool& enabled )
	{
		return apply_bundle( HKEY_LOCAL_MACHINE, activity_feed_entries, enabled );
	}

	Core::Result<bool> PrivacyModule::load_tailored_experiences( )
	{
		return load_bundle( HKEY_CURRENT_USER, tailored_experiences_entries );
	}

	Core::Status PrivacyModule::apply_tailored_experiences( const bool& enabled )
	{
		return apply_bundle( HKEY_CURRENT_USER, tailored_experiences_entries, enabled );
	}

	Core::Result<bool> PrivacyModule::load_game_dvr( )
	{
		return load_bundle( HKEY_CURRENT_USER, game_dvr_hkcu_entries );
	}

	Core::Status PrivacyModule::apply_game_dvr( const bool& enabled )
	{
		if ( const auto status = apply_bundle( HKEY_CURRENT_USER, game_dvr_hkcu_entries, enabled ); !status )
			return status;

		return apply_bundle( HKEY_LOCAL_MACHINE, game_dvr_hklm_entries, enabled );
	}

	Core::Result<bool> PrivacyModule::load_content_suggestions( )
	{
		return load_bundle( HKEY_CURRENT_USER, content_suggestions_entries );
	}

	Core::Status PrivacyModule::apply_content_suggestions( const bool& enabled )
	{
		return apply_bundle( HKEY_CURRENT_USER, content_suggestions_entries, enabled );
	}

	Core::Result<bool> PrivacyModule::load_error_reporting( )
	{
		return load_bundle( HKEY_LOCAL_MACHINE, error_reporting_entries );
	}

	Core::Status PrivacyModule::apply_error_reporting( const bool& enabled )
	{
		return apply_bundle( HKEY_LOCAL_MACHINE, error_reporting_entries, enabled );
	}

	Core::Result<bool> PrivacyModule::load_diagnostic_execution( )
	{
		return load_bundle( HKEY_LOCAL_MACHINE, diagnostic_execution_entries );
	}

	Core::Status PrivacyModule::apply_diagnostic_execution( const bool& enabled )
	{
		return apply_bundle( HKEY_LOCAL_MACHINE, diagnostic_execution_entries, enabled );
	}

	Core::Result<bool> PrivacyModule::load_location_services( )
	{
		return load_bundle( HKEY_LOCAL_MACHINE, location_services_entries );
	}

	Core::Status PrivacyModule::apply_location_services( const bool& enabled )
	{
		return apply_bundle( HKEY_LOCAL_MACHINE, location_services_entries, enabled );
	}

	Core::Result<bool> PrivacyModule::load_feedback_prompts( )
	{
		return load_bundle( HKEY_LOCAL_MACHINE, feedback_prompts_entries );
	}

	Core::Status PrivacyModule::apply_feedback_prompts( const bool& enabled )
	{
		return apply_bundle( HKEY_LOCAL_MACHINE, feedback_prompts_entries, enabled );
	}

	Core::Result<bool> PrivacyModule::load_news_and_interests( )
	{
		return load_bundle( HKEY_LOCAL_MACHINE, news_and_interests_entries );
	}

	Core::Status PrivacyModule::apply_news_and_interests( const bool& enabled )
	{
		return apply_bundle( HKEY_LOCAL_MACHINE, news_and_interests_entries, enabled );
	}

	Core::Result<bool> PrivacyModule::load_windows_feeds( )
	{
		return load_bundle( HKEY_LOCAL_MACHINE, windows_feeds_entries );
	}

	Core::Status PrivacyModule::apply_windows_feeds( const bool& enabled )
	{
		return apply_bundle( HKEY_LOCAL_MACHINE, windows_feeds_entries, enabled );
	}

	Core::Result<bool> PrivacyModule::load_setting_sync( )
	{
		return load_bundle( HKEY_CURRENT_USER, setting_sync_entries );
	}

	Core::Status PrivacyModule::apply_setting_sync( const bool& enabled )
	{
		return apply_bundle( HKEY_CURRENT_USER, setting_sync_entries, enabled );
	}

	Core::Result<bool> PrivacyModule::load_diagnostic_tasks( )
	{
		const auto result = Utils::TaskScheduler::is_enabled( diagnostic_task_paths[ 0 ] );

		if ( !result )
			return std::unexpected( result.error( ) == Core::Error::NotFound ? Core::Error::Unsupported : result.error( ) );

		return *result;
	}

	Core::Status PrivacyModule::apply_diagnostic_tasks( const bool& enabled )
	{
		for ( const auto* path : diagnostic_task_paths )
		{
			if ( const auto status = Utils::TaskScheduler::set_enabled( path, enabled ); !status )
				return status;
		}

		return {};
	}
}

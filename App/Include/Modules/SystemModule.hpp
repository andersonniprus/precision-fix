#pragma once

#include "Modules/ModuleBase.hpp"

namespace Modules
{
	enum class SystemFeature : std::uint8_t
	{
		PrioritySeparation,
		Responsiveness,
		NetworkThrottling,
		Prefetch,
		PowerThrottling,
		TimerCoalescing,
		ProcessorIdleStates,
		Tsx,
		ForegroundLockTimeout,
		MenuShowDelay,
		UiAnimations,
		ShowFileExtensions,
		DarkMode,
		HibernateEnabled,
		FastStartup,
		SleepDiagnostics,
		EnergyEstimation,
		ModernStandby,
		SvcHostSplitThreshold,
		DisableLastAccessTimestamps,
		Disable8dot3NameCreation,
		RestorePointFrequency,
		ToastNotifications,
		FastAppTermination,
		GamesMmcssProfile,
		GameMode,
		SystemManagedPageFile,
		SearchIndexing,
		SysMain,
		BackgroundApps,
		DeliveryOptimization,
		Count,
	};

	class SystemModule final : public ModuleBase<SystemModule, SystemFeature>
	{
	public:
		SystemModule( );

	private:
		template<auto>
		friend struct FeatureTraits;

		[[nodiscard]] static Core::Result<std::uint32_t> load_priority_separation( );
		static Core::Status apply_priority_separation( const std::uint32_t& );

		[[nodiscard]] static Core::Result<std::uint32_t> load_responsiveness( );
		static Core::Status apply_responsiveness( const std::uint32_t& );

		[[nodiscard]] static Core::Result<std::uint32_t> load_network_throttling( );
		static Core::Status apply_network_throttling( const std::uint32_t& );

		[[nodiscard]] static Core::Result<bool> load_prefetch( );
		static Core::Status apply_prefetch( const bool& );

		[[nodiscard]] static Core::Result<bool> load_power_throttling( );
		static Core::Status apply_power_throttling( const bool& );

		[[nodiscard]] static Core::Result<bool> load_timer_coalescing( );
		static Core::Status apply_timer_coalescing( const bool& );

		[[nodiscard]] static Core::Result<bool> load_processor_idle_states( );
		static Core::Status apply_processor_idle_states( const bool& );

		[[nodiscard]] static Core::Result<bool> load_tsx( );
		static Core::Status apply_tsx( const bool& );

		[[nodiscard]] static Core::Result<std::uint32_t> load_foreground_lock_timeout( );
		static Core::Status apply_foreground_lock_timeout( const std::uint32_t& );

		[[nodiscard]] static Core::Result<std::uint32_t> load_menu_show_delay( );
		static Core::Status apply_menu_show_delay( const std::uint32_t& );

		[[nodiscard]] static Core::Result<bool> load_ui_animations( );
		static Core::Status apply_ui_animations( const bool& );

		[[nodiscard]] static Core::Result<bool> load_show_file_extensions( );
		static Core::Status apply_show_file_extensions( const bool& );

		[[nodiscard]] static Core::Result<bool> load_dark_mode( );
		static Core::Status apply_dark_mode( const bool& );

		[[nodiscard]] static Core::Result<bool> load_hibernate_enabled( );
		static Core::Status apply_hibernate_enabled( const bool& );

		[[nodiscard]] static Core::Result<bool> load_fast_startup( );
		static Core::Status apply_fast_startup( const bool& );

		[[nodiscard]] static Core::Result<bool> load_sleep_diagnostics( );
		static Core::Status apply_sleep_diagnostics( const bool& );

		[[nodiscard]] static Core::Result<bool> load_energy_estimation( );
		static Core::Status apply_energy_estimation( const bool& );

		[[nodiscard]] static Core::Result<bool> load_modern_standby( );
		static Core::Status apply_modern_standby( const bool& );

		[[nodiscard]] static Core::Result<std::uint32_t> load_svchost_split_threshold( );
		static Core::Status apply_svchost_split_threshold( const std::uint32_t& );

		[[nodiscard]] static Core::Result<std::uint32_t> load_disable_last_access_timestamps( );
		static Core::Status apply_disable_last_access_timestamps( const std::uint32_t& );

		[[nodiscard]] static Core::Result<std::uint32_t> load_disable_8dot3_name_creation( );
		static Core::Status apply_disable_8dot3_name_creation( const std::uint32_t& );

		[[nodiscard]] static Core::Result<std::uint32_t> load_restore_point_frequency( );
		static Core::Status apply_restore_point_frequency( const std::uint32_t& );

		[[nodiscard]] static Core::Result<bool> load_toast_notifications( );
		static Core::Status apply_toast_notifications( const bool& );

		[[nodiscard]] static Core::Result<bool> load_fast_app_termination( );
		static Core::Status apply_fast_app_termination( const bool& );

		[[nodiscard]] static Core::Result<bool> load_games_mmcss_profile( );
		static Core::Status apply_games_mmcss_profile( const bool& );

		[[nodiscard]] static Core::Result<bool> load_game_mode( );
		static Core::Status apply_game_mode( const bool& );

		[[nodiscard]] static Core::Result<bool> load_system_managed_page_file( );
		static Core::Status apply_system_managed_page_file( const bool& );

		[[nodiscard]] static Core::Result<bool> load_search_indexing( );
		static Core::Status apply_search_indexing( const bool& );

		[[nodiscard]] static Core::Result<bool> load_sys_main( );
		static Core::Status apply_sys_main( const bool& );

		[[nodiscard]] static Core::Result<bool> load_background_apps( );
		static Core::Status apply_background_apps( const bool& );

		[[nodiscard]] static Core::Result<bool> load_delivery_optimization( );
		static Core::Status apply_delivery_optimization( const bool& );
	};

	template<>
	struct FeatureTraits<SystemFeature::PrioritySeparation>
	{
		using value_type = std::uint32_t;

		static constexpr value_type fallback( ) noexcept
		{
			return 2;
		}

		static constexpr auto load  = &SystemModule::load_priority_separation;
		static constexpr auto apply = &SystemModule::apply_priority_separation;
	};

	template<>
	struct FeatureTraits<SystemFeature::Responsiveness>
	{
		using value_type = std::uint32_t;

		static constexpr value_type fallback( ) noexcept
		{
			return 20;
		}

		static constexpr auto load  = &SystemModule::load_responsiveness;
		static constexpr auto apply = &SystemModule::apply_responsiveness;
	};

	template<>
	struct FeatureTraits<SystemFeature::NetworkThrottling>
	{
		using value_type = std::uint32_t;

		static constexpr value_type fallback( ) noexcept
		{
			return 0xA;
		}

		static constexpr auto load  = &SystemModule::load_network_throttling;
		static constexpr auto apply = &SystemModule::apply_network_throttling;
	};

	template<>
	struct FeatureTraits<SystemFeature::Prefetch>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &SystemModule::load_prefetch;
		static constexpr auto apply = &SystemModule::apply_prefetch;
	};

	template<>
	struct FeatureTraits<SystemFeature::PowerThrottling>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &SystemModule::load_power_throttling;
		static constexpr auto apply = &SystemModule::apply_power_throttling;
	};

	template<>
	struct FeatureTraits<SystemFeature::TimerCoalescing>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &SystemModule::load_timer_coalescing;
		static constexpr auto apply = &SystemModule::apply_timer_coalescing;
	};

	template<>
	struct FeatureTraits<SystemFeature::ProcessorIdleStates>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &SystemModule::load_processor_idle_states;
		static constexpr auto apply = &SystemModule::apply_processor_idle_states;
	};

	template<>
	struct FeatureTraits<SystemFeature::Tsx>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &SystemModule::load_tsx;
		static constexpr auto apply = &SystemModule::apply_tsx;
	};

	template<>
	struct FeatureTraits<SystemFeature::ForegroundLockTimeout>
	{
		using value_type = std::uint32_t;

		static constexpr value_type fallback( ) noexcept
		{
			return 200000;
		}

		static constexpr auto load  = &SystemModule::load_foreground_lock_timeout;
		static constexpr auto apply = &SystemModule::apply_foreground_lock_timeout;
	};

	template<>
	struct FeatureTraits<SystemFeature::MenuShowDelay>
	{
		using value_type = std::uint32_t;

		static constexpr value_type fallback( ) noexcept
		{
			return 400;
		}

		static constexpr auto load  = &SystemModule::load_menu_show_delay;
		static constexpr auto apply = &SystemModule::apply_menu_show_delay;
	};

	template<>
	struct FeatureTraits<SystemFeature::UiAnimations>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &SystemModule::load_ui_animations;
		static constexpr auto apply = &SystemModule::apply_ui_animations;
	};

	template<>
	struct FeatureTraits<SystemFeature::ShowFileExtensions>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return false;
		}

		static constexpr auto load  = &SystemModule::load_show_file_extensions;
		static constexpr auto apply = &SystemModule::apply_show_file_extensions;
	};

	template<>
	struct FeatureTraits<SystemFeature::DarkMode>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return false;
		}

		static constexpr auto load  = &SystemModule::load_dark_mode;
		static constexpr auto apply = &SystemModule::apply_dark_mode;
	};

	template<>
	struct FeatureTraits<SystemFeature::HibernateEnabled>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &SystemModule::load_hibernate_enabled;
		static constexpr auto apply = &SystemModule::apply_hibernate_enabled;
	};

	template<>
	struct FeatureTraits<SystemFeature::FastStartup>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &SystemModule::load_fast_startup;
		static constexpr auto apply = &SystemModule::apply_fast_startup;
	};

	template<>
	struct FeatureTraits<SystemFeature::SleepDiagnostics>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &SystemModule::load_sleep_diagnostics;
		static constexpr auto apply = &SystemModule::apply_sleep_diagnostics;
	};

	template<>
	struct FeatureTraits<SystemFeature::EnergyEstimation>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &SystemModule::load_energy_estimation;
		static constexpr auto apply = &SystemModule::apply_energy_estimation;
	};

	template<>
	struct FeatureTraits<SystemFeature::ModernStandby>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &SystemModule::load_modern_standby;
		static constexpr auto apply = &SystemModule::apply_modern_standby;
	};

	template<>
	struct FeatureTraits<SystemFeature::SvcHostSplitThreshold>
	{
		using value_type = std::uint32_t;

		static constexpr value_type fallback( ) noexcept
		{
			return 4194304;
		}

		static constexpr auto load  = &SystemModule::load_svchost_split_threshold;
		static constexpr auto apply = &SystemModule::apply_svchost_split_threshold;
	};

	template<>
	struct FeatureTraits<SystemFeature::DisableLastAccessTimestamps>
	{
		using value_type = std::uint32_t;

		static constexpr value_type fallback( ) noexcept
		{
			return 2;
		}

		static constexpr auto load  = &SystemModule::load_disable_last_access_timestamps;
		static constexpr auto apply = &SystemModule::apply_disable_last_access_timestamps;
	};

	template<>
	struct FeatureTraits<SystemFeature::Disable8dot3NameCreation>
	{
		using value_type = std::uint32_t;

		static constexpr value_type fallback( ) noexcept
		{
			return 2;
		}

		static constexpr auto load  = &SystemModule::load_disable_8dot3_name_creation;
		static constexpr auto apply = &SystemModule::apply_disable_8dot3_name_creation;
	};

	template<>
	struct FeatureTraits<SystemFeature::RestorePointFrequency>
	{
		using value_type = std::uint32_t;

		static constexpr value_type fallback( ) noexcept
		{
			return 1440;
		}

		static constexpr auto load  = &SystemModule::load_restore_point_frequency;
		static constexpr auto apply = &SystemModule::apply_restore_point_frequency;
	};

	template<>
	struct FeatureTraits<SystemFeature::ToastNotifications>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &SystemModule::load_toast_notifications;
		static constexpr auto apply = &SystemModule::apply_toast_notifications;
	};

	template<>
	struct FeatureTraits<SystemFeature::FastAppTermination>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return false;
		}

		static constexpr auto load  = &SystemModule::load_fast_app_termination;
		static constexpr auto apply = &SystemModule::apply_fast_app_termination;
	};

	template<>
	struct FeatureTraits<SystemFeature::GamesMmcssProfile>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return false;
		}

		static constexpr auto load  = &SystemModule::load_games_mmcss_profile;
		static constexpr auto apply = &SystemModule::apply_games_mmcss_profile;
	};

	template<>
	struct FeatureTraits<SystemFeature::GameMode>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &SystemModule::load_game_mode;
		static constexpr auto apply = &SystemModule::apply_game_mode;
	};

	template<>
	struct FeatureTraits<SystemFeature::SystemManagedPageFile>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &SystemModule::load_system_managed_page_file;
		static constexpr auto apply = &SystemModule::apply_system_managed_page_file;
	};

	template<>
	struct FeatureTraits<SystemFeature::SearchIndexing>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &SystemModule::load_search_indexing;
		static constexpr auto apply = &SystemModule::apply_search_indexing;
	};

	template<>
	struct FeatureTraits<SystemFeature::SysMain>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &SystemModule::load_sys_main;
		static constexpr auto apply = &SystemModule::apply_sys_main;
	};

	template<>
	struct FeatureTraits<SystemFeature::BackgroundApps>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &SystemModule::load_background_apps;
		static constexpr auto apply = &SystemModule::apply_background_apps;
	};

	template<>
	struct FeatureTraits<SystemFeature::DeliveryOptimization>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &SystemModule::load_delivery_optimization;
		static constexpr auto apply = &SystemModule::apply_delivery_optimization;
	};
}

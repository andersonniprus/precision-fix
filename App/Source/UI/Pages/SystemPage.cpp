#include "Stdafx.hpp"
#include "UI/Pages/SystemPage.hpp"
#include "Logger.hpp"
#include "Intl.hpp"
#include "UI/Theme.hpp"
#include "UI/Fonts/IconsLucide.h"
#include "UI/Widgets/Section.hpp"
#include "UI/Widgets/SettingsRow.hpp"
#include "UI/Widgets/Switch.hpp"

namespace UI::Pages
{
	using namespace Widgets;
	using enum Modules::SystemFeature;
	using Modules::FeatureTraits;

	SystemPage::SystemPage(
		std::shared_ptr<Modules::SystemModule> module,
		std::shared_ptr<App::Logger> logger,
		std::shared_ptr<App::Intl> intl
	)
		: system_( std::move( module ) ),
		  logger_( std::move( logger ) ),
		  intl_( std::move( intl ) )
	{
	}

	void SystemPage::on_activate( )
	{
		system_->refresh_all( );
	}

	void SystemPage::set_status( const Core::Status& status )
	{
		if ( status )
		{
			logger_->info( "System", "setting applied" );
			status_.clear( );
			return;
		}

		logger_->error( "System", std::string { Core::to_string( status.error( ) ) } );
		status_ = std::string { Core::to_string( status.error( ) ) };
	}


	void SystemPage::render( )
	{
		const float switch_width   = ImGui::GetFrameHeight( ) * switch_aspect;
		const float switch_height  = ImGui::GetFrameHeight( );
		const float control_width  = preset_row_width( 2 );
		const float control_height = preset_button_height( );

		section( ICON_LC_SLIDERS_HORIZONTAL, intl_->tr( "Scheduling" ), intl_->tr( "CPU time and multimedia throttling." ), [ & ]
		{
			ImGui::PushID( "Win32 priority separation" );
			settings_row(
				intl_->tr( "Win32 priority separation" ),
				intl_->tr( "Splits CPU time between the foreground application and background tasks." ),
				control_width, control_height,
				Level::Medium, Level::Low,
				intl_->tr( "Optimized" ),
				[ & ]
				{
					if ( preset_button( intl_->tr( "Default" ), ButtonVariant::Ghost ) )
					{
						set_status( system_->set<PrioritySeparation>( FeatureTraits<PrioritySeparation>::fallback( ) ) );
					}

					preset_same_line( );

					if ( preset_button( intl_->tr( "Optimized" ), ButtonVariant::Primary ) )
					{
						set_status( system_->set<PrioritySeparation>( 0x26 ) );
					}
				}
			);
			ImGui::PopID( );

			ImGui::PushID( "System responsiveness" );
			settings_row(
				intl_->tr( "System responsiveness" ),
				intl_->tr( "Percentage of CPU reserved away from background multimedia tasks." ),
				control_width, control_height,
				Level::Medium, Level::Low,
				intl_->tr( "Optimized" ),
				[ & ]
				{
					if ( preset_button( intl_->tr( "Default" ), ButtonVariant::Ghost ) )
					{
						set_status( system_->set<Responsiveness>( FeatureTraits<Responsiveness>::fallback( ) ) );
					}

					preset_same_line( );

					if ( preset_button( intl_->tr( "Optimized" ), ButtonVariant::Primary ) )
					{
						set_status( system_->set<Responsiveness>( 10 ) );
					}
				}
			);
			ImGui::PopID( );

			ImGui::PushID( "Network throttling index" );
			settings_row(
				intl_->tr( "Network throttling index" ),
				intl_->tr( "Caps network throughput the OS reserves for background multimedia tasks." ),
				control_width, control_height,
				Level::Medium, Level::Low,
				intl_->tr( "Optimized" ),
				[ & ]
				{
					if ( preset_button( intl_->tr( "Default" ), ButtonVariant::Ghost ) )
					{
						set_status( system_->set<NetworkThrottling>( FeatureTraits<NetworkThrottling>::fallback( ) ) );
					}

					preset_same_line( );

					if ( preset_button( intl_->tr( "Optimized" ), ButtonVariant::Primary ) )
					{
						set_status( system_->set<NetworkThrottling>( 0xFFFFFFFF ) );
					}
				}
			);
			ImGui::PopID( );

			bool games_mmcss = system_->get<GamesMmcssProfile>( );

			settings_row(
				intl_->tr( "Games MMCSS profile" ),
				intl_->tr( "Raises GPU/CPU priority for the multimedia Games task (GPU Priority 8, Priority 6, High scheduling)." ),
				switch_width, switch_height,
				Level::Medium, Level::Low,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Games MMCSS profile" ), &games_mmcss ) )
					{
						set_status( system_->set<GamesMmcssProfile>( games_mmcss ) );
					}
				}
			);

			bool game_mode = system_->get<GameMode>( );

			settings_row(
				intl_->tr( "Game Mode" ),
				intl_->tr( "Lets Windows prioritize the foreground game and reduce background work while gaming." ),
				switch_width, switch_height,
				Level::Medium, Level::Low,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Game Mode" ), &game_mode ) )
					{
						set_status( system_->set<GameMode>( game_mode ) );
					}
				}
			);
		} );

		section( ICON_LC_POWER, intl_->tr( "Power" ), intl_->tr( "Sleep, standby and energy policies." ), [ & ]
		{
			bool power_throttling = system_->get<PowerThrottling>( );

			settings_row(
				intl_->tr( "Power throttling" ),
				intl_->tr( "Lets Windows downclock the CPU to save energy under light load." ),
				switch_width, switch_height,
				Level::Medium, Level::Medium,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Power throttling" ), &power_throttling ) )
					{
						set_status( system_->set<PowerThrottling>( power_throttling ) );
					}
				}
			);

			bool timer_coalescing = system_->get<TimerCoalescing>( );

			settings_row(
				intl_->tr( "Timer coalescing" ),
				intl_->tr( "Groups timer interrupts together to reduce CPU wake-ups." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "Off" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Timer coalescing" ), &timer_coalescing ) )
					{
						set_status( system_->set<TimerCoalescing>( timer_coalescing ) );
					}
				}
			);

			bool idle_states = system_->get<ProcessorIdleStates>( );

			settings_row(
				intl_->tr( "Processor idle states" ),
				intl_->tr( "Allows the CPU to enter low-power idle states (C-states) when unused." ),
				switch_width, switch_height,
				Level::Medium, Level::High,
				intl_->tr( "Off" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Processor idle states" ), &idle_states ) )
					{
						set_status( system_->set<ProcessorIdleStates>( idle_states ) );
					}
				}
			);

			bool hibernate = system_->get<HibernateEnabled>( );

			settings_row(
				intl_->tr( "Hibernate" ),
				intl_->tr( "Saves the current session to disk and powers off completely." ),
				switch_width, switch_height,
				Level::Low, Level::Medium,
				intl_->tr( "Off" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Hibernate" ), &hibernate ) )
					{
						set_status( system_->set<HibernateEnabled>( hibernate ) );
					}
				}
			);

			bool fast_startup = system_->get<FastStartup>( );

			settings_row(
				intl_->tr( "Fast startup" ),
				intl_->tr( "Hibernates the kernel session on shutdown to speed up the next boot." ),
				switch_width, switch_height,
				Level::Medium, Level::Medium,
				intl_->tr( "Off" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Fast startup" ), &fast_startup ) )
					{
						set_status( system_->set<FastStartup>( fast_startup ) );
					}
				}
			);

			bool sleep_diagnostics = system_->get<SleepDiagnostics>( );

			settings_row(
				intl_->tr( "Sleep diagnostics" ),
				intl_->tr( "Background study of sleep-wake behavior used for diagnostics telemetry." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "Off" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Sleep diagnostics" ), &sleep_diagnostics ) )
					{
						set_status( system_->set<SleepDiagnostics>( sleep_diagnostics ) );
					}
				}
			);

			bool energy_estimation = system_->get<EnergyEstimation>( );

			settings_row(
				intl_->tr( "Energy estimation engine" ),
				intl_->tr( "Background service that estimates per-app battery usage." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "Off" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Energy estimation engine" ), &energy_estimation ) )
					{
						set_status( system_->set<EnergyEstimation>( energy_estimation ) );
					}
				}
			);

			bool modern_standby = system_->get<ModernStandby>( );

			settings_row(
				intl_->tr( "Modern Standby" ),
				intl_->tr( "Low-power connected standby (S0). Off forces classic S3 sleep on supported hardware." ),
				switch_width, switch_height,
				Level::Medium, Level::High,
				intl_->tr( "Off" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Modern Standby" ), &modern_standby ) )
					{
						set_status( system_->set<ModernStandby>( modern_standby ) );
					}
				}
			);
		} );

		section( ICON_LC_ZAP, intl_->tr( "Responsiveness" ), intl_->tr( "Focus, menus and UI latency." ), [ & ]
		{
			ImGui::PushID( "Foreground lock timeout" );
			settings_row(
				intl_->tr( "Foreground lock timeout" ),
				intl_->tr( "Delay before a background app is allowed to steal focus from you." ),
				control_width, control_height,
				Level::Low, Level::Low,
				intl_->tr( "Optimized" ),
				[ & ]
				{
					if ( preset_button( intl_->tr( "Default" ), ButtonVariant::Ghost ) )
					{
						set_status( system_->set<ForegroundLockTimeout>( FeatureTraits<ForegroundLockTimeout>::fallback( ) ) );
					}

					preset_same_line( );

					if ( preset_button( intl_->tr( "Optimized" ), ButtonVariant::Primary ) )
					{
						set_status( system_->set<ForegroundLockTimeout>( 0 ) );
					}
				}
			);
			ImGui::PopID( );

			ImGui::PushID( "Menu show delay" );
			settings_row(
				intl_->tr( "Menu show delay" ),
				intl_->tr( "Delay before hovering a menu item opens its submenu." ),
				control_width, control_height,
				Level::Low, Level::Low,
				intl_->tr( "Optimized" ),
				[ & ]
				{
					if ( preset_button( intl_->tr( "Default" ), ButtonVariant::Ghost ) )
					{
						set_status( system_->set<MenuShowDelay>( FeatureTraits<MenuShowDelay>::fallback( ) ) );
					}

					preset_same_line( );

					if ( preset_button( intl_->tr( "Optimized" ), ButtonVariant::Primary ) )
					{
						set_status( system_->set<MenuShowDelay>( 0 ) );
					}
				}
			);
			ImGui::PopID( );

			bool animations = system_->get<UiAnimations>( );

			settings_row(
				intl_->tr( "UI animations" ),
				intl_->tr( "Menu, tooltip and cursor shadow animations. Off trims perceived click latency." ),
				switch_width, switch_height,
				Level::Medium, Level::Low,
				intl_->tr( "Off" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "UI animations" ), &animations ) )
					{
						set_status( system_->set<UiAnimations>( animations ) );
					}
				}
			);
		} );

		section( ICON_LC_WRENCH, intl_->tr( "Advanced" ), intl_->tr( "Prefetch and CPU extensions." ), [ & ]
		{
			bool prefetch = system_->get<Prefetch>( );

			settings_row(
				intl_->tr( "Prefetch" ),
				intl_->tr( "Preloads frequently used applications into memory ahead of time." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "Off" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Prefetch" ), &prefetch ) )
					{
						set_status( system_->set<Prefetch>( prefetch ) );
					}
				}
			);

			bool tsx = system_->get<Tsx>( );

			settings_row(
				intl_->tr( "Intel TSX" ),
				intl_->tr( "CPU transactional memory extension. Recommended on for Intel, off for AMD." ),
				switch_width, switch_height,
				Level::Low, Level::Medium,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Intel TSX" ), &tsx ) )
					{
						set_status( system_->set<Tsx>( tsx ) );
					}
				}
			);
		} );

		section( ICON_LC_APP_WINDOW, intl_->tr( "Explorer" ), intl_->tr( "Shell appearance and notifications." ), [ & ]
		{
			bool show_extensions = system_->get<ShowFileExtensions>( );

			settings_row(
				intl_->tr( "Show file extensions" ),
				intl_->tr( "Shows the file name extension for every file in Explorer." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Show file extensions" ), &show_extensions ) )
					{
						set_status( system_->set<ShowFileExtensions>( show_extensions ) );
					}
				}
			);

			bool dark_mode = system_->get<DarkMode>( );

			settings_row(
				intl_->tr( "Dark mode" ),
				intl_->tr( "Switches Windows apps to the dark color theme." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Dark mode" ), &dark_mode ) )
					{
						set_status( system_->set<DarkMode>( dark_mode ) );
					}
				}
			);

			bool toast_notifications = system_->get<ToastNotifications>( );

			settings_row(
				intl_->tr( "Toast notifications" ),
				intl_->tr( "Allows apps to show notification banners and the notification center badge." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "Off" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Toast notifications" ), &toast_notifications ) )
					{
						set_status( system_->set<ToastNotifications>( toast_notifications ) );
					}
				}
			);

			bool fast_app_termination = system_->get<FastAppTermination>( );

			settings_row(
				intl_->tr( "Fast app termination" ),
				intl_->tr( "Shortens the delay before Windows force-closes a hung or hooked application." ),
				switch_width, switch_height,
				Level::Medium, Level::Medium,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Fast app termination" ), &fast_app_termination ) )
					{
						set_status( system_->set<FastAppTermination>( fast_app_termination ) );
					}
				}
			);
		} );

		section( ICON_LC_HARD_DRIVE, intl_->tr( "Storage" ), intl_->tr( "NTFS and service host layout." ), [ & ]
		{
			ImGui::PushID( "svchost.exe split threshold" );
			settings_row(
				intl_->tr( "svchost.exe split threshold" ),
				intl_->tr( "RAM, in KB, above which Windows splits shared services into separate svchost.exe processes." ),
				control_width, control_height,
				Level::Low, Level::Low,
				"Recommended",
				[ & ]
				{
					if ( preset_button( intl_->tr( "Default" ), ButtonVariant::Ghost ) )
					{
						set_status( system_->set<SvcHostSplitThreshold>( FeatureTraits<SvcHostSplitThreshold>::fallback( ) ) );
					}

					preset_same_line( );

					if ( preset_button( intl_->tr( "Recommended" ), ButtonVariant::Primary ) )
					{
						MEMORYSTATUSEX memory_status { .dwLength = sizeof( memory_status ) };

						if ( GlobalMemoryStatusEx( &memory_status ) )
						{
							set_status( system_->set<SvcHostSplitThreshold>(
								static_cast<std::uint32_t>( memory_status.ullTotalPhys / 1024 ) ) );
						}
						else
						{
							set_status( std::unexpected( Core::Error::Unknown ) );
						}
					}
				}
			);
			ImGui::PopID( );

			ImGui::PushID( "Disable NTFS last-access timestamps" );
			settings_row(
				intl_->tr( "Disable NTFS last-access timestamps" ),
				intl_->tr( "Stops updating a file's last-accessed time on every read." ),
				control_width, control_height,
				Level::Low, Level::Low,
				intl_->tr( "Optimized" ),
				[ & ]
				{
					if ( preset_button( intl_->tr( "Default" ), ButtonVariant::Ghost ) )
					{
						set_status( system_->set<DisableLastAccessTimestamps>( FeatureTraits<DisableLastAccessTimestamps>::fallback( ) ) );
					}

					preset_same_line( );

					if ( preset_button( intl_->tr( "Optimized" ), ButtonVariant::Primary ) )
					{
						set_status( system_->set<DisableLastAccessTimestamps>( 3 ) );
					}
				}
			);
			ImGui::PopID( );

			ImGui::PushID( "Disable 8.3 short filenames" );
			settings_row(
				intl_->tr( "Disable 8.3 short filenames" ),
				intl_->tr( "Stops NTFS from generating legacy DOS-compatible short file names." ),
				control_width, control_height,
				Level::Low, Level::Medium,
				intl_->tr( "Optimized" ),
				[ & ]
				{
					if ( preset_button( intl_->tr( "Default" ), ButtonVariant::Ghost ) )
					{
						set_status( system_->set<Disable8dot3NameCreation>( FeatureTraits<Disable8dot3NameCreation>::fallback( ) ) );
					}

					preset_same_line( );

					if ( preset_button( intl_->tr( "Optimized" ), ButtonVariant::Primary ) )
					{
						set_status( system_->set<Disable8dot3NameCreation>( 1 ) );
					}
				}
			);
			ImGui::PopID( );

			ImGui::PushID( "Restore point frequency" );
			settings_row(
				intl_->tr( "Restore point frequency" ),
				intl_->tr( "Minimum minutes between automatic System Restore checkpoints. Optimized creates one on every change." ),
				control_width, control_height,
				Level::Low, Level::Low,
				intl_->tr( "Optimized" ),
				[ & ]
				{
					if ( preset_button( intl_->tr( "Default" ), ButtonVariant::Ghost ) )
					{
						set_status( system_->set<RestorePointFrequency>( FeatureTraits<RestorePointFrequency>::fallback( ) ) );
					}

					preset_same_line( );

					if ( preset_button( intl_->tr( "Optimized" ), ButtonVariant::Primary ) )
					{
						set_status( system_->set<RestorePointFrequency>( 0 ) );
					}
				}
			);
			ImGui::PopID( );

			bool page_file = system_->get<SystemManagedPageFile>( );

			settings_row(
				intl_->tr( "System-managed page file" ),
				intl_->tr( "Lets Windows size the page file automatically. Never disable the page file; On restores the safe default." ),
				switch_width, switch_height,
				Level::Low, Level::High,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "System-managed page file" ), &page_file ) )
					{
						set_status( system_->set<SystemManagedPageFile>( page_file ) );
					}
				}
			);
		} );

		section( ICON_LC_ACTIVITY, intl_->tr( "Services & background" ),
		         intl_->tr( "Background work that competes with games and interactive apps." ), [ & ]
		         {
			         bool search_indexing = system_->get<SearchIndexing>( );

			         settings_row(
				         intl_->tr( "Windows Search indexing" ),
				         intl_->tr( "Background file indexing service (WSearch). Off frees disk and CPU on desktop gaming PCs." ),
				         switch_width, switch_height,
				         Level::Medium, Level::Medium,
				         intl_->tr( "Off" ),
				         [ & ]
				         {
					         if ( toggle_switch( intl_->tr( "Windows Search indexing" ), &search_indexing ) )
					         {
						         set_status( system_->set<SearchIndexing>( search_indexing ) );
					         }
				         }
			         );

			         bool sys_main = system_->get<SysMain>( );

			         settings_row(
				         intl_->tr( "SysMain (Superfetch)" ),
				         intl_->tr( "Prefetches apps into RAM in the background. Often less useful on fast SSDs." ),
				         switch_width, switch_height,
				         Level::Low, Level::Medium,
				         intl_->tr( "Off" ),
				         [ & ]
				         {
					         if ( toggle_switch( intl_->tr( "SysMain (Superfetch)" ), &sys_main ) )
					         {
						         set_status( system_->set<SysMain>( sys_main ) );
					         }
				         }
			         );

			         bool background_apps = system_->get<BackgroundApps>( );

			         settings_row(
				         intl_->tr( "Background apps" ),
				         intl_->tr( "Allows UWP/Store apps to run and refresh in the background." ),
				         switch_width, switch_height,
				         Level::Medium, Level::Low,
				         intl_->tr( "Off" ),
				         [ & ]
				         {
					         if ( toggle_switch( intl_->tr( "Background apps" ), &background_apps ) )
					         {
						         set_status( system_->set<BackgroundApps>( background_apps ) );
					         }
				         }
			         );

			         bool delivery_opt = system_->get<DeliveryOptimization>( );

			         settings_row(
				         intl_->tr( "Delivery Optimization" ),
				         intl_->tr( "Peer-to-peer Windows Update / Store downloads. Off limits sharing and background bandwidth use." ),
				         switch_width, switch_height,
				         Level::Low, Level::Low,
				         intl_->tr( "Off" ),
				         [ & ]
				         {
					         if ( toggle_switch( intl_->tr( "Delivery Optimization" ), &delivery_opt ) )
					         {
						         set_status( system_->set<DeliveryOptimization>( delivery_opt ) );
					         }
				         }
			         );
		         } );

		if ( !status_.empty( ) )
		{
			ImGui::TextColored( Theme::Danger, "%s", status_.c_str( ) );
		}
	}
}

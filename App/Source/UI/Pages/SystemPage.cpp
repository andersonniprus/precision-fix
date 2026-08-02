#include "Stdafx.hpp"
#include "UI/Pages/SystemPage.hpp"
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

	SystemPage::SystemPage( std::shared_ptr<Modules::SystemModule> system )
		: system_( std::move( system ) )
	{
	}

	void SystemPage::on_activate( )
	{
		system_->refresh_all( );
	}

	void SystemPage::set_status( const Core::Status& status )
	{
		status_ = status ? std::string {} : std::string { Core::to_string( status.error( ) ) };
	}

	void SystemPage::render( )
	{
		const float switch_width   = ImGui::GetFrameHeight( ) * switch_aspect;
		const float switch_height  = ImGui::GetFrameHeight( );
		const float control_width  = preset_row_width( 2 );
		const float control_height = preset_button_height( );

		section( ICON_LC_SLIDERS_HORIZONTAL, "Scheduling", "CPU time and multimedia throttling.", [ & ]
		{
			ImGui::PushID( "Win32 priority separation" );
			settings_row(
				"Win32 priority separation",
				"Splits CPU time between the foreground application and background tasks.",
				control_width, control_height,
				Level::Medium, Level::Low,
				"Optimized",
				[ & ]
				{
					if ( preset_button( "Default", ButtonVariant::Ghost ) )
					{
						set_status( system_->set<PrioritySeparation>( FeatureTraits<PrioritySeparation>::fallback( ) ) );
					}

					preset_same_line( );

					if ( preset_button( "Optimized", ButtonVariant::Primary ) )
					{
						set_status( system_->set<PrioritySeparation>( 0x26 ) );
					}
				}
			);
			ImGui::PopID( );

			ImGui::PushID( "System responsiveness" );
			settings_row(
				"System responsiveness",
				"Percentage of CPU reserved away from background multimedia tasks.",
				control_width, control_height,
				Level::Medium, Level::Low,
				"Optimized",
				[ & ]
				{
					if ( preset_button( "Default", ButtonVariant::Ghost ) )
					{
						set_status( system_->set<Responsiveness>( FeatureTraits<Responsiveness>::fallback( ) ) );
					}

					preset_same_line( );

					if ( preset_button( "Optimized", ButtonVariant::Primary ) )
					{
						set_status( system_->set<Responsiveness>( 10 ) );
					}
				}
			);
			ImGui::PopID( );

			ImGui::PushID( "Network throttling index" );
			settings_row(
				"Network throttling index",
				"Caps network throughput the OS reserves for background multimedia tasks.",
				control_width, control_height,
				Level::Medium, Level::Low,
				"Optimized",
				[ & ]
				{
					if ( preset_button( "Default", ButtonVariant::Ghost ) )
					{
						set_status( system_->set<NetworkThrottling>( FeatureTraits<NetworkThrottling>::fallback( ) ) );
					}

					preset_same_line( );

					if ( preset_button( "Optimized", ButtonVariant::Primary ) )
					{
						set_status( system_->set<NetworkThrottling>( 0xFFFFFFFF ) );
					}
				}
			);
			ImGui::PopID( );
		} );

		section( ICON_LC_POWER, "Power", "Sleep, standby and energy policies.", [ & ]
		{
			bool power_throttling = system_->get<PowerThrottling>( );

			settings_row(
				"Power throttling",
				"Lets Windows downclock the CPU to save energy under light load.",
				switch_width, switch_height,
				Level::Medium, Level::Medium,
				"Off",
				[ & ]
				{
					if ( toggle_switch( "Power throttling", &power_throttling ) )
					{
						set_status( system_->set<PowerThrottling>( power_throttling ) );
					}
				}
			);

			bool timer_coalescing = system_->get<TimerCoalescing>( );

			settings_row(
				"Timer coalescing",
				"Groups timer interrupts together to reduce CPU wake-ups.",
				switch_width, switch_height,
				Level::Low, Level::Low,
				"Off",
				[ & ]
				{
					if ( toggle_switch( "Timer coalescing", &timer_coalescing ) )
					{
						set_status( system_->set<TimerCoalescing>( timer_coalescing ) );
					}
				}
			);

			bool idle_states = system_->get<ProcessorIdleStates>( );

			settings_row(
				"Processor idle states",
				"Allows the CPU to enter low-power idle states (C-states) when unused.",
				switch_width, switch_height,
				Level::Medium, Level::High,
				"Off",
				[ & ]
				{
					if ( toggle_switch( "Processor idle states", &idle_states ) )
					{
						set_status( system_->set<ProcessorIdleStates>( idle_states ) );
					}
				}
			);

			bool hibernate = system_->get<HibernateEnabled>( );

			settings_row(
				"Hibernate",
				"Saves the current session to disk and powers off completely.",
				switch_width, switch_height,
				Level::Low, Level::Medium,
				"Off",
				[ & ]
				{
					if ( toggle_switch( "Hibernate", &hibernate ) )
					{
						set_status( system_->set<HibernateEnabled>( hibernate ) );
					}
				}
			);

			bool fast_startup = system_->get<FastStartup>( );

			settings_row(
				"Fast startup",
				"Hibernates the kernel session on shutdown to speed up the next boot.",
				switch_width, switch_height,
				Level::Medium, Level::Medium,
				"Off",
				[ & ]
				{
					if ( toggle_switch( "Fast startup", &fast_startup ) )
					{
						set_status( system_->set<FastStartup>( fast_startup ) );
					}
				}
			);

			bool sleep_diagnostics = system_->get<SleepDiagnostics>( );

			settings_row(
				"Sleep diagnostics",
				"Background study of sleep-wake behavior used for diagnostics telemetry.",
				switch_width, switch_height,
				Level::Low, Level::Low,
				"Off",
				[ & ]
				{
					if ( toggle_switch( "Sleep diagnostics", &sleep_diagnostics ) )
					{
						set_status( system_->set<SleepDiagnostics>( sleep_diagnostics ) );
					}
				}
			);

			bool energy_estimation = system_->get<EnergyEstimation>( );

			settings_row(
				"Energy estimation engine",
				"Background service that estimates per-app battery usage.",
				switch_width, switch_height,
				Level::Low, Level::Low,
				"Off",
				[ & ]
				{
					if ( toggle_switch( "Energy estimation engine", &energy_estimation ) )
					{
						set_status( system_->set<EnergyEstimation>( energy_estimation ) );
					}
				}
			);

			bool modern_standby = system_->get<ModernStandby>( );

			settings_row(
				"Modern Standby",
				"Low-power connected standby (S0). Off forces classic S3 sleep on supported hardware.",
				switch_width, switch_height,
				Level::Medium, Level::High,
				"Off",
				[ & ]
				{
					if ( toggle_switch( "Modern Standby", &modern_standby ) )
					{
						set_status( system_->set<ModernStandby>( modern_standby ) );
					}
				}
			);
		} );

		section( ICON_LC_ZAP, "Responsiveness", "Focus, menus and UI latency.", [ & ]
		{
			ImGui::PushID( "Foreground lock timeout" );
			settings_row(
				"Foreground lock timeout",
				"Delay before a background app is allowed to steal focus from you.",
				control_width, control_height,
				Level::Low, Level::Low,
				"Optimized",
				[ & ]
				{
					if ( preset_button( "Default", ButtonVariant::Ghost ) )
					{
						set_status( system_->set<ForegroundLockTimeout>( FeatureTraits<ForegroundLockTimeout>::fallback( ) ) );
					}

					preset_same_line( );

					if ( preset_button( "Optimized", ButtonVariant::Primary ) )
					{
						set_status( system_->set<ForegroundLockTimeout>( 0 ) );
					}
				}
			);
			ImGui::PopID( );

			ImGui::PushID( "Menu show delay" );
			settings_row(
				"Menu show delay",
				"Delay before hovering a menu item opens its submenu.",
				control_width, control_height,
				Level::Low, Level::Low,
				"Optimized",
				[ & ]
				{
					if ( preset_button( "Default", ButtonVariant::Ghost ) )
					{
						set_status( system_->set<MenuShowDelay>( FeatureTraits<MenuShowDelay>::fallback( ) ) );
					}

					preset_same_line( );

					if ( preset_button( "Optimized", ButtonVariant::Primary ) )
					{
						set_status( system_->set<MenuShowDelay>( 0 ) );
					}
				}
			);
			ImGui::PopID( );

			bool animations = system_->get<UiAnimations>( );

			settings_row(
				"UI animations",
				"Menu, tooltip and cursor shadow animations. Off trims perceived click latency.",
				switch_width, switch_height,
				Level::Medium, Level::Low,
				"Off",
				[ & ]
				{
					if ( toggle_switch( "UI animations", &animations ) )
					{
						set_status( system_->set<UiAnimations>( animations ) );
					}
				}
			);
		} );

		section( ICON_LC_WRENCH, "Advanced", "Prefetch and CPU extensions.", [ & ]
		{
			bool prefetch = system_->get<Prefetch>( );

			settings_row(
				"Prefetch",
				"Preloads frequently used applications into memory ahead of time.",
				switch_width, switch_height,
				Level::Low, Level::Low,
				"Off",
				[ & ]
				{
					if ( toggle_switch( "Prefetch", &prefetch ) )
					{
						set_status( system_->set<Prefetch>( prefetch ) );
					}
				}
			);

			bool tsx = system_->get<Tsx>( );

			settings_row(
				"Intel TSX",
				"CPU transactional memory extension. Recommended on for Intel, off for AMD.",
				switch_width, switch_height,
				Level::Low, Level::Medium,
				"On",
				[ & ]
				{
					if ( toggle_switch( "Intel TSX", &tsx ) )
					{
						set_status( system_->set<Tsx>( tsx ) );
					}
				}
			);
		} );

		section( ICON_LC_APP_WINDOW, "Explorer", "Shell appearance and notifications.", [ & ]
		{
			bool show_extensions = system_->get<ShowFileExtensions>( );

			settings_row(
				"Show file extensions",
				"Shows the file name extension for every file in Explorer.",
				switch_width, switch_height,
				Level::Low, Level::Low,
				"On",
				[ & ]
				{
					if ( toggle_switch( "Show file extensions", &show_extensions ) )
					{
						set_status( system_->set<ShowFileExtensions>( show_extensions ) );
					}
				}
			);

			bool dark_mode = system_->get<DarkMode>( );

			settings_row(
				"Dark mode",
				"Switches Windows apps to the dark color theme.",
				switch_width, switch_height,
				Level::Low, Level::Low,
				"On",
				[ & ]
				{
					if ( toggle_switch( "Dark mode", &dark_mode ) )
					{
						set_status( system_->set<DarkMode>( dark_mode ) );
					}
				}
			);

			bool toast_notifications = system_->get<ToastNotifications>( );

			settings_row(
				"Toast notifications",
				"Allows apps to show notification banners and the notification center badge.",
				switch_width, switch_height,
				Level::Low, Level::Low,
				"Off",
				[ & ]
				{
					if ( toggle_switch( "Toast notifications", &toast_notifications ) )
					{
						set_status( system_->set<ToastNotifications>( toast_notifications ) );
					}
				}
			);

			bool fast_app_termination = system_->get<FastAppTermination>( );

			settings_row(
				"Fast app termination",
				"Shortens the delay before Windows force-closes a hung or hooked application.",
				switch_width, switch_height,
				Level::Medium, Level::Medium,
				"On",
				[ & ]
				{
					if ( toggle_switch( "Fast app termination", &fast_app_termination ) )
					{
						set_status( system_->set<FastAppTermination>( fast_app_termination ) );
					}
				}
			);
		} );

		section( ICON_LC_HARD_DRIVE, "Storage", "NTFS and service host layout.", [ & ]
		{
			ImGui::PushID( "svchost.exe split threshold" );
			settings_row(
				"svchost.exe split threshold",
				"RAM, in KB, above which Windows splits shared services into separate svchost.exe processes.",
				control_width, control_height,
				Level::Low, Level::Low,
				"Recommended",
				[ & ]
				{
					if ( preset_button( "Default", ButtonVariant::Ghost ) )
					{
						set_status( system_->set<SvcHostSplitThreshold>( FeatureTraits<SvcHostSplitThreshold>::fallback( ) ) );
					}

					preset_same_line( );

					if ( preset_button( "Recommended", ButtonVariant::Primary ) )
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
				"Disable NTFS last-access timestamps",
				"Stops updating a file's last-accessed time on every read.",
				control_width, control_height,
				Level::Low, Level::Low,
				"Optimized",
				[ & ]
				{
					if ( preset_button( "Default", ButtonVariant::Ghost ) )
					{
						set_status( system_->set<DisableLastAccessTimestamps>( FeatureTraits<DisableLastAccessTimestamps>::fallback( ) ) );
					}

					preset_same_line( );

					if ( preset_button( "Optimized", ButtonVariant::Primary ) )
					{
						set_status( system_->set<DisableLastAccessTimestamps>( 3 ) );
					}
				}
			);
			ImGui::PopID( );

			ImGui::PushID( "Disable 8.3 short filenames" );
			settings_row(
				"Disable 8.3 short filenames",
				"Stops NTFS from generating legacy DOS-compatible short file names.",
				control_width, control_height,
				Level::Low, Level::Medium,
				"Optimized",
				[ & ]
				{
					if ( preset_button( "Default", ButtonVariant::Ghost ) )
					{
						set_status( system_->set<Disable8dot3NameCreation>( FeatureTraits<Disable8dot3NameCreation>::fallback( ) ) );
					}

					preset_same_line( );

					if ( preset_button( "Optimized", ButtonVariant::Primary ) )
					{
						set_status( system_->set<Disable8dot3NameCreation>( 1 ) );
					}
				}
			);
			ImGui::PopID( );

			ImGui::PushID( "Restore point frequency" );
			settings_row(
				"Restore point frequency",
				"Minimum minutes between automatic System Restore checkpoints. Optimized creates one on every change.",
				control_width, control_height,
				Level::Low, Level::Low,
				"Optimized",
				[ & ]
				{
					if ( preset_button( "Default", ButtonVariant::Ghost ) )
					{
						set_status( system_->set<RestorePointFrequency>( FeatureTraits<RestorePointFrequency>::fallback( ) ) );
					}

					preset_same_line( );

					if ( preset_button( "Optimized", ButtonVariant::Primary ) )
					{
						set_status( system_->set<RestorePointFrequency>( 0 ) );
					}
				}
			);
			ImGui::PopID( );
		} );

		if ( !status_.empty( ) )
		{
			ImGui::TextColored( Theme::Danger, "%s", status_.c_str( ) );
		}
	}
}

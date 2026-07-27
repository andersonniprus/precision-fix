#include "Stdafx.hpp"
#include "UI/Pages/SystemPage.hpp"
#include "UI/Widgets/SettingsRow.hpp"
#include "UI/Widgets/Switch.hpp"

namespace UI::Pages
{
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

	template<Modules::SystemFeature F>
	void SystemPage::render_preset_row( const char* title, const char* description, const std::uint32_t optimized, const Widgets::Level gain,
	                                    const Widgets::Level impact )
	{
		ImGui::PushID( title );

		Widgets::settings_row( title, description, Widgets::preset_row_width( 2 ), ImGui::GetFrameHeight( ), gain, impact, [ & ]
		{
			if ( ImGui::Button( "Default", ImVec2( Widgets::preset_button_width, 0.f ) ) )
				set_status( system_->set<F>( Modules::FeatureTraits<F>::fallback( ) ) );

			ImGui::SameLine( );

			if ( ImGui::Button( "Optimized", ImVec2( Widgets::preset_button_width, 0.f ) ) )
				set_status( system_->set<F>( optimized ) );
		} );

		ImGui::PopID( );
	}

	void SystemPage::render_scheduling( )
	{
		render_preset_row<Modules::SystemFeature::PrioritySeparation>(
			"Win32 priority separation", "Splits CPU time between the foreground application and background tasks.", 0x26, Widgets::Level::Medium,
			Widgets::Level::Low
		);

		ImGui::Separator( );

		render_preset_row<Modules::SystemFeature::Responsiveness>(
			"System responsiveness", "Percentage of CPU reserved away from background multimedia tasks.", 10, Widgets::Level::Medium,
			Widgets::Level::Low
		);

		ImGui::Separator( );

		render_preset_row<Modules::SystemFeature::NetworkThrottling>(
			"Network throttling index", "Caps network throughput the OS reserves for background multimedia tasks.", 0xFFFFFFFF,
			Widgets::Level::Medium, Widgets::Level::Low
		);
	}

	void SystemPage::render_power( )
	{
		const float switch_width  = ImGui::GetFrameHeight( ) * Widgets::switch_aspect;
		const float switch_height = ImGui::GetFrameHeight( );

		bool power_throttling = system_->get<Modules::SystemFeature::PowerThrottling>( );

		Widgets::settings_row(
			"Power throttling",
			"Lets Windows downclock the CPU to save energy under light load.",
			switch_width,
			switch_height,
			Widgets::Level::Medium,
			Widgets::Level::Medium,
			[ & ]
			{
				if ( Widgets::toggle_switch( "Power throttling", &power_throttling ) )
					set_status( system_->set<Modules::SystemFeature::PowerThrottling>( power_throttling ) );
			}
		);

		ImGui::Separator( );

		bool timer_coalescing = system_->get<Modules::SystemFeature::TimerCoalescing>( );

		Widgets::settings_row(
			"Timer coalescing",
			"Groups timer interrupts together to reduce CPU wake-ups.",
			switch_width,
			switch_height,
			Widgets::Level::Low,
			Widgets::Level::Low,
			[ & ]
			{
				if ( Widgets::toggle_switch( "Timer coalescing", &timer_coalescing ) )
					set_status( system_->set<Modules::SystemFeature::TimerCoalescing>( timer_coalescing ) );
			}
		);

		ImGui::Separator( );

		bool idle_states = system_->get<Modules::SystemFeature::ProcessorIdleStates>( );

		Widgets::settings_row(
			"Processor idle states",
			"Allows the CPU to enter low-power idle states (C-states) when unused.",
			switch_width,
			switch_height,
			Widgets::Level::Medium,
			Widgets::Level::High,
			[ & ]
			{
				if ( Widgets::toggle_switch( "Processor idle states", &idle_states ) )
					set_status( system_->set<Modules::SystemFeature::ProcessorIdleStates>( idle_states ) );
			}
		);

		ImGui::Separator( );

		bool hibernate = system_->get<Modules::SystemFeature::HibernateEnabled>( );

		Widgets::settings_row(
			"Hibernate",
			"Saves the current session to disk and powers off completely.",
			switch_width,
			switch_height,
			Widgets::Level::Low,
			Widgets::Level::Medium,
			[ & ]
			{
				if ( Widgets::toggle_switch( "Hibernate", &hibernate ) )
					set_status( system_->set<Modules::SystemFeature::HibernateEnabled>( hibernate ) );
			}
		);

		ImGui::Separator( );

		bool fast_startup = system_->get<Modules::SystemFeature::FastStartup>( );

		Widgets::settings_row(
			"Fast startup",
			"Hibernates the kernel session on shutdown to speed up the next boot.",
			switch_width,
			switch_height,
			Widgets::Level::Medium,
			Widgets::Level::Medium,
			[ & ]
			{
				if ( Widgets::toggle_switch( "Fast startup", &fast_startup ) )
					set_status( system_->set<Modules::SystemFeature::FastStartup>( fast_startup ) );
			}
		);

		ImGui::Separator( );

		bool sleep_diagnostics = system_->get<Modules::SystemFeature::SleepDiagnostics>( );

		Widgets::settings_row(
			"Sleep diagnostics",
			"Background study of sleep-wake behavior used for diagnostics telemetry.",
			switch_width,
			switch_height,
			Widgets::Level::Low,
			Widgets::Level::Low,
			[ & ]
			{
				if ( Widgets::toggle_switch( "Sleep diagnostics", &sleep_diagnostics ) )
					set_status( system_->set<Modules::SystemFeature::SleepDiagnostics>( sleep_diagnostics ) );
			}
		);

		ImGui::Separator( );

		bool energy_estimation = system_->get<Modules::SystemFeature::EnergyEstimation>( );

		Widgets::settings_row(
			"Energy estimation engine",
			"Background service that estimates per-app battery usage.",
			switch_width,
			switch_height,
			Widgets::Level::Low,
			Widgets::Level::Low,
			[ & ]
			{
				if ( Widgets::toggle_switch( "Energy estimation engine", &energy_estimation ) )
					set_status( system_->set<Modules::SystemFeature::EnergyEstimation>( energy_estimation ) );
			}
		);

		ImGui::Separator( );

		bool modern_standby = system_->get<Modules::SystemFeature::ModernStandby>( );

		Widgets::settings_row(
			"Modern Standby",
			"Low-power connected standby (S0). Off forces classic S3 sleep on supported hardware.",
			switch_width,
			switch_height,
			Widgets::Level::Medium,
			Widgets::Level::High,
			[ & ]
			{
				if ( Widgets::toggle_switch( "Modern Standby", &modern_standby ) )
					set_status( system_->set<Modules::SystemFeature::ModernStandby>( modern_standby ) );
			}
		);
	}

	void SystemPage::render_advanced( )
	{
		const float switch_width  = ImGui::GetFrameHeight( ) * Widgets::switch_aspect;
		const float switch_height = ImGui::GetFrameHeight( );

		bool prefetch = system_->get<Modules::SystemFeature::Prefetch>( );

		Widgets::settings_row(
			"Prefetch",
			"Preloads frequently used applications into memory ahead of time.",
			switch_width,
			switch_height,
			Widgets::Level::Low,
			Widgets::Level::Low,
			[ & ]
			{
				if ( Widgets::toggle_switch( "Prefetch", &prefetch ) )
					set_status( system_->set<Modules::SystemFeature::Prefetch>( prefetch ) );
			}
		);

		ImGui::Separator( );

		bool tsx = system_->get<Modules::SystemFeature::Tsx>( );

		Widgets::settings_row(
			"Intel TSX",
			"CPU transactional memory extension. Recommended on for Intel, off for AMD.",
			switch_width,
			switch_height,
			Widgets::Level::Low,
			Widgets::Level::Medium,
			[ & ]
			{
				if ( Widgets::toggle_switch( "Intel TSX", &tsx ) )
					set_status( system_->set<Modules::SystemFeature::Tsx>( tsx ) );
			}
		);
	}

	void SystemPage::render_responsiveness( )
	{
		render_preset_row<Modules::SystemFeature::ForegroundLockTimeout>(
			"Foreground lock timeout", "Delay before a background app is allowed to steal focus from you.", 0, Widgets::Level::Low,
			Widgets::Level::Low
		);

		ImGui::Separator( );

		render_preset_row<Modules::SystemFeature::MenuShowDelay>(
			"Menu show delay", "Delay before hovering a menu item opens its submenu.", 0, Widgets::Level::Low, Widgets::Level::Low
		);

		ImGui::Separator( );

		const float switch_width  = ImGui::GetFrameHeight( ) * Widgets::switch_aspect;
		const float switch_height = ImGui::GetFrameHeight( );

		bool animations = system_->get<Modules::SystemFeature::UiAnimations>( );

		Widgets::settings_row(
			"UI animations",
			"Menu, tooltip and cursor shadow animations. Off trims perceived click latency.",
			switch_width,
			switch_height,
			Widgets::Level::Medium,
			Widgets::Level::Low,
			[ & ]
			{
				if ( Widgets::toggle_switch( "UI animations", &animations ) )
					set_status( system_->set<Modules::SystemFeature::UiAnimations>( animations ) );
			}
		);
	}

	void SystemPage::render_explorer( )
	{
		const float switch_width  = ImGui::GetFrameHeight( ) * Widgets::switch_aspect;
		const float switch_height = ImGui::GetFrameHeight( );

		bool show_extensions = system_->get<Modules::SystemFeature::ShowFileExtensions>( );

		Widgets::settings_row(
			"Show file extensions",
			"Shows the file name extension for every file in Explorer.",
			switch_width,
			switch_height,
			Widgets::Level::Low,
			Widgets::Level::Low,
			[ & ]
			{
				if ( Widgets::toggle_switch( "Show file extensions", &show_extensions ) )
					set_status( system_->set<Modules::SystemFeature::ShowFileExtensions>( show_extensions ) );
			}
		);

		ImGui::Separator( );

		bool dark_mode = system_->get<Modules::SystemFeature::DarkMode>( );

		Widgets::settings_row(
			"Dark mode",
			"Switches Windows apps to the dark color theme.",
			switch_width,
			switch_height,
			Widgets::Level::Low,
			Widgets::Level::Low,
			[ & ]
			{
				if ( Widgets::toggle_switch( "Dark mode", &dark_mode ) )
					set_status( system_->set<Modules::SystemFeature::DarkMode>( dark_mode ) );
			}
		);

		ImGui::Separator( );

		bool toast_notifications = system_->get<Modules::SystemFeature::ToastNotifications>( );

		Widgets::settings_row(
			"Toast notifications",
			"Allows apps to show notification banners and the notification center badge.",
			switch_width,
			switch_height,
			Widgets::Level::Low,
			Widgets::Level::Low,
			[ & ]
			{
				if ( Widgets::toggle_switch( "Toast notifications", &toast_notifications ) )
					set_status( system_->set<Modules::SystemFeature::ToastNotifications>( toast_notifications ) );
			}
		);

		ImGui::Separator( );

		bool fast_app_termination = system_->get<Modules::SystemFeature::FastAppTermination>( );

		Widgets::settings_row(
			"Fast app termination",
			"Shortens the delay before Windows force-closes a hung or hooked application.",
			switch_width,
			switch_height,
			Widgets::Level::Medium,
			Widgets::Level::Medium,
			[ & ]
			{
				if ( Widgets::toggle_switch( "Fast app termination", &fast_app_termination ) )
					set_status( system_->set<Modules::SystemFeature::FastAppTermination>( fast_app_termination ) );
			}
		);
	}

	void SystemPage::render_storage( )
	{
		ImGui::PushID( "svchost.exe split threshold" );

		Widgets::settings_row(
			"svchost.exe split threshold",
			"RAM, in KB, above which Windows splits shared services into separate svchost.exe processes.",
			Widgets::preset_row_width( 2 ),
			ImGui::GetFrameHeight( ),
			Widgets::Level::Low,
			Widgets::Level::Low,
			[ & ]
			{
				if ( ImGui::Button( "Default", ImVec2( Widgets::preset_button_width, 0.f ) ) )
					set_status( system_->set<Modules::SystemFeature::SvcHostSplitThreshold>(
						Modules::FeatureTraits<Modules::SystemFeature::SvcHostSplitThreshold>::fallback( ) ) );

				ImGui::SameLine( );

				if ( ImGui::Button( "Recommended", ImVec2( Widgets::preset_button_width, 0.f ) ) )
				{
					MEMORYSTATUSEX memory_status { .dwLength = sizeof( memory_status ) };

					if ( GlobalMemoryStatusEx( &memory_status ) )
						set_status( system_->set<Modules::SystemFeature::SvcHostSplitThreshold>(
							static_cast<std::uint32_t>( memory_status.ullTotalPhys / 1024 ) ) );
					else
						set_status( std::unexpected( Core::Error::Unknown ) );
				}
			}
		);

		ImGui::PopID( );

		ImGui::Separator( );

		render_preset_row<Modules::SystemFeature::DisableLastAccessTimestamps>(
			"Disable NTFS last-access timestamps", "Stops updating a file's last-accessed time on every read.", 3, Widgets::Level::Low,
			Widgets::Level::Low
		);

		ImGui::Separator( );

		render_preset_row<Modules::SystemFeature::Disable8dot3NameCreation>(
			"Disable 8.3 short filenames", "Stops NTFS from generating legacy DOS-compatible short file names.", 1, Widgets::Level::Low,
			Widgets::Level::Medium
		);

		ImGui::Separator( );

		render_preset_row<Modules::SystemFeature::RestorePointFrequency>(
			"Restore point frequency", "Minimum minutes between automatic System Restore checkpoints. Optimized creates one on every change.", 0,
			Widgets::Level::Low, Widgets::Level::Low
		);
	}

	void SystemPage::render( )
	{
		if ( ImGui::CollapsingHeader( "Scheduling", ImGuiTreeNodeFlags_DefaultOpen ) )
			render_scheduling( );

		if ( ImGui::CollapsingHeader( "Power", ImGuiTreeNodeFlags_DefaultOpen ) )
			render_power( );

		if ( ImGui::CollapsingHeader( "Responsiveness", ImGuiTreeNodeFlags_DefaultOpen ) )
			render_responsiveness( );

		if ( ImGui::CollapsingHeader( "Advanced", ImGuiTreeNodeFlags_DefaultOpen ) )
			render_advanced( );

		if ( ImGui::CollapsingHeader( "Explorer", ImGuiTreeNodeFlags_DefaultOpen ) )
			render_explorer( );

		if ( ImGui::CollapsingHeader( "Storage", ImGuiTreeNodeFlags_DefaultOpen ) )
			render_storage( );
	}
}

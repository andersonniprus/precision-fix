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
	void SystemPage::render_preset_row( const char* title, const char* description, const std::uint32_t optimized )
	{
		ImGui::PushID( title );

		Widgets::settings_row( title, description, Widgets::preset_row_width( 2 ), ImGui::GetFrameHeight( ), [ & ]
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
			"Win32 priority separation", "Splits CPU time between the foreground application and background tasks.", 0x26
		);

		ImGui::Separator( );

		render_preset_row<Modules::SystemFeature::Responsiveness>(
			"System responsiveness", "Percentage of CPU reserved away from background multimedia tasks.", 10
		);

		ImGui::Separator( );

		render_preset_row<Modules::SystemFeature::NetworkThrottling>(
			"Network throttling index", "Caps network throughput the OS reserves for background multimedia tasks.", 0xFFFFFFFF
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
			[ & ]
			{
				if ( Widgets::toggle_switch( "Processor idle states", &idle_states ) )
					set_status( system_->set<Modules::SystemFeature::ProcessorIdleStates>( idle_states ) );
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
			"Foreground lock timeout", "Delay before a background app is allowed to steal focus from you.", 0
		);

		ImGui::Separator( );

		render_preset_row<Modules::SystemFeature::MenuShowDelay>(
			"Menu show delay", "Delay before hovering a menu item opens its submenu.", 0
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
			[ & ]
			{
				if ( Widgets::toggle_switch( "UI animations", &animations ) )
					set_status( system_->set<Modules::SystemFeature::UiAnimations>( animations ) );
			}
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
	}
}

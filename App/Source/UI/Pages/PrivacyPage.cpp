#include "Stdafx.hpp"
#include "UI/Pages/PrivacyPage.hpp"
#include "UI/Widgets/SettingsRow.hpp"
#include "UI/Widgets/Switch.hpp"

namespace UI::Pages
{
	PrivacyPage::PrivacyPage( std::shared_ptr<Modules::PrivacyModule> privacy )
		: privacy_( std::move( privacy ) )
	{
	}

	void PrivacyPage::on_activate( )
	{
		privacy_->refresh_all( );
	}

	void PrivacyPage::set_status( const Core::Status& status )
	{
		status_ = status ? std::string {} : std::string { Core::to_string( status.error( ) ) };
	}

	void PrivacyPage::render_telemetry( )
	{
		const float switch_width  = ImGui::GetFrameHeight( ) * Widgets::switch_aspect;
		const float switch_height = ImGui::GetFrameHeight( );

		bool telemetry = privacy_->get<Modules::PrivacyFeature::Telemetry>( );

		Widgets::settings_row(
			"Diagnostic data collection",
			"The main Windows telemetry opt-out (requires an Enterprise/Education edition for full effect).",
			switch_width,
			switch_height,
			Widgets::Level::Low,
			Widgets::Level::Low,
			[ & ]
			{
				if ( Widgets::toggle_switch( "Diagnostic data collection", &telemetry ) )
					set_status( privacy_->set<Modules::PrivacyFeature::Telemetry>( telemetry ) );
			}
		);

		ImGui::Separator( );

		bool diagnostic_execution = privacy_->get<Modules::PrivacyFeature::DiagnosticExecution>( );

		Widgets::settings_row(
			"Application inventory collection",
			"Stops the compatibility appraiser from inventorying installed applications.",
			switch_width,
			switch_height,
			Widgets::Level::Low,
			Widgets::Level::Low,
			[ & ]
			{
				if ( Widgets::toggle_switch( "Application inventory collection", &diagnostic_execution ) )
					set_status( privacy_->set<Modules::PrivacyFeature::DiagnosticExecution>( diagnostic_execution ) );
			}
		);

		ImGui::Separator( );

		bool error_reporting = privacy_->get<Modules::PrivacyFeature::ErrorReporting>( );

		Widgets::settings_row(
			"Windows Error Reporting",
			"Stops crash dumps and error reports from being sent to Microsoft.",
			switch_width,
			switch_height,
			Widgets::Level::Low,
			Widgets::Level::Medium,
			[ & ]
			{
				if ( Widgets::toggle_switch( "Windows Error Reporting", &error_reporting ) )
					set_status( privacy_->set<Modules::PrivacyFeature::ErrorReporting>( error_reporting ) );
			}
		);

		ImGui::Separator( );

		bool diagnostic_tasks = privacy_->get<Modules::PrivacyFeature::DiagnosticTasks>( );

		Widgets::settings_row(
			"Diagnostic scheduled tasks",
			"Disables the background scheduled tasks that gather usage and diagnostic data.",
			switch_width,
			switch_height,
			Widgets::Level::Low,
			Widgets::Level::Low,
			[ & ]
			{
				if ( Widgets::toggle_switch( "Diagnostic scheduled tasks", &diagnostic_tasks ) )
					set_status( privacy_->set<Modules::PrivacyFeature::DiagnosticTasks>( diagnostic_tasks ) );
			}
		);

		ImGui::Separator( );

		bool feedback_prompts = privacy_->get<Modules::PrivacyFeature::FeedbackPrompts>( );

		Widgets::settings_row(
			"Feedback prompts",
			"Stops Windows from periodically asking for feedback.",
			switch_width,
			switch_height,
			Widgets::Level::Low,
			Widgets::Level::Low,
			[ & ]
			{
				if ( Widgets::toggle_switch( "Feedback prompts", &feedback_prompts ) )
					set_status( privacy_->set<Modules::PrivacyFeature::FeedbackPrompts>( feedback_prompts ) );
			}
		);
	}

	void PrivacyPage::render_personalization( )
	{
		const float switch_width  = ImGui::GetFrameHeight( ) * Widgets::switch_aspect;
		const float switch_height = ImGui::GetFrameHeight( );

		bool advertising_id = privacy_->get<Modules::PrivacyFeature::AdvertisingId>( );

		Widgets::settings_row(
			"Advertising ID",
			"Per-user identifier apps use to personalize and track advertising.",
			switch_width,
			switch_height,
			Widgets::Level::Low,
			Widgets::Level::Low,
			[ & ]
			{
				if ( Widgets::toggle_switch( "Advertising ID", &advertising_id ) )
					set_status( privacy_->set<Modules::PrivacyFeature::AdvertisingId>( advertising_id ) );
			}
		);

		ImGui::Separator( );

		bool tailored_experiences = privacy_->get<Modules::PrivacyFeature::TailoredExperiences>( );

		Widgets::settings_row(
			"Tailored experiences",
			"Stops Windows from using your diagnostic data to personalize tips and suggestions.",
			switch_width,
			switch_height,
			Widgets::Level::Low,
			Widgets::Level::Low,
			[ & ]
			{
				if ( Widgets::toggle_switch( "Tailored experiences", &tailored_experiences ) )
					set_status( privacy_->set<Modules::PrivacyFeature::TailoredExperiences>( tailored_experiences ) );
			}
		);

		ImGui::Separator( );

		bool content_suggestions = privacy_->get<Modules::PrivacyFeature::ContentSuggestions>( );

		Widgets::settings_row(
			"Suggested content & pre-installed apps",
			"Stops Start menu suggestions and Microsoft Store app pre-installs.",
			switch_width,
			switch_height,
			Widgets::Level::Low,
			Widgets::Level::Low,
			[ & ]
			{
				if ( Widgets::toggle_switch( "Suggested content & pre-installed apps", &content_suggestions ) )
					set_status( privacy_->set<Modules::PrivacyFeature::ContentSuggestions>( content_suggestions ) );
			}
		);

		ImGui::Separator( );

		bool news_and_interests = privacy_->get<Modules::PrivacyFeature::NewsAndInterests>( );

		Widgets::settings_row(
			"News and interests",
			"Disables the taskbar News and interests widget content policy.",
			switch_width,
			switch_height,
			Widgets::Level::Low,
			Widgets::Level::Low,
			[ & ]
			{
				if ( Widgets::toggle_switch( "News and interests", &news_and_interests ) )
					set_status( privacy_->set<Modules::PrivacyFeature::NewsAndInterests>( news_and_interests ) );
			}
		);

		ImGui::Separator( );

		bool windows_feeds = privacy_->get<Modules::PrivacyFeature::WindowsFeeds>( );

		Widgets::settings_row(
			"Windows feeds",
			"Disables the Widgets board content feed.",
			switch_width,
			switch_height,
			Widgets::Level::Low,
			Widgets::Level::Low,
			[ & ]
			{
				if ( Widgets::toggle_switch( "Windows feeds", &windows_feeds ) )
					set_status( privacy_->set<Modules::PrivacyFeature::WindowsFeeds>( windows_feeds ) );
			}
		);

		ImGui::Separator( );

		bool activity_feed = privacy_->get<Modules::PrivacyFeature::ActivityFeed>( );

		Widgets::settings_row(
			"Activity feed",
			"Stops Windows from recording and uploading your activity history.",
			switch_width,
			switch_height,
			Widgets::Level::Low,
			Widgets::Level::Low,
			[ & ]
			{
				if ( Widgets::toggle_switch( "Activity feed", &activity_feed ) )
					set_status( privacy_->set<Modules::PrivacyFeature::ActivityFeed>( activity_feed ) );
			}
		);

		ImGui::Separator( );

		bool setting_sync = privacy_->get<Modules::PrivacyFeature::SettingSync>( );

		Widgets::settings_row(
			"Settings sync",
			"Stops Windows from syncing settings, theme and layout to your Microsoft account.",
			switch_width,
			switch_height,
			Widgets::Level::Low,
			Widgets::Level::Medium,
			[ & ]
			{
				if ( Widgets::toggle_switch( "Settings sync", &setting_sync ) )
					set_status( privacy_->set<Modules::PrivacyFeature::SettingSync>( setting_sync ) );
			}
		);
	}

	void PrivacyPage::render_gaming_and_location( )
	{
		const float switch_width  = ImGui::GetFrameHeight( ) * Widgets::switch_aspect;
		const float switch_height = ImGui::GetFrameHeight( );

		bool game_dvr = privacy_->get<Modules::PrivacyFeature::GameDvr>( );

		Widgets::settings_row(
			"Game Bar / Game DVR",
			"Disables Xbox Game Bar recording, capture and overlay for all apps.",
			switch_width,
			switch_height,
			Widgets::Level::Medium,
			Widgets::Level::Low,
			[ & ]
			{
				if ( Widgets::toggle_switch( "Game Bar / Game DVR", &game_dvr ) )
					set_status( privacy_->set<Modules::PrivacyFeature::GameDvr>( game_dvr ) );
			}
		);

		ImGui::Separator( );

		bool location_services = privacy_->get<Modules::PrivacyFeature::LocationServices>( );

		Widgets::settings_row(
			"Location services",
			"Disables system-wide location, sensors and the Windows location provider.",
			switch_width,
			switch_height,
			Widgets::Level::Low,
			Widgets::Level::Medium,
			[ & ]
			{
				if ( Widgets::toggle_switch( "Location services", &location_services ) )
					set_status( privacy_->set<Modules::PrivacyFeature::LocationServices>( location_services ) );
			}
		);
	}

	void PrivacyPage::render( )
	{
		if ( ImGui::CollapsingHeader( "Telemetry", ImGuiTreeNodeFlags_DefaultOpen ) )
			render_telemetry( );

		if ( ImGui::CollapsingHeader( "Personalization", ImGuiTreeNodeFlags_DefaultOpen ) )
			render_personalization( );

		if ( ImGui::CollapsingHeader( "Gaming & location", ImGuiTreeNodeFlags_DefaultOpen ) )
			render_gaming_and_location( );

		if ( !status_.empty( ) )
			ImGui::TextColored( ImVec4( 1.f, 0.35f, 0.35f, 1.f ), "%s", status_.c_str( ) );
	}
}

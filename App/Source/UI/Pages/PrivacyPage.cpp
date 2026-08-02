#include "Stdafx.hpp"
#include "UI/Pages/PrivacyPage.hpp"
#include "UI/Theme.hpp"
#include "UI/Fonts/IconsLucide.h"
#include "UI/Widgets/Section.hpp"
#include "UI/Widgets/SettingsRow.hpp"
#include "UI/Widgets/Switch.hpp"

namespace UI::Pages
{
	using namespace Widgets;
	using enum Modules::PrivacyFeature;

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

	void PrivacyPage::render( )
	{
		const float switch_width  = ImGui::GetFrameHeight( ) * switch_aspect;
		const float switch_height = ImGui::GetFrameHeight( );

		section( ICON_LC_ACTIVITY, "Telemetry", "Diagnostic data and feedback.", [ & ]
		{
			bool telemetry = privacy_->get<Telemetry>( );

			settings_row(
				"Diagnostic data collection",
				"The main Windows telemetry opt-out (requires an Enterprise/Education edition for full effect).",
				switch_width, switch_height,
				Level::Low, Level::Low,
				"Off",
				[ & ]
				{
					if ( toggle_switch( "Diagnostic data collection", &telemetry ) )
					{
						set_status( privacy_->set<Telemetry>( telemetry ) );
					}
				}
			);

			bool diagnostic_execution = privacy_->get<DiagnosticExecution>( );

			settings_row(
				"Application inventory collection",
				"Stops the compatibility appraiser from inventorying installed applications.",
				switch_width, switch_height,
				Level::Low, Level::Low,
				"Off",
				[ & ]
				{
					if ( toggle_switch( "Application inventory collection", &diagnostic_execution ) )
					{
						set_status( privacy_->set<DiagnosticExecution>( diagnostic_execution ) );
					}
				}
			);

			bool error_reporting = privacy_->get<ErrorReporting>( );

			settings_row(
				"Windows Error Reporting",
				"Stops crash dumps and error reports from being sent to Microsoft.",
				switch_width, switch_height,
				Level::Low, Level::Medium,
				"Off",
				[ & ]
				{
					if ( toggle_switch( "Windows Error Reporting", &error_reporting ) )
					{
						set_status( privacy_->set<ErrorReporting>( error_reporting ) );
					}
				}
			);

			bool diagnostic_tasks = privacy_->get<DiagnosticTasks>( );

			settings_row(
				"Diagnostic scheduled tasks",
				"Disables the background scheduled tasks that gather usage and diagnostic data.",
				switch_width, switch_height,
				Level::Low, Level::Low,
				"Off",
				[ & ]
				{
					if ( toggle_switch( "Diagnostic scheduled tasks", &diagnostic_tasks ) )
					{
						set_status( privacy_->set<DiagnosticTasks>( diagnostic_tasks ) );
					}
				}
			);

			bool feedback_prompts = privacy_->get<FeedbackPrompts>( );

			settings_row(
				"Feedback prompts",
				"Stops Windows from periodically asking for feedback.",
				switch_width, switch_height,
				Level::Low, Level::Low,
				"Off",
				[ & ]
				{
					if ( toggle_switch( "Feedback prompts", &feedback_prompts ) )
					{
						set_status( privacy_->set<FeedbackPrompts>( feedback_prompts ) );
					}
				}
			);
		} );

		section( ICON_LC_SPARKLES, "Personalization", "Ads, suggestions and sync.", [ & ]
		{
			bool advertising_id = privacy_->get<AdvertisingId>( );

			settings_row(
				"Advertising ID",
				"Per-user identifier apps use to personalize and track advertising.",
				switch_width, switch_height,
				Level::Low, Level::Low,
				"Off",
				[ & ]
				{
					if ( toggle_switch( "Advertising ID", &advertising_id ) )
					{
						set_status( privacy_->set<AdvertisingId>( advertising_id ) );
					}
				}
			);

			bool tailored_experiences = privacy_->get<TailoredExperiences>( );

			settings_row(
				"Tailored experiences",
				"Stops Windows from using your diagnostic data to personalize tips and suggestions.",
				switch_width, switch_height,
				Level::Low, Level::Low,
				"Off",
				[ & ]
				{
					if ( toggle_switch( "Tailored experiences", &tailored_experiences ) )
					{
						set_status( privacy_->set<TailoredExperiences>( tailored_experiences ) );
					}
				}
			);

			bool content_suggestions = privacy_->get<ContentSuggestions>( );

			settings_row(
				"Suggested content & pre-installed apps",
				"Stops Start menu suggestions and Microsoft Store app pre-installs.",
				switch_width, switch_height,
				Level::Low, Level::Low,
				"Off",
				[ & ]
				{
					if ( toggle_switch( "Suggested content & pre-installed apps", &content_suggestions ) )
					{
						set_status( privacy_->set<ContentSuggestions>( content_suggestions ) );
					}
				}
			);

			bool news_and_interests = privacy_->get<NewsAndInterests>( );

			settings_row(
				"News and interests",
				"Disables the taskbar News and interests widget content policy.",
				switch_width, switch_height,
				Level::Low, Level::Low,
				"Off",
				[ & ]
				{
					if ( toggle_switch( "News and interests", &news_and_interests ) )
					{
						set_status( privacy_->set<NewsAndInterests>( news_and_interests ) );
					}
				}
			);

			bool windows_feeds = privacy_->get<WindowsFeeds>( );

			settings_row(
				"Windows feeds",
				"Disables the Widgets board content feed.",
				switch_width, switch_height,
				Level::Low, Level::Low,
				"Off",
				[ & ]
				{
					if ( toggle_switch( "Windows feeds", &windows_feeds ) )
					{
						set_status( privacy_->set<WindowsFeeds>( windows_feeds ) );
					}
				}
			);

			bool activity_feed = privacy_->get<ActivityFeed>( );

			settings_row(
				"Activity feed",
				"Stops Windows from recording and uploading your activity history.",
				switch_width, switch_height,
				Level::Low, Level::Low,
				"Off",
				[ & ]
				{
					if ( toggle_switch( "Activity feed", &activity_feed ) )
					{
						set_status( privacy_->set<ActivityFeed>( activity_feed ) );
					}
				}
			);

			bool setting_sync = privacy_->get<SettingSync>( );

			settings_row(
				"Settings sync",
				"Stops Windows from syncing settings, theme and layout to your Microsoft account.",
				switch_width, switch_height,
				Level::Low, Level::Medium,
				"Off",
				[ & ]
				{
					if ( toggle_switch( "Settings sync", &setting_sync ) )
					{
						set_status( privacy_->set<SettingSync>( setting_sync ) );
					}
				}
			);
		} );

		section( ICON_LC_GAMEPAD, "Gaming & location", "Game Bar and location services.", [ & ]
		{
			bool game_dvr = privacy_->get<GameDvr>( );

			settings_row(
				"Game Bar / Game DVR",
				"Disables Xbox Game Bar recording, capture and overlay for all apps.",
				switch_width, switch_height,
				Level::Medium, Level::Low,
				"Off",
				[ & ]
				{
					if ( toggle_switch( "Game Bar / Game DVR", &game_dvr ) )
					{
						set_status( privacy_->set<GameDvr>( game_dvr ) );
					}
				}
			);

			bool location_services = privacy_->get<LocationServices>( );

			settings_row(
				"Location services",
				"Disables system-wide location, sensors and the Windows location provider.",
				switch_width, switch_height,
				Level::Low, Level::Medium,
				"Off",
				[ & ]
				{
					if ( toggle_switch( "Location services", &location_services ) )
					{
						set_status( privacy_->set<LocationServices>( location_services ) );
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

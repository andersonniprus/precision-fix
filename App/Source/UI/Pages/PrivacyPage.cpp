#include "Stdafx.hpp"
#include "UI/Pages/PrivacyPage.hpp"
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
	using enum Modules::PrivacyFeature;

	PrivacyPage::PrivacyPage(
		std::shared_ptr<Modules::PrivacyModule> module,
		std::shared_ptr<App::Logger> logger,
		std::shared_ptr<App::Intl> intl
	)
		: privacy_( std::move( module ) ),
		  logger_( std::move( logger ) ),
		  intl_( std::move( intl ) )
	{
	}

	void PrivacyPage::on_activate( )
	{
		privacy_->refresh_all( );
	}

	void PrivacyPage::set_status( const Core::Status& status )
	{
		if ( status )
		{
			logger_->info( "Privacy", "setting applied" );
			status_.clear( );
			return;
		}

		logger_->error( "Privacy", std::string { Core::to_string( status.error( ) ) } );
		status_ = std::string { Core::to_string( status.error( ) ) };
	}


	void PrivacyPage::render( )
	{
		const float switch_width  = ImGui::GetFrameHeight( ) * switch_aspect;
		const float switch_height = ImGui::GetFrameHeight( );

		section( ICON_LC_ACTIVITY, intl_->tr( "Telemetry" ), intl_->tr( "Diagnostic data and feedback." ), [ & ]
		{
			bool telemetry = privacy_->get<Telemetry>( );

			settings_row(
				intl_->tr( "Diagnostic data collection" ),
				intl_->tr( "The main Windows telemetry opt-out (requires an Enterprise/Education edition for full effect)." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "Off" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Diagnostic data collection" ), &telemetry ) )
					{
						set_status( privacy_->set<Telemetry>( telemetry ) );
					}
				}
			);

			bool diagnostic_execution = privacy_->get<DiagnosticExecution>( );

			settings_row(
				intl_->tr( "Application inventory collection" ),
				intl_->tr( "Stops the compatibility appraiser from inventorying installed applications." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "Off" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Application inventory collection" ), &diagnostic_execution ) )
					{
						set_status( privacy_->set<DiagnosticExecution>( diagnostic_execution ) );
					}
				}
			);

			bool error_reporting = privacy_->get<ErrorReporting>( );

			settings_row(
				intl_->tr( "Windows Error Reporting" ),
				intl_->tr( "Stops crash dumps and error reports from being sent to Microsoft." ),
				switch_width, switch_height,
				Level::Low, Level::Medium,
				intl_->tr( "Off" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Windows Error Reporting" ), &error_reporting ) )
					{
						set_status( privacy_->set<ErrorReporting>( error_reporting ) );
					}
				}
			);

			bool diagnostic_tasks = privacy_->get<DiagnosticTasks>( );

			settings_row(
				intl_->tr( "Diagnostic scheduled tasks" ),
				intl_->tr( "Disables the background scheduled tasks that gather usage and diagnostic data." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "Off" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Diagnostic scheduled tasks" ), &diagnostic_tasks ) )
					{
						set_status( privacy_->set<DiagnosticTasks>( diagnostic_tasks ) );
					}
				}
			);

			bool feedback_prompts = privacy_->get<FeedbackPrompts>( );

			settings_row(
				intl_->tr( "Feedback prompts" ),
				intl_->tr( "Stops Windows from periodically asking for feedback." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "Off" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Feedback prompts" ), &feedback_prompts ) )
					{
						set_status( privacy_->set<FeedbackPrompts>( feedback_prompts ) );
					}
				}
			);
		} );

		section( ICON_LC_SPARKLES, intl_->tr( "Personalization" ), intl_->tr( "Ads, suggestions and sync." ), [ & ]
		{
			bool advertising_id = privacy_->get<AdvertisingId>( );

			settings_row(
				intl_->tr( "Advertising ID" ),
				intl_->tr( "Per-user identifier apps use to personalize and track advertising." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "Off" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Advertising ID" ), &advertising_id ) )
					{
						set_status( privacy_->set<AdvertisingId>( advertising_id ) );
					}
				}
			);

			bool tailored_experiences = privacy_->get<TailoredExperiences>( );

			settings_row(
				intl_->tr( "Tailored experiences" ),
				intl_->tr( "Stops Windows from using your diagnostic data to personalize tips and suggestions." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "Off" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Tailored experiences" ), &tailored_experiences ) )
					{
						set_status( privacy_->set<TailoredExperiences>( tailored_experiences ) );
					}
				}
			);

			bool content_suggestions = privacy_->get<ContentSuggestions>( );

			settings_row(
				intl_->tr( "Suggested content & pre-installed apps" ),
				intl_->tr( "Stops Start menu suggestions and Microsoft Store app pre-installs." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "Off" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Suggested content & pre-installed apps" ), &content_suggestions ) )
					{
						set_status( privacy_->set<ContentSuggestions>( content_suggestions ) );
					}
				}
			);

			bool news_and_interests = privacy_->get<NewsAndInterests>( );

			settings_row(
				intl_->tr( "News and interests" ),
				intl_->tr( "Disables the taskbar News and interests widget content policy." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "Off" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "News and interests" ), &news_and_interests ) )
					{
						set_status( privacy_->set<NewsAndInterests>( news_and_interests ) );
					}
				}
			);

			bool windows_feeds = privacy_->get<WindowsFeeds>( );

			settings_row(
				intl_->tr( "Windows feeds" ),
				intl_->tr( "Disables the Widgets board content feed." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "Off" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Windows feeds" ), &windows_feeds ) )
					{
						set_status( privacy_->set<WindowsFeeds>( windows_feeds ) );
					}
				}
			);

			bool activity_feed = privacy_->get<ActivityFeed>( );

			settings_row(
				intl_->tr( "Activity feed" ),
				intl_->tr( "Stops Windows from recording and uploading your activity history." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "Off" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Activity feed" ), &activity_feed ) )
					{
						set_status( privacy_->set<ActivityFeed>( activity_feed ) );
					}
				}
			);

			bool setting_sync = privacy_->get<SettingSync>( );

			settings_row(
				intl_->tr( "Settings sync" ),
				intl_->tr( "Stops Windows from syncing settings, theme and layout to your Microsoft account." ),
				switch_width, switch_height,
				Level::Low, Level::Medium,
				intl_->tr( "Off" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Settings sync" ), &setting_sync ) )
					{
						set_status( privacy_->set<SettingSync>( setting_sync ) );
					}
				}
			);
		} );

		section( ICON_LC_GAMEPAD, intl_->tr( "Gaming & location" ), intl_->tr( "Game Bar and location services." ), [ & ]
		{
			bool game_dvr = privacy_->get<GameDvr>( );

			settings_row(
				intl_->tr( "Game Bar / Game DVR" ),
				intl_->tr( "Disables Xbox Game Bar recording, capture and overlay for all apps." ),
				switch_width, switch_height,
				Level::Medium, Level::Low,
				intl_->tr( "Off" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Game Bar / Game DVR" ), &game_dvr ) )
					{
						set_status( privacy_->set<GameDvr>( game_dvr ) );
					}
				}
			);

			bool location_services = privacy_->get<LocationServices>( );

			settings_row(
				intl_->tr( "Location services" ),
				intl_->tr( "Disables system-wide location, sensors and the Windows location provider." ),
				switch_width, switch_height,
				Level::Low, Level::Medium,
				intl_->tr( "Off" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Location services" ), &location_services ) )
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

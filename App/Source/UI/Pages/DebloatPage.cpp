#include "Stdafx.hpp"
#include "UI/Pages/DebloatPage.hpp"
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
	using enum Modules::DebloatFeature;

	DebloatPage::DebloatPage(
		std::shared_ptr<Modules::DebloatModule> module,
		std::shared_ptr<App::Logger> logger,
		std::shared_ptr<App::Intl> intl
	)
		: debloat_( std::move( module ) ),
		  logger_( std::move( logger ) ),
		  intl_( std::move( intl ) )
	{
	}

	void DebloatPage::on_activate( )
	{
		debloat_->refresh_all( );
	}

	void DebloatPage::set_status( const Core::Status& status )
	{
		if ( status )
		{
			logger_->info( "Debloat", "setting applied" );
			status_.clear( );
			return;
		}

		logger_->error( "Debloat", std::string { Core::to_string( status.error( ) ) } );
		status_ = std::string { Core::to_string( status.error( ) ) };
	}


	void DebloatPage::render( )
	{
		const float switch_width  = ImGui::GetFrameHeight( ) * switch_aspect;
		const float switch_height = ImGui::GetFrameHeight( );

		section( ICON_LC_PACKAGE, intl_->tr( "Apps" ), intl_->tr( "Built-in Windows apps you can remove." ), [ & ]
		{
			bool weather = debloat_->get<BingWeather>( );

			settings_row(
				intl_->tr( "Weather" ),
				intl_->tr( "Removes the built-in Weather app for all users." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Weather" ), &weather ) )
					{
						set_status( debloat_->set<BingWeather>( weather ) );
					}
				}
			);

			bool get_help = debloat_->get<GetHelp>( );

			settings_row(
				intl_->tr( "Get Help" ),
				intl_->tr( "Removes the Get Help support app." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Get Help" ), &get_help ) )
					{
						set_status( debloat_->set<GetHelp>( get_help ) );
					}
				}
			);

			bool tips = debloat_->get<GetStarted>( );

			settings_row(
				intl_->tr( "Tips" ),
				intl_->tr( "Removes the Tips (Get Started) app." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Tips" ), &tips ) )
					{
						set_status( debloat_->set<GetStarted>( tips ) );
					}
				}
			);

			bool heif_extension = debloat_->get<HeifImageExtension>( );

			settings_row(
				intl_->tr( "HEIF Image Extension" ),
				intl_->tr( "Removes the HEIF image codec extension." ),
				switch_width, switch_height,
				Level::Low, Level::Medium,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "HEIF Image Extension" ), &heif_extension ) )
					{
						set_status( debloat_->set<HeifImageExtension>( heif_extension ) );
					}
				}
			);

			bool messaging = debloat_->get<Messaging>( );

			settings_row(
				intl_->tr( "Messaging" ),
				intl_->tr( "Removes the legacy Messaging app." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Messaging" ), &messaging ) )
					{
						set_status( debloat_->set<Messaging>( messaging ) );
					}
				}
			);

			bool viewer_3d = debloat_->get<Microsoft3DViewer>( );

			settings_row(
				intl_->tr( "3D Viewer" ),
				intl_->tr( "Removes the 3D Viewer app." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "3D Viewer" ), &viewer_3d ) )
					{
						set_status( debloat_->set<Microsoft3DViewer>( viewer_3d ) );
					}
				}
			);

			bool solitaire = debloat_->get<SolitaireCollection>( );

			settings_row(
				intl_->tr( "Solitaire Collection" ),
				intl_->tr( "Removes the Microsoft Solitaire Collection game." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Solitaire Collection" ), &solitaire ) )
					{
						set_status( debloat_->set<SolitaireCollection>( solitaire ) );
					}
				}
			);

			bool sticky_notes = debloat_->get<StickyNotes>( );

			settings_row(
				intl_->tr( "Sticky Notes" ),
				intl_->tr( "Removes the built-in Sticky Notes app." ),
				switch_width, switch_height,
				Level::Low, Level::Medium,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Sticky Notes" ), &sticky_notes ) )
					{
						set_status( debloat_->set<StickyNotes>( sticky_notes ) );
					}
				}
			);

			bool mixed_reality = debloat_->get<MixedRealityPortal>( );

			settings_row(
				intl_->tr( "Mixed Reality Portal" ),
				intl_->tr( "Removes the Windows Mixed Reality Portal." ),
				switch_width, switch_height,
				Level::Medium, Level::Low,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Mixed Reality Portal" ), &mixed_reality ) )
					{
						set_status( debloat_->set<MixedRealityPortal>( mixed_reality ) );
					}
				}
			);

			bool mobile_plans = debloat_->get<OneConnect>( );

			settings_row(
				intl_->tr( "Mobile Plans" ),
				intl_->tr( "Removes the Mobile Plans (OneConnect) app." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Mobile Plans" ), &mobile_plans ) )
					{
						set_status( debloat_->set<OneConnect>( mobile_plans ) );
					}
				}
			);

			bool people = debloat_->get<People>( );

			settings_row(
				intl_->tr( "People" ),
				intl_->tr( "Removes the People contacts app." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "People" ), &people ) )
					{
						set_status( debloat_->set<People>( people ) );
					}
				}
			);

			bool print_3d = debloat_->get<Print3D>( );

			settings_row(
				intl_->tr( "Print 3D" ),
				intl_->tr( "Removes the Print 3D app." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Print 3D" ), &print_3d ) )
					{
						set_status( debloat_->set<Print3D>( print_3d ) );
					}
				}
			);

			bool skype = debloat_->get<SkypeApp>( );

			settings_row(
				intl_->tr( "Skype" ),
				intl_->tr( "Removes the built-in Skype app." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Skype" ), &skype ) )
					{
						set_status( debloat_->set<SkypeApp>( skype ) );
					}
				}
			);

			bool web_media_extensions = debloat_->get<WebMediaExtensions>( );

			settings_row(
				intl_->tr( "Web Media Extensions" ),
				intl_->tr( "Removes the Web Media Extensions codec pack." ),
				switch_width, switch_height,
				Level::Low, Level::Medium,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Web Media Extensions" ), &web_media_extensions ) )
					{
						set_status( debloat_->set<WebMediaExtensions>( web_media_extensions ) );
					}
				}
			);

			bool webp_extension = debloat_->get<WebpImageExtension>( );

			settings_row(
				intl_->tr( "WebP Image Extension" ),
				intl_->tr( "Removes the WebP image codec extension." ),
				switch_width, switch_height,
				Level::Low, Level::Medium,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "WebP Image Extension" ), &webp_extension ) )
					{
						set_status( debloat_->set<WebpImageExtension>( webp_extension ) );
					}
				}
			);

			bool alarms = debloat_->get<WindowsAlarms>( );

			settings_row(
				intl_->tr( "Alarms & Clock" ),
				intl_->tr( "Removes the Alarms & Clock app." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Alarms & Clock" ), &alarms ) )
					{
						set_status( debloat_->set<WindowsAlarms>( alarms ) );
					}
				}
			);

			bool camera = debloat_->get<WindowsCamera>( );

			settings_row(
				intl_->tr( "Camera" ),
				intl_->tr( "Removes the built-in Camera app." ),
				switch_width, switch_height,
				Level::Low, Level::Medium,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Camera" ), &camera ) )
					{
						set_status( debloat_->set<WindowsCamera>( camera ) );
					}
				}
			);

			bool feedback_hub = debloat_->get<FeedbackHub>( );

			settings_row(
				intl_->tr( "Feedback Hub" ),
				intl_->tr( "Removes the Feedback Hub app." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Feedback Hub" ), &feedback_hub ) )
					{
						set_status( debloat_->set<FeedbackHub>( feedback_hub ) );
					}
				}
			);

			bool maps = debloat_->get<WindowsMaps>( );

			settings_row(
				intl_->tr( "Maps" ),
				intl_->tr( "Removes the built-in Maps app and offline map data." ),
				switch_width, switch_height,
				Level::Medium, Level::Low,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Maps" ), &maps ) )
					{
						set_status( debloat_->set<WindowsMaps>( maps ) );
					}
				}
			);

			bool voice_recorder = debloat_->get<SoundRecorder>( );

			settings_row(
				intl_->tr( "Voice Recorder" ),
				intl_->tr( "Removes the Voice Recorder app." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Voice Recorder" ), &voice_recorder ) )
					{
						set_status( debloat_->set<SoundRecorder>( voice_recorder ) );
					}
				}
			);

			bool phone_link = debloat_->get<YourPhone>( );

			settings_row(
				intl_->tr( "Phone Link" ),
				intl_->tr( "Removes the Phone Link (Your Phone) companion app." ),
				switch_width, switch_height,
				Level::Low, Level::Medium,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Phone Link" ), &phone_link ) )
					{
						set_status( debloat_->set<YourPhone>( phone_link ) );
					}
				}
			);

			bool groove_music = debloat_->get<ZuneMusic>( );

			settings_row(
				intl_->tr( "Media Player (Groove Music)" ),
				intl_->tr( "Removes the legacy Groove Music app." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Media Player (Groove Music)" ), &groove_music ) )
					{
						set_status( debloat_->set<ZuneMusic>( groove_music ) );
					}
				}
			);

			bool mail_and_calendar = debloat_->get<MailAndCalendar>( );

			settings_row(
				intl_->tr( "Mail and Calendar" ),
				intl_->tr( "Removes the built-in Mail and Calendar apps." ),
				switch_width, switch_height,
				Level::Medium, Level::Medium,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Mail and Calendar" ), &mail_and_calendar ) )
					{
						set_status( debloat_->set<MailAndCalendar>( mail_and_calendar ) );
					}
				}
			);

			bool bing_apps = debloat_->get<BingApps>( );

			settings_row(
				intl_->tr( "Bing News / Finance / Sports" ),
				intl_->tr( "Removes every built-in Bing-branded app." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Bing News / Finance / Sports" ), &bing_apps ) )
					{
						set_status( debloat_->set<BingApps>( bing_apps ) );
					}
				}
			);

			bool drawboard_pdf = debloat_->get<DrawboardPdf>( );

			settings_row(
				intl_->tr( "Drawboard PDF" ),
				intl_->tr( "Removes the Drawboard PDF annotation app." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Drawboard PDF" ), &drawboard_pdf ) )
					{
						set_status( debloat_->set<DrawboardPdf>( drawboard_pdf ) );
					}
				}
			);

			bool sway = debloat_->get<Sway>( );

			settings_row(
				intl_->tr( "Sway" ),
				intl_->tr( "Removes the Microsoft Sway app." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Sway" ), &sway ) )
					{
						set_status( debloat_->set<Sway>( sway ) );
					}
				}
			);

			bool cortana = debloat_->get<Cortana>( );

			settings_row(
				intl_->tr( "Cortana" ),
				intl_->tr( "Removes the Cortana app." ),
				switch_width, switch_height,
				Level::Medium, Level::Low,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Cortana" ), &cortana ) )
					{
						set_status( debloat_->set<Cortana>( cortana ) );
					}
				}
			);

			bool copilot = debloat_->get<Copilot>( );

			settings_row(
				intl_->tr( "Copilot" ),
				intl_->tr( "Removes the built-in Windows Copilot app." ),
				switch_width, switch_height,
				Level::Medium, Level::Low,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Copilot" ), &copilot ) )
					{
						set_status( debloat_->set<Copilot>( copilot ) );
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

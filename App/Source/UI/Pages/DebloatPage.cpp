#include "Stdafx.hpp"
#include "UI/Pages/DebloatPage.hpp"
#include "UI/Theme.hpp"
#include "UI/Fonts/IconsLucide.h"
#include "UI/Widgets/Section.hpp"
#include "UI/Widgets/SettingsRow.hpp"
#include "UI/Widgets/Switch.hpp"

namespace UI::Pages
{
	using namespace Widgets;
	using enum Modules::DebloatFeature;

	DebloatPage::DebloatPage( std::shared_ptr<Modules::DebloatModule> debloat )
		: debloat_( std::move( debloat ) )
	{
	}

	void DebloatPage::on_activate( )
	{
		debloat_->refresh_all( );
	}

	void DebloatPage::set_status( const Core::Status& status )
	{
		status_ = status ? std::string {} : std::string { Core::to_string( status.error( ) ) };
	}

	void DebloatPage::render( )
	{
		const float switch_width  = ImGui::GetFrameHeight( ) * switch_aspect;
		const float switch_height = ImGui::GetFrameHeight( );

		section( ICON_LC_PACKAGE, "Apps", "Built-in Windows apps you can remove.", [ & ]
		{
			bool weather = debloat_->get<BingWeather>( );

			settings_row(
				"Weather",
				"Removes the built-in Weather app for all users.",
				switch_width, switch_height,
				Level::Low, Level::Low,
				"On",
				[ & ]
				{
					if ( toggle_switch( "Weather", &weather ) )
					{
						set_status( debloat_->set<BingWeather>( weather ) );
					}
				}
			);

			bool get_help = debloat_->get<GetHelp>( );

			settings_row(
				"Get Help",
				"Removes the Get Help support app.",
				switch_width, switch_height,
				Level::Low, Level::Low,
				"On",
				[ & ]
				{
					if ( toggle_switch( "Get Help", &get_help ) )
					{
						set_status( debloat_->set<GetHelp>( get_help ) );
					}
				}
			);

			bool tips = debloat_->get<GetStarted>( );

			settings_row(
				"Tips",
				"Removes the Tips (Get Started) app.",
				switch_width, switch_height,
				Level::Low, Level::Low,
				"On",
				[ & ]
				{
					if ( toggle_switch( "Tips", &tips ) )
					{
						set_status( debloat_->set<GetStarted>( tips ) );
					}
				}
			);

			bool heif_extension = debloat_->get<HeifImageExtension>( );

			settings_row(
				"HEIF Image Extension",
				"Removes the HEIF image codec extension.",
				switch_width, switch_height,
				Level::Low, Level::Medium,
				"On",
				[ & ]
				{
					if ( toggle_switch( "HEIF Image Extension", &heif_extension ) )
					{
						set_status( debloat_->set<HeifImageExtension>( heif_extension ) );
					}
				}
			);

			bool messaging = debloat_->get<Messaging>( );

			settings_row(
				"Messaging",
				"Removes the legacy Messaging app.",
				switch_width, switch_height,
				Level::Low, Level::Low,
				"On",
				[ & ]
				{
					if ( toggle_switch( "Messaging", &messaging ) )
					{
						set_status( debloat_->set<Messaging>( messaging ) );
					}
				}
			);

			bool viewer_3d = debloat_->get<Microsoft3DViewer>( );

			settings_row(
				"3D Viewer",
				"Removes the 3D Viewer app.",
				switch_width, switch_height,
				Level::Low, Level::Low,
				"On",
				[ & ]
				{
					if ( toggle_switch( "3D Viewer", &viewer_3d ) )
					{
						set_status( debloat_->set<Microsoft3DViewer>( viewer_3d ) );
					}
				}
			);

			bool solitaire = debloat_->get<SolitaireCollection>( );

			settings_row(
				"Solitaire Collection",
				"Removes the Microsoft Solitaire Collection game.",
				switch_width, switch_height,
				Level::Low, Level::Low,
				"On",
				[ & ]
				{
					if ( toggle_switch( "Solitaire Collection", &solitaire ) )
					{
						set_status( debloat_->set<SolitaireCollection>( solitaire ) );
					}
				}
			);

			bool sticky_notes = debloat_->get<StickyNotes>( );

			settings_row(
				"Sticky Notes",
				"Removes the built-in Sticky Notes app.",
				switch_width, switch_height,
				Level::Low, Level::Medium,
				"On",
				[ & ]
				{
					if ( toggle_switch( "Sticky Notes", &sticky_notes ) )
					{
						set_status( debloat_->set<StickyNotes>( sticky_notes ) );
					}
				}
			);

			bool mixed_reality = debloat_->get<MixedRealityPortal>( );

			settings_row(
				"Mixed Reality Portal",
				"Removes the Windows Mixed Reality Portal.",
				switch_width, switch_height,
				Level::Medium, Level::Low,
				"On",
				[ & ]
				{
					if ( toggle_switch( "Mixed Reality Portal", &mixed_reality ) )
					{
						set_status( debloat_->set<MixedRealityPortal>( mixed_reality ) );
					}
				}
			);

			bool mobile_plans = debloat_->get<OneConnect>( );

			settings_row(
				"Mobile Plans",
				"Removes the Mobile Plans (OneConnect) app.",
				switch_width, switch_height,
				Level::Low, Level::Low,
				"On",
				[ & ]
				{
					if ( toggle_switch( "Mobile Plans", &mobile_plans ) )
					{
						set_status( debloat_->set<OneConnect>( mobile_plans ) );
					}
				}
			);

			bool people = debloat_->get<People>( );

			settings_row(
				"People",
				"Removes the People contacts app.",
				switch_width, switch_height,
				Level::Low, Level::Low,
				"On",
				[ & ]
				{
					if ( toggle_switch( "People", &people ) )
					{
						set_status( debloat_->set<People>( people ) );
					}
				}
			);

			bool print_3d = debloat_->get<Print3D>( );

			settings_row(
				"Print 3D",
				"Removes the Print 3D app.",
				switch_width, switch_height,
				Level::Low, Level::Low,
				"On",
				[ & ]
				{
					if ( toggle_switch( "Print 3D", &print_3d ) )
					{
						set_status( debloat_->set<Print3D>( print_3d ) );
					}
				}
			);

			bool skype = debloat_->get<SkypeApp>( );

			settings_row(
				"Skype",
				"Removes the built-in Skype app.",
				switch_width, switch_height,
				Level::Low, Level::Low,
				"On",
				[ & ]
				{
					if ( toggle_switch( "Skype", &skype ) )
					{
						set_status( debloat_->set<SkypeApp>( skype ) );
					}
				}
			);

			bool web_media_extensions = debloat_->get<WebMediaExtensions>( );

			settings_row(
				"Web Media Extensions",
				"Removes the Web Media Extensions codec pack.",
				switch_width, switch_height,
				Level::Low, Level::Medium,
				"On",
				[ & ]
				{
					if ( toggle_switch( "Web Media Extensions", &web_media_extensions ) )
					{
						set_status( debloat_->set<WebMediaExtensions>( web_media_extensions ) );
					}
				}
			);

			bool webp_extension = debloat_->get<WebpImageExtension>( );

			settings_row(
				"WebP Image Extension",
				"Removes the WebP image codec extension.",
				switch_width, switch_height,
				Level::Low, Level::Medium,
				"On",
				[ & ]
				{
					if ( toggle_switch( "WebP Image Extension", &webp_extension ) )
					{
						set_status( debloat_->set<WebpImageExtension>( webp_extension ) );
					}
				}
			);

			bool alarms = debloat_->get<WindowsAlarms>( );

			settings_row(
				"Alarms & Clock",
				"Removes the Alarms & Clock app.",
				switch_width, switch_height,
				Level::Low, Level::Low,
				"On",
				[ & ]
				{
					if ( toggle_switch( "Alarms & Clock", &alarms ) )
					{
						set_status( debloat_->set<WindowsAlarms>( alarms ) );
					}
				}
			);

			bool camera = debloat_->get<WindowsCamera>( );

			settings_row(
				"Camera",
				"Removes the built-in Camera app.",
				switch_width, switch_height,
				Level::Low, Level::Medium,
				"On",
				[ & ]
				{
					if ( toggle_switch( "Camera", &camera ) )
					{
						set_status( debloat_->set<WindowsCamera>( camera ) );
					}
				}
			);

			bool feedback_hub = debloat_->get<FeedbackHub>( );

			settings_row(
				"Feedback Hub",
				"Removes the Feedback Hub app.",
				switch_width, switch_height,
				Level::Low, Level::Low,
				"On",
				[ & ]
				{
					if ( toggle_switch( "Feedback Hub", &feedback_hub ) )
					{
						set_status( debloat_->set<FeedbackHub>( feedback_hub ) );
					}
				}
			);

			bool maps = debloat_->get<WindowsMaps>( );

			settings_row(
				"Maps",
				"Removes the built-in Maps app and offline map data.",
				switch_width, switch_height,
				Level::Medium, Level::Low,
				"On",
				[ & ]
				{
					if ( toggle_switch( "Maps", &maps ) )
					{
						set_status( debloat_->set<WindowsMaps>( maps ) );
					}
				}
			);

			bool voice_recorder = debloat_->get<SoundRecorder>( );

			settings_row(
				"Voice Recorder",
				"Removes the Voice Recorder app.",
				switch_width, switch_height,
				Level::Low, Level::Low,
				"On",
				[ & ]
				{
					if ( toggle_switch( "Voice Recorder", &voice_recorder ) )
					{
						set_status( debloat_->set<SoundRecorder>( voice_recorder ) );
					}
				}
			);

			bool phone_link = debloat_->get<YourPhone>( );

			settings_row(
				"Phone Link",
				"Removes the Phone Link (Your Phone) companion app.",
				switch_width, switch_height,
				Level::Low, Level::Medium,
				"On",
				[ & ]
				{
					if ( toggle_switch( "Phone Link", &phone_link ) )
					{
						set_status( debloat_->set<YourPhone>( phone_link ) );
					}
				}
			);

			bool groove_music = debloat_->get<ZuneMusic>( );

			settings_row(
				"Media Player (Groove Music)",
				"Removes the legacy Groove Music app.",
				switch_width, switch_height,
				Level::Low, Level::Low,
				"On",
				[ & ]
				{
					if ( toggle_switch( "Media Player (Groove Music)", &groove_music ) )
					{
						set_status( debloat_->set<ZuneMusic>( groove_music ) );
					}
				}
			);

			bool mail_and_calendar = debloat_->get<MailAndCalendar>( );

			settings_row(
				"Mail and Calendar",
				"Removes the built-in Mail and Calendar apps.",
				switch_width, switch_height,
				Level::Medium, Level::Medium,
				"On",
				[ & ]
				{
					if ( toggle_switch( "Mail and Calendar", &mail_and_calendar ) )
					{
						set_status( debloat_->set<MailAndCalendar>( mail_and_calendar ) );
					}
				}
			);

			bool bing_apps = debloat_->get<BingApps>( );

			settings_row(
				"Bing News / Finance / Sports",
				"Removes every built-in Bing-branded app.",
				switch_width, switch_height,
				Level::Low, Level::Low,
				"On",
				[ & ]
				{
					if ( toggle_switch( "Bing News / Finance / Sports", &bing_apps ) )
					{
						set_status( debloat_->set<BingApps>( bing_apps ) );
					}
				}
			);

			bool drawboard_pdf = debloat_->get<DrawboardPdf>( );

			settings_row(
				"Drawboard PDF",
				"Removes the Drawboard PDF annotation app.",
				switch_width, switch_height,
				Level::Low, Level::Low,
				"On",
				[ & ]
				{
					if ( toggle_switch( "Drawboard PDF", &drawboard_pdf ) )
					{
						set_status( debloat_->set<DrawboardPdf>( drawboard_pdf ) );
					}
				}
			);

			bool sway = debloat_->get<Sway>( );

			settings_row(
				"Sway",
				"Removes the Microsoft Sway app.",
				switch_width, switch_height,
				Level::Low, Level::Low,
				"On",
				[ & ]
				{
					if ( toggle_switch( "Sway", &sway ) )
					{
						set_status( debloat_->set<Sway>( sway ) );
					}
				}
			);

			bool cortana = debloat_->get<Cortana>( );

			settings_row(
				"Cortana",
				"Removes the Cortana app.",
				switch_width, switch_height,
				Level::Medium, Level::Low,
				"On",
				[ & ]
				{
					if ( toggle_switch( "Cortana", &cortana ) )
					{
						set_status( debloat_->set<Cortana>( cortana ) );
					}
				}
			);

			bool copilot = debloat_->get<Copilot>( );

			settings_row(
				"Copilot",
				"Removes the built-in Windows Copilot app.",
				switch_width, switch_height,
				Level::Medium, Level::Low,
				"On",
				[ & ]
				{
					if ( toggle_switch( "Copilot", &copilot ) )
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

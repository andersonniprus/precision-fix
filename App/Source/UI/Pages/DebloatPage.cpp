#include "Stdafx.hpp"
#include "UI/Pages/DebloatPage.hpp"
#include "UI/Widgets/SettingsRow.hpp"
#include "UI/Widgets/Switch.hpp"

namespace UI::Pages
{
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
		if ( ImGui::CollapsingHeader( "Apps", ImGuiTreeNodeFlags_DefaultOpen ) )
		{
			const float switch_width  = ImGui::GetFrameHeight( ) * Widgets::switch_aspect;
			const float switch_height = ImGui::GetFrameHeight( );

			bool weather = debloat_->get<Modules::DebloatFeature::BingWeather>( );

			Widgets::settings_row(
				"Weather",
				"Removes the built-in Weather app for all users.",
				switch_width,
				switch_height,
				Widgets::Level::Low, Widgets::Level::Low,
				[ & ]
				{
					if ( Widgets::toggle_switch( "Weather", &weather ) )
						set_status( debloat_->set<Modules::DebloatFeature::BingWeather>( weather ) );
				}
			);

			ImGui::Separator( );

			bool get_help = debloat_->get<Modules::DebloatFeature::GetHelp>( );

			Widgets::settings_row(
				"Get Help",
				"Removes the Get Help support app.",
				switch_width,
				switch_height,
				Widgets::Level::Low, Widgets::Level::Low,
				[ & ]
				{
					if ( Widgets::toggle_switch( "Get Help", &get_help ) )
						set_status( debloat_->set<Modules::DebloatFeature::GetHelp>( get_help ) );
				}
			);

			ImGui::Separator( );

			bool tips = debloat_->get<Modules::DebloatFeature::GetStarted>( );

			Widgets::settings_row(
				"Tips",
				"Removes the Tips (Get Started) app.",
				switch_width,
				switch_height,
				Widgets::Level::Low, Widgets::Level::Low,
				[ & ]
				{
					if ( Widgets::toggle_switch( "Tips", &tips ) )
						set_status( debloat_->set<Modules::DebloatFeature::GetStarted>( tips ) );
				}
			);

			ImGui::Separator( );

			bool heif_extension = debloat_->get<Modules::DebloatFeature::HeifImageExtension>( );

			Widgets::settings_row(
				"HEIF Image Extension",
				"Removes the HEIF image codec extension.",
				switch_width,
				switch_height,
				Widgets::Level::Low, Widgets::Level::Medium,
				[ & ]
				{
					if ( Widgets::toggle_switch( "HEIF Image Extension", &heif_extension ) )
						set_status( debloat_->set<Modules::DebloatFeature::HeifImageExtension>( heif_extension ) );
				}
			);

			ImGui::Separator( );

			bool messaging = debloat_->get<Modules::DebloatFeature::Messaging>( );

			Widgets::settings_row(
				"Messaging",
				"Removes the legacy Messaging app.",
				switch_width,
				switch_height,
				Widgets::Level::Low, Widgets::Level::Low,
				[ & ]
				{
					if ( Widgets::toggle_switch( "Messaging", &messaging ) )
						set_status( debloat_->set<Modules::DebloatFeature::Messaging>( messaging ) );
				}
			);

			ImGui::Separator( );

			bool viewer_3d = debloat_->get<Modules::DebloatFeature::Microsoft3DViewer>( );

			Widgets::settings_row(
				"3D Viewer",
				"Removes the 3D Viewer app.",
				switch_width,
				switch_height,
				Widgets::Level::Low, Widgets::Level::Low,
				[ & ]
				{
					if ( Widgets::toggle_switch( "3D Viewer", &viewer_3d ) )
						set_status( debloat_->set<Modules::DebloatFeature::Microsoft3DViewer>( viewer_3d ) );
				}
			);

			ImGui::Separator( );

			bool solitaire = debloat_->get<Modules::DebloatFeature::SolitaireCollection>( );

			Widgets::settings_row(
				"Solitaire Collection",
				"Removes the Microsoft Solitaire Collection game.",
				switch_width,
				switch_height,
				Widgets::Level::Low, Widgets::Level::Low,
				[ & ]
				{
					if ( Widgets::toggle_switch( "Solitaire Collection", &solitaire ) )
						set_status( debloat_->set<Modules::DebloatFeature::SolitaireCollection>( solitaire ) );
				}
			);

			ImGui::Separator( );

			bool sticky_notes = debloat_->get<Modules::DebloatFeature::StickyNotes>( );

			Widgets::settings_row(
				"Sticky Notes",
				"Removes the built-in Sticky Notes app.",
				switch_width,
				switch_height,
				Widgets::Level::Low, Widgets::Level::Medium,
				[ & ]
				{
					if ( Widgets::toggle_switch( "Sticky Notes", &sticky_notes ) )
						set_status( debloat_->set<Modules::DebloatFeature::StickyNotes>( sticky_notes ) );
				}
			);

			ImGui::Separator( );

			bool mixed_reality = debloat_->get<Modules::DebloatFeature::MixedRealityPortal>( );

			Widgets::settings_row(
				"Mixed Reality Portal",
				"Removes the Windows Mixed Reality Portal.",
				switch_width,
				switch_height,
				Widgets::Level::Medium, Widgets::Level::Low,
				[ & ]
				{
					if ( Widgets::toggle_switch( "Mixed Reality Portal", &mixed_reality ) )
						set_status( debloat_->set<Modules::DebloatFeature::MixedRealityPortal>( mixed_reality ) );
				}
			);

			ImGui::Separator( );

			bool mobile_plans = debloat_->get<Modules::DebloatFeature::OneConnect>( );

			Widgets::settings_row(
				"Mobile Plans",
				"Removes the Mobile Plans (OneConnect) app.",
				switch_width,
				switch_height,
				Widgets::Level::Low, Widgets::Level::Low,
				[ & ]
				{
					if ( Widgets::toggle_switch( "Mobile Plans", &mobile_plans ) )
						set_status( debloat_->set<Modules::DebloatFeature::OneConnect>( mobile_plans ) );
				}
			);

			ImGui::Separator( );

			bool people = debloat_->get<Modules::DebloatFeature::People>( );

			Widgets::settings_row(
				"People",
				"Removes the People contacts app.",
				switch_width,
				switch_height,
				Widgets::Level::Low, Widgets::Level::Low,
				[ & ]
				{
					if ( Widgets::toggle_switch( "People", &people ) )
						set_status( debloat_->set<Modules::DebloatFeature::People>( people ) );
				}
			);

			ImGui::Separator( );

			bool print_3d = debloat_->get<Modules::DebloatFeature::Print3D>( );

			Widgets::settings_row(
				"Print 3D",
				"Removes the Print 3D app.",
				switch_width,
				switch_height,
				Widgets::Level::Low, Widgets::Level::Low,
				[ & ]
				{
					if ( Widgets::toggle_switch( "Print 3D", &print_3d ) )
						set_status( debloat_->set<Modules::DebloatFeature::Print3D>( print_3d ) );
				}
			);

			ImGui::Separator( );

			bool skype = debloat_->get<Modules::DebloatFeature::SkypeApp>( );

			Widgets::settings_row(
				"Skype",
				"Removes the built-in Skype app.",
				switch_width,
				switch_height,
				Widgets::Level::Low, Widgets::Level::Low,
				[ & ]
				{
					if ( Widgets::toggle_switch( "Skype", &skype ) )
						set_status( debloat_->set<Modules::DebloatFeature::SkypeApp>( skype ) );
				}
			);

			ImGui::Separator( );

			bool web_media_extensions = debloat_->get<Modules::DebloatFeature::WebMediaExtensions>( );

			Widgets::settings_row(
				"Web Media Extensions",
				"Removes the Web Media Extensions codec pack.",
				switch_width,
				switch_height,
				Widgets::Level::Low, Widgets::Level::Medium,
				[ & ]
				{
					if ( Widgets::toggle_switch( "Web Media Extensions", &web_media_extensions ) )
						set_status( debloat_->set<Modules::DebloatFeature::WebMediaExtensions>( web_media_extensions ) );
				}
			);

			ImGui::Separator( );

			bool webp_extension = debloat_->get<Modules::DebloatFeature::WebpImageExtension>( );

			Widgets::settings_row(
				"WebP Image Extension",
				"Removes the WebP image codec extension.",
				switch_width,
				switch_height,
				Widgets::Level::Low, Widgets::Level::Medium,
				[ & ]
				{
					if ( Widgets::toggle_switch( "WebP Image Extension", &webp_extension ) )
						set_status( debloat_->set<Modules::DebloatFeature::WebpImageExtension>( webp_extension ) );
				}
			);

			ImGui::Separator( );

			bool alarms = debloat_->get<Modules::DebloatFeature::WindowsAlarms>( );

			Widgets::settings_row(
				"Alarms & Clock",
				"Removes the Alarms & Clock app.",
				switch_width,
				switch_height,
				Widgets::Level::Low, Widgets::Level::Low,
				[ & ]
				{
					if ( Widgets::toggle_switch( "Alarms & Clock", &alarms ) )
						set_status( debloat_->set<Modules::DebloatFeature::WindowsAlarms>( alarms ) );
				}
			);

			ImGui::Separator( );

			bool camera = debloat_->get<Modules::DebloatFeature::WindowsCamera>( );

			Widgets::settings_row(
				"Camera",
				"Removes the built-in Camera app.",
				switch_width,
				switch_height,
				Widgets::Level::Low, Widgets::Level::Medium,
				[ & ]
				{
					if ( Widgets::toggle_switch( "Camera", &camera ) )
						set_status( debloat_->set<Modules::DebloatFeature::WindowsCamera>( camera ) );
				}
			);

			ImGui::Separator( );

			bool feedback_hub = debloat_->get<Modules::DebloatFeature::FeedbackHub>( );

			Widgets::settings_row(
				"Feedback Hub",
				"Removes the Feedback Hub app.",
				switch_width,
				switch_height,
				Widgets::Level::Low, Widgets::Level::Low,
				[ & ]
				{
					if ( Widgets::toggle_switch( "Feedback Hub", &feedback_hub ) )
						set_status( debloat_->set<Modules::DebloatFeature::FeedbackHub>( feedback_hub ) );
				}
			);

			ImGui::Separator( );

			bool maps = debloat_->get<Modules::DebloatFeature::WindowsMaps>( );

			Widgets::settings_row(
				"Maps",
				"Removes the built-in Maps app and offline map data.",
				switch_width,
				switch_height,
				Widgets::Level::Medium, Widgets::Level::Low,
				[ & ]
				{
					if ( Widgets::toggle_switch( "Maps", &maps ) )
						set_status( debloat_->set<Modules::DebloatFeature::WindowsMaps>( maps ) );
				}
			);

			ImGui::Separator( );

			bool voice_recorder = debloat_->get<Modules::DebloatFeature::SoundRecorder>( );

			Widgets::settings_row(
				"Voice Recorder",
				"Removes the Voice Recorder app.",
				switch_width,
				switch_height,
				Widgets::Level::Low, Widgets::Level::Low,
				[ & ]
				{
					if ( Widgets::toggle_switch( "Voice Recorder", &voice_recorder ) )
						set_status( debloat_->set<Modules::DebloatFeature::SoundRecorder>( voice_recorder ) );
				}
			);

			ImGui::Separator( );

			bool phone_link = debloat_->get<Modules::DebloatFeature::YourPhone>( );

			Widgets::settings_row(
				"Phone Link",
				"Removes the Phone Link (Your Phone) companion app.",
				switch_width,
				switch_height,
				Widgets::Level::Low, Widgets::Level::Medium,
				[ & ]
				{
					if ( Widgets::toggle_switch( "Phone Link", &phone_link ) )
						set_status( debloat_->set<Modules::DebloatFeature::YourPhone>( phone_link ) );
				}
			);

			ImGui::Separator( );

			bool groove_music = debloat_->get<Modules::DebloatFeature::ZuneMusic>( );

			Widgets::settings_row(
				"Media Player (Groove Music)",
				"Removes the legacy Groove Music app.",
				switch_width,
				switch_height,
				Widgets::Level::Low, Widgets::Level::Low,
				[ & ]
				{
					if ( Widgets::toggle_switch( "Media Player (Groove Music)", &groove_music ) )
						set_status( debloat_->set<Modules::DebloatFeature::ZuneMusic>( groove_music ) );
				}
			);

			ImGui::Separator( );

			bool mail_and_calendar = debloat_->get<Modules::DebloatFeature::MailAndCalendar>( );

			Widgets::settings_row(
				"Mail and Calendar",
				"Removes the built-in Mail and Calendar apps.",
				switch_width,
				switch_height,
				Widgets::Level::Medium, Widgets::Level::Medium,
				[ & ]
				{
					if ( Widgets::toggle_switch( "Mail and Calendar", &mail_and_calendar ) )
						set_status( debloat_->set<Modules::DebloatFeature::MailAndCalendar>( mail_and_calendar ) );
				}
			);

			ImGui::Separator( );

			bool bing_apps = debloat_->get<Modules::DebloatFeature::BingApps>( );

			Widgets::settings_row(
				"Bing News / Finance / Sports",
				"Removes every built-in Bing-branded app.",
				switch_width,
				switch_height,
				Widgets::Level::Low, Widgets::Level::Low,
				[ & ]
				{
					if ( Widgets::toggle_switch( "Bing News / Finance / Sports", &bing_apps ) )
						set_status( debloat_->set<Modules::DebloatFeature::BingApps>( bing_apps ) );
				}
			);

			ImGui::Separator( );

			bool drawboard_pdf = debloat_->get<Modules::DebloatFeature::DrawboardPdf>( );

			Widgets::settings_row(
				"Drawboard PDF",
				"Removes the Drawboard PDF annotation app.",
				switch_width,
				switch_height,
				Widgets::Level::Low, Widgets::Level::Low,
				[ & ]
				{
					if ( Widgets::toggle_switch( "Drawboard PDF", &drawboard_pdf ) )
						set_status( debloat_->set<Modules::DebloatFeature::DrawboardPdf>( drawboard_pdf ) );
				}
			);

			ImGui::Separator( );

			bool sway = debloat_->get<Modules::DebloatFeature::Sway>( );

			Widgets::settings_row(
				"Sway",
				"Removes the Microsoft Sway app.",
				switch_width,
				switch_height,
				Widgets::Level::Low, Widgets::Level::Low,
				[ & ]
				{
					if ( Widgets::toggle_switch( "Sway", &sway ) )
						set_status( debloat_->set<Modules::DebloatFeature::Sway>( sway ) );
				}
			);

			ImGui::Separator( );

			bool cortana = debloat_->get<Modules::DebloatFeature::Cortana>( );

			Widgets::settings_row(
				"Cortana",
				"Removes the Cortana app.",
				switch_width,
				switch_height,
				Widgets::Level::Medium, Widgets::Level::Low,
				[ & ]
				{
					if ( Widgets::toggle_switch( "Cortana", &cortana ) )
						set_status( debloat_->set<Modules::DebloatFeature::Cortana>( cortana ) );
				}
			);

			ImGui::Separator( );

			bool copilot = debloat_->get<Modules::DebloatFeature::Copilot>( );

			Widgets::settings_row(
				"Copilot",
				"Removes the built-in Windows Copilot app.",
				switch_width,
				switch_height,
				Widgets::Level::Medium, Widgets::Level::Low,
				[ & ]
				{
					if ( Widgets::toggle_switch( "Copilot", &copilot ) )
						set_status( debloat_->set<Modules::DebloatFeature::Copilot>( copilot ) );
				}
			);
		}

		if ( !status_.empty( ) )
			ImGui::TextColored( ImVec4( 1.f, 0.35f, 0.35f, 1.f ), "%s", status_.c_str( ) );
	}
}

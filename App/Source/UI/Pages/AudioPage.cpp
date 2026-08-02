#include "Stdafx.hpp"
#include "UI/Pages/AudioPage.hpp"
#include "UI/Theme.hpp"
#include "UI/Fonts/IconsLucide.h"
#include "UI/Widgets/Section.hpp"
#include "UI/Widgets/SettingsRow.hpp"
#include "UI/Widgets/Switch.hpp"

namespace UI::Pages
{
	using namespace Widgets;
	using enum Modules::AudioFeature;

	AudioPage::AudioPage( std::shared_ptr<Modules::AudioModule> audio )
		: audio_( std::move( audio ) )
	{
	}

	void AudioPage::on_activate( )
	{
		audio_->refresh_all( );
	}

	void AudioPage::set_status( const Core::Status& status )
	{
		status_ = status ? std::string {} : std::string { Core::to_string( status.error( ) ) };
	}

	void AudioPage::render( )
	{
		const float switch_width  = ImGui::GetFrameHeight( ) * switch_aspect;
		const float switch_height = ImGui::GetFrameHeight( );

		section( ICON_LC_VOLUME_2, "Playback", "System-wide audio effects.", [ & ]
		{
			bool enhancements = audio_->get<Enhancements>( );

			settings_row(
				"Audio enhancements",
				"System effects (APOs) applied to every active playback and recording device. Support varies by driver.",
				switch_width, switch_height,
				Level::Low, Level::Low,
				"Off",
				[ & ]
				{
					if ( toggle_switch( "Audio enhancements", &enhancements ) )
					{
						set_status( audio_->set<Enhancements>( enhancements ) );
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

#include "Stdafx.hpp"
#include "UI/Pages/AudioPage.hpp"
#include "UI/Widgets/SettingsRow.hpp"
#include "UI/Widgets/Switch.hpp"

namespace UI::Pages
{
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
		const float switch_width  = ImGui::GetFrameHeight( ) * Widgets::switch_aspect;
		const float switch_height = ImGui::GetFrameHeight( );

		bool enhancements = audio_->get<Modules::AudioFeature::Enhancements>( );

		Widgets::settings_row(
			"Audio enhancements",
			"System effects (APOs) applied to every active playback and recording device. Support varies by driver.",
			switch_width,
			switch_height,
			[ & ]
			{
				if ( Widgets::toggle_switch( "Audio enhancements", &enhancements ) )
					set_status( audio_->set<Modules::AudioFeature::Enhancements>( enhancements ) );
			}
		);

		if ( !status_.empty( ) )
			ImGui::TextColored( ImVec4( 1.f, 0.35f, 0.35f, 1.f ), "%s", status_.c_str( ) );
	}
}

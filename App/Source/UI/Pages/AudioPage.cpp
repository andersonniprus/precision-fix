#include "Stdafx.hpp"
#include "UI/Pages/AudioPage.hpp"
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
	using enum Modules::AudioFeature;

	AudioPage::AudioPage(
		std::shared_ptr<Modules::AudioModule> module,
		std::shared_ptr<App::Logger> logger,
		std::shared_ptr<App::Intl> intl
	)
		: audio_( std::move( module ) ),
		  logger_( std::move( logger ) ),
		  intl_( std::move( intl ) )
	{
	}

	void AudioPage::on_activate( )
	{
		audio_->refresh_all( );
	}

	void AudioPage::set_status( const Core::Status& status )
	{
		if ( status )
		{
			logger_->info( "Audio", "setting applied" );
			status_.clear( );
			return;
		}

		logger_->error( "Audio", std::string { Core::to_string( status.error( ) ) } );
		status_ = std::string { Core::to_string( status.error( ) ) };
	}


	void AudioPage::render( )
	{
		const float switch_width  = ImGui::GetFrameHeight( ) * switch_aspect;
		const float switch_height = ImGui::GetFrameHeight( );

		section( ICON_LC_VOLUME_2, intl_->tr( "Playback" ), intl_->tr( "System-wide audio effects." ), [ & ]
		{
			bool enhancements = audio_->get<Enhancements>( );

			settings_row(
				intl_->tr( "Audio enhancements" ),
				intl_->tr( "System effects (APOs) applied to every active playback and recording device. Support varies by driver." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "Off" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Audio enhancements" ), &enhancements ) )
					{
						set_status( audio_->set<Enhancements>( enhancements ) );
					}
				}
			);

			bool exclusive = audio_->get<ExclusiveMode>( );

			settings_row(
				intl_->tr( "Exclusive mode" ),
				intl_->tr( "Allows apps to take exclusive control of the endpoint (lower latency, no shared mixing)." ),
				switch_width, switch_height,
				Level::Medium, Level::Low,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Exclusive mode" ), &exclusive ) )
					{
						set_status( audio_->set<ExclusiveMode>( exclusive ) );
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

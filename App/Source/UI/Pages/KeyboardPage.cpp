#include "Stdafx.hpp"
#include "UI/Pages/KeyboardPage.hpp"
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
	using enum Modules::KeyboardFeature;
	using Modules::FeatureTraits;

	KeyboardPage::KeyboardPage(
		std::shared_ptr<Modules::KeyboardModule> module,
		std::shared_ptr<App::Logger> logger,
		std::shared_ptr<App::Intl> intl
	)
		: keyboard_( std::move( module ) ),
		  logger_( std::move( logger ) ),
		  intl_( std::move( intl ) )
	{
	}

	void KeyboardPage::on_activate( )
	{
		keyboard_->refresh_all( );
	}

	void KeyboardPage::set_status( const Core::Status& status )
	{
		if ( status )
		{
			logger_->info( "Keyboard", "setting applied" );
			status_.clear( );
			return;
		}

		logger_->error( "Keyboard", std::string { Core::to_string( status.error( ) ) } );
		status_ = std::string { Core::to_string( status.error( ) ) };
	}


	void KeyboardPage::render( )
	{
		const float switch_width   = ImGui::GetFrameHeight( ) * switch_aspect;
		const float switch_height  = ImGui::GetFrameHeight( );
		const float control_width  = preset_row_width( 3 );
		const float control_height = preset_button_height( );

		section( ICON_LC_SHIELD, intl_->tr( "Accessibility" ), intl_->tr( "Sticky, filter and mouse-key helpers." ), [ & ]
		{
			bool filter_keys = keyboard_->get<FilterKeys>( );

			settings_row(
				intl_->tr( "Filter Keys" ),
				intl_->tr( "Ignores brief or repeated keystrokes." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "Off" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Filter Keys" ), &filter_keys ) )
					{
						set_status( keyboard_->set<FilterKeys>( filter_keys ) );
					}
				}
			);

			bool sticky_keys = keyboard_->get<StickyKeys>( );

			settings_row(
				intl_->tr( "Sticky Keys" ),
				intl_->tr( "Lets modifier keys stay active without holding them down." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "Off" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Sticky Keys" ), &sticky_keys ) )
					{
						set_status( keyboard_->set<StickyKeys>( sticky_keys ) );
					}
				}
			);

			bool toggle_keys = keyboard_->get<ToggleKeys>( );

			settings_row(
				intl_->tr( "Toggle Keys" ),
				intl_->tr( "Plays a tone when Caps Lock, Num Lock or Scroll Lock changes." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "Off" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Toggle Keys" ), &toggle_keys ) )
					{
						set_status( keyboard_->set<ToggleKeys>( toggle_keys ) );
					}
				}
			);

			bool mouse_keys = keyboard_->get<MouseKeys>( );

			settings_row(
				intl_->tr( "Mouse Keys" ),
				intl_->tr( "Lets the numeric keypad move the mouse pointer." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "Off" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Mouse Keys" ), &mouse_keys ) )
					{
						set_status( keyboard_->set<MouseKeys>( mouse_keys ) );
					}
				}
			);
		} );

		section( ICON_LC_TIMER, intl_->tr( "Repeat" ), intl_->tr( "Key repeat delay and rate." ), [ & ]
		{
			ImGui::PushID( "Repeat delay" );
			settings_row(
				intl_->tr( "Repeat delay" ),
				intl_->tr( "Time before a held key starts repeating." ),
				control_width, control_height,
				Level::Low, Level::Low,
				"Fastest",
				[ & ]
				{
					if ( preset_button( intl_->tr( "Default" ), ButtonVariant::Ghost ) )
					{
						set_status( keyboard_->set<RepeatDelay>( FeatureTraits<RepeatDelay>::fallback( ) ) );
					}

					preset_same_line( );

					if ( preset_button( intl_->tr( "Slow" ), ButtonVariant::Ghost ) )
					{
						set_status( keyboard_->set<RepeatDelay>( 3u ) );
					}

					preset_same_line( );

					if ( preset_button( intl_->tr( "Fastest" ), ButtonVariant::Primary ) )
					{
						set_status( keyboard_->set<RepeatDelay>( 0u ) );
					}
				}
			);
			ImGui::PopID( );

			ImGui::PushID( "Repeat rate" );
			settings_row(
				intl_->tr( "Repeat rate" ),
				intl_->tr( "Speed of key repetition once it starts." ),
				control_width, control_height,
				Level::Low, Level::Low,
				"Fastest",
				[ & ]
				{
					if ( preset_button( intl_->tr( "Default" ), ButtonVariant::Ghost ) )
					{
						set_status( keyboard_->set<RepeatRate>( FeatureTraits<RepeatRate>::fallback( ) ) );
					}

					preset_same_line( );

					if ( preset_button( intl_->tr( "Slow" ), ButtonVariant::Ghost ) )
					{
						set_status( keyboard_->set<RepeatRate>( 0u ) );
					}

					preset_same_line( );

					if ( preset_button( intl_->tr( "Fastest" ), ButtonVariant::Primary ) )
					{
						set_status( keyboard_->set<RepeatRate>( 31u ) );
					}
				}
			);
			ImGui::PopID( );
		} );

		section( ICON_LC_POWER, intl_->tr( "Power" ), intl_->tr( "USB power management." ), [ & ]
		{
			bool selective_suspend = keyboard_->get<UsbSelectiveSuspend>( );

			settings_row(
				intl_->tr( "USB selective suspend" ),
				intl_->tr( "Lets Windows suspend idle USB devices to save power." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "Off" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "USB selective suspend" ), &selective_suspend ) )
					{
						set_status( keyboard_->set<UsbSelectiveSuspend>( selective_suspend ) );
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

#include "Stdafx.hpp"
#include "UI/Pages/KeyboardPage.hpp"
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

	KeyboardPage::KeyboardPage( std::shared_ptr<Modules::KeyboardModule> keyboard )
		: keyboard_( std::move( keyboard ) )
	{
	}

	void KeyboardPage::on_activate( )
	{
		keyboard_->refresh_all( );
	}

	void KeyboardPage::set_status( const Core::Status& status )
	{
		status_ = status ? std::string {} : std::string { Core::to_string( status.error( ) ) };
	}

	void KeyboardPage::render( )
	{
		const float switch_width   = ImGui::GetFrameHeight( ) * switch_aspect;
		const float switch_height  = ImGui::GetFrameHeight( );
		const float control_width  = preset_row_width( 3 );
		const float control_height = preset_button_height( );

		section( ICON_LC_SHIELD, "Accessibility", "Sticky, filter and mouse-key helpers.", [ & ]
		{
			bool filter_keys = keyboard_->get<FilterKeys>( );

			settings_row(
				"Filter Keys",
				"Ignores brief or repeated keystrokes.",
				switch_width, switch_height,
				Level::Low, Level::Low,
				"Off",
				[ & ]
				{
					if ( toggle_switch( "Filter Keys", &filter_keys ) )
					{
						set_status( keyboard_->set<FilterKeys>( filter_keys ) );
					}
				}
			);

			bool sticky_keys = keyboard_->get<StickyKeys>( );

			settings_row(
				"Sticky Keys",
				"Lets modifier keys stay active without holding them down.",
				switch_width, switch_height,
				Level::Low, Level::Low,
				"Off",
				[ & ]
				{
					if ( toggle_switch( "Sticky Keys", &sticky_keys ) )
					{
						set_status( keyboard_->set<StickyKeys>( sticky_keys ) );
					}
				}
			);

			bool toggle_keys = keyboard_->get<ToggleKeys>( );

			settings_row(
				"Toggle Keys",
				"Plays a tone when Caps Lock, Num Lock or Scroll Lock changes.",
				switch_width, switch_height,
				Level::Low, Level::Low,
				"Off",
				[ & ]
				{
					if ( toggle_switch( "Toggle Keys", &toggle_keys ) )
					{
						set_status( keyboard_->set<ToggleKeys>( toggle_keys ) );
					}
				}
			);

			bool mouse_keys = keyboard_->get<MouseKeys>( );

			settings_row(
				"Mouse Keys",
				"Lets the numeric keypad move the mouse pointer.",
				switch_width, switch_height,
				Level::Low, Level::Low,
				"Off",
				[ & ]
				{
					if ( toggle_switch( "Mouse Keys", &mouse_keys ) )
					{
						set_status( keyboard_->set<MouseKeys>( mouse_keys ) );
					}
				}
			);
		} );

		section( ICON_LC_TIMER, "Repeat", "Key repeat delay and rate.", [ & ]
		{
			ImGui::PushID( "Repeat delay" );
			settings_row(
				"Repeat delay",
				"Time before a held key starts repeating.",
				control_width, control_height,
				Level::Low, Level::Low,
				"Fastest",
				[ & ]
				{
					if ( preset_button( "Default", ButtonVariant::Ghost ) )
					{
						set_status( keyboard_->set<RepeatDelay>( FeatureTraits<RepeatDelay>::fallback( ) ) );
					}

					preset_same_line( );

					if ( preset_button( "Slow", ButtonVariant::Ghost ) )
					{
						set_status( keyboard_->set<RepeatDelay>( 3u ) );
					}

					preset_same_line( );

					if ( preset_button( "Fastest", ButtonVariant::Primary ) )
					{
						set_status( keyboard_->set<RepeatDelay>( 0u ) );
					}
				}
			);
			ImGui::PopID( );

			ImGui::PushID( "Repeat rate" );
			settings_row(
				"Repeat rate",
				"Speed of key repetition once it starts.",
				control_width, control_height,
				Level::Low, Level::Low,
				"Fastest",
				[ & ]
				{
					if ( preset_button( "Default", ButtonVariant::Ghost ) )
					{
						set_status( keyboard_->set<RepeatRate>( FeatureTraits<RepeatRate>::fallback( ) ) );
					}

					preset_same_line( );

					if ( preset_button( "Slow", ButtonVariant::Ghost ) )
					{
						set_status( keyboard_->set<RepeatRate>( 0u ) );
					}

					preset_same_line( );

					if ( preset_button( "Fastest", ButtonVariant::Primary ) )
					{
						set_status( keyboard_->set<RepeatRate>( 31u ) );
					}
				}
			);
			ImGui::PopID( );
		} );

		section( ICON_LC_POWER, "Power", "USB power management.", [ & ]
		{
			bool selective_suspend = keyboard_->get<UsbSelectiveSuspend>( );

			settings_row(
				"USB selective suspend",
				"Lets Windows suspend idle USB devices to save power.",
				switch_width, switch_height,
				Level::Low, Level::Low,
				"Off",
				[ & ]
				{
					if ( toggle_switch( "USB selective suspend", &selective_suspend ) )
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

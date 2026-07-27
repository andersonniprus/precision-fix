#include "Stdafx.hpp"
#include "UI/Pages/KeyboardPage.hpp"
#include "UI/Widgets/SettingsRow.hpp"
#include "UI/Widgets/Switch.hpp"

namespace UI::Pages
{
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

	void KeyboardPage::render_accessibility( )
	{
		const float switch_width  = ImGui::GetFrameHeight( ) * Widgets::switch_aspect;
		const float switch_height = ImGui::GetFrameHeight( );

		bool filter_keys = keyboard_->get<Modules::KeyboardFeature::FilterKeys>( );

		Widgets::settings_row(
			"Filter Keys",
			"Ignores brief or repeated keystrokes.",
			switch_width,
			switch_height,
			Widgets::Level::Low,
			Widgets::Level::Low,
			[ & ]
			{
				if ( Widgets::toggle_switch( "Filter Keys", &filter_keys ) )
					set_status( keyboard_->set<Modules::KeyboardFeature::FilterKeys>( filter_keys ) );
			}
		);

		ImGui::Separator( );

		bool sticky_keys = keyboard_->get<Modules::KeyboardFeature::StickyKeys>( );

		Widgets::settings_row(
			"Sticky Keys",
			"Lets modifier keys stay active without holding them down.",
			switch_width,
			switch_height,
			Widgets::Level::Low,
			Widgets::Level::Low,
			[ & ]
			{
				if ( Widgets::toggle_switch( "Sticky Keys", &sticky_keys ) )
					set_status( keyboard_->set<Modules::KeyboardFeature::StickyKeys>( sticky_keys ) );
			}
		);

		ImGui::Separator( );

		bool toggle_keys = keyboard_->get<Modules::KeyboardFeature::ToggleKeys>( );

		Widgets::settings_row(
			"Toggle Keys",
			"Plays a tone when Caps Lock, Num Lock or Scroll Lock changes.",
			switch_width,
			switch_height,
			Widgets::Level::Low,
			Widgets::Level::Low,
			[ & ]
			{
				if ( Widgets::toggle_switch( "Toggle Keys", &toggle_keys ) )
					set_status( keyboard_->set<Modules::KeyboardFeature::ToggleKeys>( toggle_keys ) );
			}
		);

		ImGui::Separator( );

		bool mouse_keys = keyboard_->get<Modules::KeyboardFeature::MouseKeys>( );

		Widgets::settings_row(
			"Mouse Keys",
			"Lets the numeric keypad move the mouse pointer.",
			switch_width,
			switch_height,
			Widgets::Level::Low,
			Widgets::Level::Low,
			[ & ]
			{
				if ( Widgets::toggle_switch( "Mouse Keys", &mouse_keys ) )
					set_status( keyboard_->set<Modules::KeyboardFeature::MouseKeys>( mouse_keys ) );
			}
		);
	}

	void KeyboardPage::render_repeat( )
	{
		const float row_width      = Widgets::preset_row_width( 3 );
		const float control_height = ImGui::GetFrameHeight( );

		ImGui::PushID( "Repeat delay" );

		Widgets::settings_row(
			"Repeat delay",
			"Time before a held key starts repeating.",
			row_width,
			control_height,
			Widgets::Level::Low,
			Widgets::Level::Low,
			[ & ]
			{
				if ( ImGui::Button( "Default", ImVec2( Widgets::preset_button_width, 0.f ) ) )
					set_status(
						keyboard_->set<Modules::KeyboardFeature::RepeatDelay>(
							Modules::FeatureTraits<Modules::KeyboardFeature::RepeatDelay>::fallback( )
						)
					);

				ImGui::SameLine( );

				if ( ImGui::Button( "Slow", ImVec2( Widgets::preset_button_width, 0.f ) ) )
					set_status( keyboard_->set<Modules::KeyboardFeature::RepeatDelay>( 3u ) );

				ImGui::SameLine( );

				if ( ImGui::Button( "Fastest", ImVec2( Widgets::preset_button_width, 0.f ) ) )
					set_status( keyboard_->set<Modules::KeyboardFeature::RepeatDelay>( 0u ) );
			}
		);

		ImGui::PopID( );

		ImGui::Separator( );

		ImGui::PushID( "Repeat rate" );

		Widgets::settings_row(
			"Repeat rate",
			"Speed of key repetition once it starts.",
			row_width,
			control_height,
			Widgets::Level::Low,
			Widgets::Level::Low,
			[ & ]
			{
				if ( ImGui::Button( "Default", ImVec2( Widgets::preset_button_width, 0.f ) ) )
					set_status( keyboard_->set<Modules::KeyboardFeature::RepeatRate>(
						Modules::FeatureTraits<Modules::KeyboardFeature::RepeatRate>::fallback( ) ) );

				ImGui::SameLine( );

				if ( ImGui::Button( "Slow", ImVec2( Widgets::preset_button_width, 0.f ) ) )
					set_status( keyboard_->set<Modules::KeyboardFeature::RepeatRate>( 0u ) );

				ImGui::SameLine( );

				if ( ImGui::Button( "Fastest", ImVec2( Widgets::preset_button_width, 0.f ) ) )
					set_status( keyboard_->set<Modules::KeyboardFeature::RepeatRate>( 31u ) );
			}
		);

		ImGui::PopID( );
	}

	void KeyboardPage::render_power( )
	{
		const float switch_width  = ImGui::GetFrameHeight( ) * Widgets::switch_aspect;
		const float switch_height = ImGui::GetFrameHeight( );

		bool selective_suspend = keyboard_->get<Modules::KeyboardFeature::UsbSelectiveSuspend>( );

		Widgets::settings_row(
			"USB selective suspend",
			"Lets Windows suspend idle USB devices to save power.",
			switch_width,
			switch_height,
			Widgets::Level::Low,
			Widgets::Level::Low,
			[ & ]
			{
				if ( Widgets::toggle_switch( "USB selective suspend", &selective_suspend ) )
					set_status( keyboard_->set<Modules::KeyboardFeature::UsbSelectiveSuspend>( selective_suspend ) );
			}
		);
	}

	void KeyboardPage::render( )
	{
		if ( ImGui::CollapsingHeader( "Accessibility", ImGuiTreeNodeFlags_DefaultOpen ) )
			render_accessibility( );

		if ( ImGui::CollapsingHeader( "Repeat", ImGuiTreeNodeFlags_DefaultOpen ) )
			render_repeat( );

		if ( ImGui::CollapsingHeader( "Power", ImGuiTreeNodeFlags_DefaultOpen ) )
			render_power( );

		if ( !status_.empty( ) )
			ImGui::TextColored( ImVec4( 1.f, 0.35f, 0.35f, 1.f ), "%s", status_.c_str( ) );
	}
}

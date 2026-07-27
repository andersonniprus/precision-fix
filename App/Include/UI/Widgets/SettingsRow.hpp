#pragma once

namespace UI::Widgets
{
	inline constexpr float preset_button_width = 90.f;

	[[nodiscard]] inline float preset_row_width( const int count )
	{
		return preset_button_width * static_cast<float>( count )
		       + ImGui::GetStyle( ).ItemSpacing.x * static_cast<float>( count - 1 );
	}

	enum class Level : std::uint8_t
	{
		Low,
		Medium,
		High,
	};

	namespace Detail
	{
		inline constexpr float gain_impact_bar_width = 4.f;
		inline constexpr float gain_impact_gutter    = gain_impact_bar_width + 8.f;

		[[nodiscard]] inline ImU32 gain_color( const Level level ) noexcept
		{
			switch ( level )
			{
				case Level::Low:
					return IM_COL32( 110, 112, 118, 255 );

				case Level::Medium:
					return IM_COL32( 227, 176, 60, 255 );

				case Level::High:
					return IM_COL32( 88, 196, 112, 255 );
			}

			return IM_COL32( 110, 112, 118, 255 );
		}

		[[nodiscard]] inline ImU32 impact_color( const Level level ) noexcept
		{
			switch ( level )
			{
				case Level::Low:
					return IM_COL32( 88, 196, 112, 255 );

				case Level::Medium:
					return IM_COL32( 227, 176, 60, 255 );

				case Level::High:
					return IM_COL32( 222, 82, 82, 255 );
			}

			return IM_COL32( 88, 196, 112, 255 );
		}

		[[nodiscard]] inline const char* level_label( const Level level ) noexcept
		{
			switch ( level )
			{
				case Level::Low:
					return "Low";

				case Level::Medium:
					return "Medium";

				case Level::High:
					return "High";
			}

			return "Low";
		}
	}

	template<typename ControlFn>
	void settings_row(
		const char* title,
		const char* description,
		const float control_width,
		const float control_height,
		const Level gain,
		const Level impact,
		ControlFn&& control
	)
	{
		const ImVec2 start        = ImGui::GetCursorPos( );
		const ImVec2 screen_start = ImGui::GetCursorScreenPos( );
		const float avail         = ImGui::GetContentRegionAvail( ).x;

		ImGui::SetCursorPosX( start.x + Detail::gain_impact_gutter );

		const float text_width = avail - Detail::gain_impact_gutter - control_width - ImGui::GetStyle( ).ItemSpacing.x;

		ImGui::PushTextWrapPos( ImGui::GetCursorPos( ).x + text_width );
		ImGui::TextUnformatted( title );

		ImGui::SetCursorPosX( start.x + Detail::gain_impact_gutter );
		ImGui::PushStyleColor( ImGuiCol_Text, ImGui::GetStyle( ).Colors[ ImGuiCol_TextDisabled ] );
		ImGui::TextWrapped( "%s", description );
		ImGui::PopStyleColor( );
		ImGui::PopTextWrapPos( );

		const float row_height  = ImGui::GetCursorPosY( ) - start.y;
		const float slot_height = row_height > control_height ? row_height : control_height;

		ImGui::SetCursorPos(
			ImVec2( start.x + Detail::gain_impact_gutter + text_width + ImGui::GetStyle( ).ItemSpacing.x,
			        start.y + ( slot_height - control_height ) * 0.5f )
		);
		control( );

		ImDrawList* draw_list = ImGui::GetWindowDrawList( );
		const float mid_y     = screen_start.y + slot_height * 0.5f;

		draw_list->AddRectFilled(
			screen_start, ImVec2( screen_start.x + Detail::gain_impact_bar_width, mid_y ), Detail::gain_color( gain )
		);
		draw_list->AddRectFilled(
			ImVec2( screen_start.x, mid_y ),
			ImVec2( screen_start.x + Detail::gain_impact_bar_width, screen_start.y + slot_height ),
			Detail::impact_color( impact )
		);

		ImGui::PushID( title );
		ImGui::PushID( "GainImpactBar" );
		ImGui::SetCursorScreenPos( screen_start );
		ImGui::InvisibleButton( "##bar", ImVec2( Detail::gain_impact_bar_width, slot_height ) );

		if ( ImGui::IsItemHovered( ) )
		{
			ImGui::BeginTooltip( );
			ImGui::Text( "Performance gain: %s", Detail::level_label( gain ) );
			ImGui::Text( "System impact: %s", Detail::level_label( impact ) );
			ImGui::EndTooltip( );
		}

		ImGui::PopID( );
		ImGui::PopID( );

		ImGui::SetCursorPos( ImVec2( start.x, start.y + slot_height ) );
		ImGui::Dummy( ImVec2( 0.f, 0.f ) );
	}
}

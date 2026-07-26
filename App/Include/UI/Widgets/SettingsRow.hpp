#pragma once

namespace UI::Widgets
{
	inline constexpr float preset_button_width = 90.f;

	[[nodiscard]] inline float preset_row_width( const int count )
	{
		return preset_button_width * static_cast<float>( count )
		       + ImGui::GetStyle( ).ItemSpacing.x * static_cast<float>( count - 1 );
	}

	template<typename ControlFn>
	void settings_row(
		const char* title,
		const char* description,
		const float control_width,
		const float control_height,
		ControlFn&& control
	)
	{
		const ImVec2 start = ImGui::GetCursorPos( );
		const float avail  = ImGui::GetContentRegionAvail( ).x;

		const float text_width = avail - control_width - ImGui::GetStyle( ).ItemSpacing.x;

		ImGui::PushTextWrapPos( ImGui::GetCursorPos( ).x + text_width );
		ImGui::TextUnformatted( title );

		ImGui::PushStyleColor( ImGuiCol_Text, ImGui::GetStyle( ).Colors[ ImGuiCol_TextDisabled ] );
		ImGui::TextWrapped( "%s", description );
		ImGui::PopStyleColor( );
		ImGui::PopTextWrapPos( );

		const float row_height  = ImGui::GetCursorPosY( ) - start.y;
		const float slot_height = row_height > control_height ? row_height : control_height;

		ImGui::SetCursorPos( ImVec2( start.x + text_width + ImGui::GetStyle( ).ItemSpacing.x, start.y + ( slot_height - control_height ) * 0.5f ) );
		control( );

		ImGui::SetCursorPos( ImVec2( start.x, start.y + slot_height ) );
		ImGui::Dummy( ImVec2( 0.f, 0.f ) );
	}
}

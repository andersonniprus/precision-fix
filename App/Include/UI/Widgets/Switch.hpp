#pragma once

namespace UI::Widgets
{
	inline constexpr float switch_aspect = 1.75f;

	inline bool toggle_switch( const char* id, bool* value )
	{
		static std::unordered_map<ImGuiID, float> progress_;

		const float height = ImGui::GetFrameHeight( );
		const float width  = height * switch_aspect;
		const float radius = height * 0.5f;

		const ImVec2 pos = ImGui::GetCursorScreenPos( );

		const bool clicked = ImGui::InvisibleButton( id, ImVec2( width, height ) );

		if ( clicked )
			*value = !*value;

		float& t = progress_[ ImGui::GetID( id ) ];

		const float target = *value ? 1.f : 0.f;
		const float speed  = ImGui::GetIO( ).DeltaTime * 12.f;

		t += ( target - t ) * ( speed < 1.f ? speed : 1.f );

		const ImVec4 off_col = ImGui::GetStyle( ).Colors[ ImGuiCol_FrameBg ];
		const ImVec4 on_col  = ImGui::GetStyle( ).Colors[ ImGuiCol_ButtonActive ];

		const ImVec4 track_col(
			off_col.x + ( on_col.x - off_col.x ) * t,
			off_col.y + ( on_col.y - off_col.y ) * t,
			off_col.z + ( on_col.z - off_col.z ) * t,
			off_col.w + ( on_col.w - off_col.w ) * t
		);

		ImDrawList* draw_list = ImGui::GetWindowDrawList( );
		const ImVec2 p_max( pos.x + width, pos.y + height );

		draw_list->AddRectFilled( pos, p_max, ImGui::ColorConvertFloat4ToU32( track_col ), radius );

		const float knob_x = pos.x + radius + t * ( width - height );
		draw_list->AddCircleFilled( ImVec2( knob_x, pos.y + radius ), radius - 2.f, IM_COL32( 235, 235, 235, 255 ) );

		return clicked;
	}
}

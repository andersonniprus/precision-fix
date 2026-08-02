#pragma once

#include "UI/Theme.hpp"

namespace UI::Widgets
{
	inline constexpr float switch_aspect = 1.75f;

	inline bool toggle_switch( const char* id, bool* value )
	{
		struct Anim
		{
			float t = 0.f;
			float v = 0.f;
		};

		static std::unordered_map<ImGuiID, Anim> anim_;

		const float h = ImGui::GetFrameHeight( );
		const float w = h * switch_aspect;
		const float r = h * 0.5f;

		const ImVec2 pos = ImGui::GetCursorScreenPos( );

		const bool clicked = ImGui::InvisibleButton( id, ImVec2( w, h ) );
		const bool hovered = ImGui::IsItemHovered( );
		const bool active  = ImGui::IsItemActive( );

		if ( hovered )
			ImGui::SetMouseCursor( ImGuiMouseCursor_Hand );

		if ( clicked )
			*value = !*value;

		Anim& a = anim_[ ImGui::GetID( id ) ];

		const float target = *value ? 1.f : 0.f;
		const float dt     = ImGui::GetIO( ).DeltaTime;

		// Softer spring when turning ON; snappier when turning OFF.
		const bool going_on = target > a.t;
		const float stiff   = going_on ? 55.f : 120.f;
		const float damp    = going_on ? 11.5f : 16.f;

		const float force = ( target - a.t ) * stiff - a.v * damp;
		a.v               += force * dt;
		a.t               += a.v * dt;

		if ( std::abs( target - a.t ) < 0.001f && std::abs( a.v ) < 0.01f )
		{
			a.t = target;
			a.v = 0.f;
		}

		a.t = std::clamp( a.t, 0.f, 1.f );

		// Ease-out quart on the visual so the ON settle is buttery.
		const float inv = 1.f - a.t;
		const float e   = 1.f - inv * inv * inv * inv;

		constexpr ImVec4 off = Theme::Elevated;
		constexpr ImVec4 on  = Theme::Primary;

		ImVec4 track {
			off.x + ( on.x - off.x ) * e,
			off.y + ( on.y - off.y ) * e,
			off.z + ( on.z - off.z ) * e,
			1.f
		};

		if ( hovered )
		{
			track.x = std::min( track.x + 0.04f, 1.f );
			track.y = std::min( track.y + 0.04f, 1.f );
			track.z = std::min( track.z + 0.04f, 1.f );
		}

		ImDrawList* dl = ImGui::GetWindowDrawList( );
		const ImVec2 max { pos.x + w, pos.y + h };

		dl->AddRectFilled( pos, max, Theme::col( track ), r );

		if ( hovered )
			dl->AddRect( pos, max, Theme::col( Theme::Primary, 0.25f ), r, 0, 1.f );

		const float pad    = Theme::px( 2.5f );
		const float knob_r = r - pad;
		const float travel = w - h;
		const float knob_x = pos.x + r + e * travel;
		const float knob_y = pos.y + r;
		const float scale  = active ? 0.90f : ( hovered ? 1.05f : 1.f );
		const float kr     = knob_r * scale;

		dl->AddCircleFilled( { knob_x + 0.5f, knob_y + 1.f }, kr, IM_COL32( 0, 0, 0, 50 ), 24 );
		dl->AddCircleFilled( { knob_x, knob_y }, kr, IM_COL32( 245, 248, 246, 255 ), 24 );

		if ( e > 0.01f )
			dl->AddCircle( { knob_x, knob_y }, kr, Theme::col( Theme::Primary, 0.20f * e ), 24, 1.f );

		return clicked;
	}
}

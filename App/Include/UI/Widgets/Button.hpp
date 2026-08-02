#pragma once

#include "UI/Theme.hpp"

namespace UI::Widgets
{
	enum class ButtonVariant : std::uint8_t
	{
		Primary,
		Ghost,
	};

	namespace detail
	{
		[[nodiscard]] inline float side( const float v ) noexcept
		{
			return Theme::px( v );
		}

		[[nodiscard]] inline ImU32 bg_color( const ButtonVariant variant, const bool hovered, const bool active ) noexcept
		{
			switch ( variant )
			{
				case ButtonVariant::Primary:
				{
					ImVec4 bg = Theme::Primary;
					if ( active )
						bg = { bg.x * 0.85f, bg.y * 0.85f, bg.z * 0.85f, 1.f };
					else if ( hovered )
						bg = { std::min( bg.x + 0.06f, 1.f ), std::min( bg.y + 0.06f, 1.f ), std::min( bg.z + 0.06f, 1.f ), 1.f };
					return Theme::col( bg );
				}

				case ButtonVariant::Ghost:
					if ( active )
						return Theme::col( Theme::Primary, 0.16f );
					if ( hovered )
						return Theme::col( Theme::Primary, 0.10f );
					return Theme::col( Theme::Elevated, 0.55f );
			}

			return 0;
		}

		[[nodiscard]] inline ImU32 text_color( const ButtonVariant variant, const bool hovered ) noexcept
		{
			if ( variant == ButtonVariant::Primary )
				return IM_COL32( 8, 18, 12, 255 );

			return Theme::col( hovered ? Theme::Foreground : Theme::SecondaryText );
		}
	}

	inline bool IconButton(
		const char* icon_with_id,
		const ButtonVariant variant = ButtonVariant::Ghost,
		const float size = 32.f,
		const float font_size = 15.f
	)
	{
		const char* icon_end = std::strchr( icon_with_id, '#' );

		ImGui::PushID( icon_with_id );

		ImFont* font = ImGui::GetFont( );
		const ImVec2 sz( Theme::px( size ), Theme::px( size ) );

		const ImVec2 label_size = font->CalcTextSizeA( Theme::px( font_size ), FLT_MAX, 0.f, icon_with_id, icon_end );
		const ImVec2 label_pos  = ImVec2( sz.x * 0.5f - label_size.x * 0.5f, sz.y * 0.5f - label_size.y * 0.5f );

		const bool pressed = ImGui::InvisibleButton( "##btn", sz );
		const bool hovered = ImGui::IsItemHovered( );
		const bool active  = ImGui::IsItemActive( );

		if ( hovered )
			ImGui::SetMouseCursor( ImGuiMouseCursor_Hand );

		const ImVec2 min = ImGui::GetItemRectMin( );
		const ImVec2 max = ImGui::GetItemRectMax( );

		ImDrawList* dl = ImGui::GetWindowDrawList( );

		const float r = Theme::Radius * 0.7f;

		const ImU32 bg = detail::bg_color( variant, hovered, active );

		if ( bg != 0 )
			dl->AddRectFilled( min, max, bg, r );

		if ( hovered || active )
			dl->AddRect( min, max, Theme::col( Theme::Primary, active ? 0.45f : 0.28f ), r, 0, 1.f );
		else if ( variant == ButtonVariant::Ghost )
			dl->AddRect( min, max, Theme::col( Theme::Border ), r, 0, 1.f );

		dl->AddText(
			font,
			Theme::px( font_size ),
			{ min.x + label_pos.x, min.y + label_pos.y },
			detail::text_color( variant, hovered ),
			icon_with_id,
			icon_end
		);

		ImGui::PopID( );

		return pressed;
	}

	inline bool Button(
		const char* label,
		const ButtonVariant variant = ButtonVariant::Ghost,
		const ImVec2& explicit_size = { 0.f, 0.f }
	)
	{
		const char* label_end = std::strchr( label, '#' );

		ImGui::PushID( label );

		ImFont* font = ImGui::GetFont( );

		const float r = Theme::Radius * 0.7f;
		const float font_size = Theme::px( 14.f );

		const ImVec2 label_size = font->CalcTextSizeA( font_size, FLT_MAX, 0.f, label, label_end );
		const float pad_x = Theme::px( 16.f );

		ImVec2 sz( label_size.x + pad_x * 2.f, Theme::px( 34.f ) );

		if ( explicit_size.x > 0.f )
			sz.x = explicit_size.x;
		if ( explicit_size.y > 0.f )
			sz.y = explicit_size.y;

		const bool pressed = ImGui::InvisibleButton( "##btn", sz );
		const bool hovered = ImGui::IsItemHovered( );
		const bool active  = ImGui::IsItemActive( );

		if ( hovered )
			ImGui::SetMouseCursor( ImGuiMouseCursor_Hand );

		const ImVec2 min = ImGui::GetItemRectMin( );
		const ImVec2 max = ImGui::GetItemRectMax( );

		ImDrawList* dl = ImGui::GetWindowDrawList( );

		const ImU32 bg = detail::bg_color( variant, hovered, active );

		if ( bg != 0 )
			dl->AddRectFilled( min, max, bg, r );

		if ( hovered || active )
			dl->AddRect( min, max, Theme::col( Theme::Primary, active ? 0.45f : 0.28f ), r, 0, 1.f );
		else if ( variant == ButtonVariant::Ghost )
			dl->AddRect( min, max, Theme::col( Theme::Border ), r, 0, 1.f );

		dl->AddText(
			font,
			font_size,
			{
				min.x + ( max.x - min.x - label_size.x ) * 0.5f,
				min.y + ( max.y - min.y - label_size.y ) * 0.5f
			},
			detail::text_color( variant, hovered ),
			label,
			label_end
		);

		ImGui::PopID( );

		return pressed;
	}
}

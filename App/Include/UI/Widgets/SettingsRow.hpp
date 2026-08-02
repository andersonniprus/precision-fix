#pragma once

#include "UI/Theme.hpp"
#include "UI/Widgets/Button.hpp"

namespace UI::Widgets
{
	inline constexpr float preset_button_width = 96.f;
	inline constexpr float preset_button_gap   = 10.f;
	inline constexpr float row_gap             = 14.f;

	[[nodiscard]] inline float preset_button_height( ) noexcept
	{
		return Theme::px( 30.f );
	}

	[[nodiscard]] inline float preset_row_width( const int count ) noexcept
	{
		const float gap = Theme::px( preset_button_gap );
		return Theme::px( preset_button_width ) * static_cast<float>( count )
		       + gap * static_cast<float>( count - 1 );
	}

	inline bool preset_button( const char* label, const ButtonVariant variant = ButtonVariant::Ghost )
	{
		return Button( label, variant, { Theme::px( preset_button_width ), preset_button_height( ) } );
	}

	inline void preset_same_line( )
	{
		ImGui::SameLine( 0.f, Theme::px( preset_button_gap ) );
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
		inline constexpr float gain_impact_gutter    = gain_impact_bar_width + 10.f;

		[[nodiscard]] inline ImU32 gain_color( const Level level ) noexcept
		{
			switch ( level )
			{
				case Level::Low: return IM_COL32( 110, 112, 118, 255 );
				case Level::Medium: return IM_COL32( 227, 176, 60, 255 );
				case Level::High: return IM_COL32( 88, 196, 112, 255 );
			}
			return IM_COL32( 110, 112, 118, 255 );
		}

		[[nodiscard]] inline ImU32 impact_color( const Level level ) noexcept
		{
			switch ( level )
			{
				case Level::Low: return IM_COL32( 88, 196, 112, 255 );
				case Level::Medium: return IM_COL32( 227, 176, 60, 255 );
				case Level::High: return IM_COL32( 222, 82, 82, 255 );
			}
			return IM_COL32( 88, 196, 112, 255 );
		}

		[[nodiscard]] inline const char* level_label( const Level level ) noexcept
		{
			switch ( level )
			{
				case Level::Low: return "Low";
				case Level::Medium: return "Medium";
				case Level::High: return "High";
			}
			return "Low";
		}

		inline void draw_recommend_badge( const char* label )
		{
			if ( !label || label[ 0 ] == '\0' )
				return;

			ImGui::SameLine( 0.f, Theme::px( 8.f ) );

			const float pad_x     = Theme::px( 7.f );
			const float pad_y     = Theme::px( 2.f );
			const float font_size = Theme::px( 11.f );

			ImFont* font           = ImGui::GetFont( );
			const ImVec2 text_size = font->CalcTextSizeA( font_size, FLT_MAX, 0.f, label );
			const ImVec2 size { text_size.x + pad_x * 2.f, text_size.y + pad_y * 2.f };

			const ImVec2 p0 = ImGui::GetCursorScreenPos( );
			const ImVec2 p1 { p0.x + size.x, p0.y + size.y };
			const float radius = size.y * 0.5f;

			ImDrawList* dl = ImGui::GetWindowDrawList( );
			dl->AddRectFilled( p0, p1, Theme::col( Theme::Primary, 0.14f ), radius );
			dl->AddRect( p0, p1, Theme::col( Theme::Primary, 0.35f ), radius, 0, 1.f );
			dl->AddText(
				font,
				font_size,
				{ p0.x + pad_x, p0.y + pad_y },
				Theme::col( Theme::Primary ),
				label
			);

			ImGui::Dummy( size );

			if ( ImGui::IsItemHovered( ImGuiHoveredFlags_AllowWhenBlockedByActiveItem ) )
			{
				ImGui::BeginTooltip( );
				ImGui::Text( "Recommended: %s", label );
				ImGui::EndTooltip( );
			}
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
		const char* recommend,
		ControlFn&& control
	)
	{
		const ImVec2 start        = ImGui::GetCursorPos( );
		const ImVec2 screen_start = ImGui::GetCursorScreenPos( );
		const float avail         = ImGui::GetContentRegionAvail( ).x;
		const float spacing       = ImGui::GetStyle( ).ItemSpacing.x;

		ImGui::SetCursorPosX( start.x + Detail::gain_impact_gutter );

		const float text_width = avail - Detail::gain_impact_gutter - control_width - spacing;

		ImGui::PushTextWrapPos( ImGui::GetCursorPos( ).x + text_width );
		ImGui::TextUnformatted( title );
		Detail::draw_recommend_badge( recommend );

		ImGui::SetCursorPosX( start.x + Detail::gain_impact_gutter );
		ImGui::PushStyleColor( ImGuiCol_Text, Theme::MutedText );
		ImGui::TextWrapped( "%s", description );
		ImGui::PopStyleColor( );
		ImGui::PopTextWrapPos( );

		const float row_height  = ImGui::GetCursorPosY( ) - start.y;
		const float slot_height = row_height > control_height ? row_height : control_height;

		ImGui::SetCursorPos(
			ImVec2( start.x + Detail::gain_impact_gutter + text_width + spacing,
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
		ImGui::SetCursorScreenPos( screen_start );
		ImGui::InvisibleButton( "##bar", ImVec2( Detail::gain_impact_bar_width, slot_height ) );

		if ( ImGui::IsItemHovered( ) )
		{
			ImGui::BeginTooltip( );
			ImGui::Text( "Performance gain: %s", Detail::level_label( gain ) );
			ImGui::Text( "System impact: %s", Detail::level_label( impact ) );
			if ( recommend && recommend[ 0 ] != '\0' )
				ImGui::Text( "Recommended: %s", recommend );
			ImGui::EndTooltip( );
		}

		ImGui::PopID( );

		ImGui::SetCursorPos( ImVec2( start.x, start.y + slot_height ) );
		ImGui::Dummy( ImVec2( 0.f, Theme::px( row_gap ) ) );
	}
}

#pragma once

#include "UI/Theme.hpp"

namespace UI::Widgets
{
	inline void section_header( const char* icon, const char* title, const char* description = nullptr )
	{
		ImGui::Dummy( ImVec2( 0.f, Theme::px( 6.f ) ) );

		const float icon_size = Theme::px( 16.f );
		const float title_size = 16.f;

		ImFont* font = ImGui::GetFont( );
		const ImVec2 title_sz = font->CalcTextSizeA( title_size, FLT_MAX, 0.f, title );

		const float row_h = ( std::max )( icon_size, title_sz.y );
		const ImVec2 origin = ImGui::GetCursorScreenPos( );
		ImDrawList* dl = ImGui::GetWindowDrawList( );

		if ( icon && icon[ 0 ] != '\0' )
		{
			dl->AddText(
				font,
				icon_size,
				{ origin.x, origin.y + ( row_h - icon_size ) * 0.5f },
				Theme::col( Theme::Primary ),
				icon
			);

			ImGui::SetCursorScreenPos( { origin.x + icon_size + Theme::px( 8.f ), origin.y + ( row_h - title_sz.y ) * 0.5f } );
		}

		ImGui::PushFont( nullptr, title_size );
		ImGui::PushStyleColor( ImGuiCol_Text, Theme::Foreground );
		ImGui::TextUnformatted( title );
		ImGui::PopStyleColor( );
		ImGui::PopFont( );

		ImGui::SetCursorScreenPos( { origin.x, origin.y + row_h + Theme::px( 4.f ) } );

		if ( description && description[ 0 ] != '\0' )
		{
			ImGui::PushStyleColor( ImGuiCol_Text, Theme::MutedText );
			ImGui::PushTextWrapPos( ImGui::GetCursorPos( ).x + ImGui::GetContentRegionAvail( ).x );
			ImGui::TextUnformatted( description );
			ImGui::PopTextWrapPos( );
			ImGui::PopStyleColor( );
		}

		ImGui::Dummy( ImVec2( 0.f, Theme::px( 8.f ) ) );

		const ImVec2 line = ImGui::GetCursorScreenPos( );
		const float width = ImGui::GetContentRegionAvail( ).x;

		dl->AddLine( line, { line.x + width, line.y }, Theme::separator_col( ), Theme::SeparatorThickness );

		ImGui::Dummy( ImVec2( 0.f, Theme::px( 10.f ) ) );
	}

	template<typename BodyFn>
	void section( const char* icon, const char* title, const char* description, BodyFn&& body )
	{
		section_header( icon, title, description );
		body( );
		ImGui::Dummy( ImVec2( 0.f, Theme::px( 14.f ) ) );
	}

	template<typename BodyFn>
	void section( const char* icon, const char* title, BodyFn&& body )
	{
		section( icon, title, nullptr, std::forward<BodyFn>( body ) );
	}
}

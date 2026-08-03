#include "Stdafx.hpp"
#include "UI/Managers/TabManager.hpp"
#include "UI/Theme.hpp"

namespace UI::Managers
{
	void TabManager::add_tab( std::string key, const char* icon, std::unique_ptr<IPage> page )
	{
		tabs_.push_back( { .key = std::move( key ), .icon = icon, .page = std::move( page ) } );
	}

	const char* TabManager::tab_label( const Tab& tab ) const noexcept
	{
		if ( intl_ )
			return intl_->tr( tab.key );

		return tab.key.c_str( );
	}

	void TabManager::render( )
	{
		if ( tabs_.empty( ) )
			return;

		if ( active_ == none )
		{
			active_ = 0;
			fade_   = 1.f;

			if ( tabs_[ 0 ].page )
				tabs_[ 0 ].page->on_activate( );
		}

		ImGui::Dummy( ImVec2( 0.f, Theme::px( 8.f ) ) );

		const float body_h = ImGui::GetContentRegionAvail( ).y;

		render_sidebar( body_h );

		ImGui::SameLine( 0.f, 0.f );

		render_content( body_h );
	}

	void TabManager::render_sidebar( const float height )
	{
		ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( Theme::px( 12.f ), Theme::px( 8.f ) ) );
		ImGui::PushStyleVar( ImGuiStyleVar_ChildRounding, 0.f );
		ImGui::PushStyleVar( ImGuiStyleVar_ChildBorderSize, 0.f );
		ImGui::PushStyleColor( ImGuiCol_ChildBg, Theme::Card );

		ImGui::BeginChild( "##sidebar", ImVec2( sidebar_width, height ), ImGuiChildFlags_AlwaysUseWindowPadding );

		ImFont* label_font = font_manager_ ? font_manager_->get( FontType::Medium, 15.f ) : ImGui::GetFont( );

		const float icon_size = Theme::px( 18.f );
		const float row_h     = ImGui::GetFrameHeight( ) + Theme::px( 8.f );

		ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2( 0.f, Theme::px( 4.f ) ) );

		for ( std::size_t i = 0; i < tabs_.size( ); ++i )
		{
			const auto& tab   = tabs_[ i ];
			const char* label = tab_label( tab );

			const bool selected = ( i == active_ );

			const float width = ImGui::GetContentRegionAvail( ).x;

			const ImVec2 p0 = ImGui::GetCursorScreenPos( );
			const ImVec2 p1 = { p0.x + width, p0.y + row_h };

			const bool hovered = ImGui::IsMouseHoveringRect( p0, p1, true );
			ImGui::PushID( static_cast<int>( i ) );
			const bool clicked = ImGui::InvisibleButton( "##tab", ImVec2( width, row_h ) );
			ImGui::PopID( );

			if ( hovered )
				ImGui::SetMouseCursor( ImGuiMouseCursor_Hand );

			if ( clicked && i != active_ )
			{
				previous_   = active_;
				active_     = i;
				fade_       = 1.f;
				fading_out_ = true;
			}

			ImDrawList* dl = ImGui::GetWindowDrawList( );

			ImU32 bg = 0;
			if ( selected )
				bg = Theme::col( Theme::Primary, 0.12f );
			else if ( hovered )
				bg = Theme::col( Theme::Primary, 0.07f );

			if ( bg != 0 )
				dl->AddRectFilled( p0, p1, bg, Theme::px( 6.f ) );

			if ( selected )
			{
				dl->AddRectFilled(
					{ p0.x, p0.y + Theme::px( 6.f ) },
					{ p0.x + Theme::px( 3.f ), p1.y - Theme::px( 6.f ) },
					Theme::col( Theme::Primary ),
					Theme::px( 2.f )
				);
			}

			const ImVec4 text_col = selected ? Theme::Foreground : ( hovered ? Theme::Foreground : Theme::SecondaryText );
			const ImVec4 icon_col = selected ? Theme::Primary : text_col;

			dl->AddText(
				ImGui::GetFont( ),
				icon_size,
				{ p0.x + Theme::px( 14.f ), p0.y + ( row_h - icon_size ) * 0.5f },
				Theme::col( icon_col ),
				tab.icon
			);

			const float label_font_size = Theme::px( 15.f );
			const ImVec2 label_size     = label_font->CalcTextSizeA( label_font_size, FLT_MAX, 0.f, label );

			dl->AddText(
				label_font,
				label_font_size,
				{ p0.x + Theme::px( 42.f ), p0.y + ( row_h - label_size.y ) * 0.5f },
				Theme::col( text_col ),
				label
			);
		}

		ImGui::PopStyleVar( );

		const ImVec2 sb_min = ImGui::GetWindowPos( );
		const ImVec2 sb_max = { sb_min.x + sidebar_width, sb_min.y + ImGui::GetWindowHeight( ) };

		ImGui::GetWindowDrawList( )->AddLine(
			{ sb_max.x - 0.5f, sb_min.y },
			{ sb_max.x - 0.5f, sb_max.y },
			Theme::separator_col( ),
			Theme::SeparatorThickness
		);

		ImGui::EndChild( );

		ImGui::PopStyleColor( );
		ImGui::PopStyleVar( 3 );
	}

	void TabManager::render_content( const float height )
	{
		ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( Theme::px( 20.f ), Theme::px( 12.f ) ) );
		ImGui::PushStyleVar( ImGuiStyleVar_ChildRounding, 0.f );
		ImGui::PushStyleVar( ImGuiStyleVar_ChildBorderSize, 0.f );
		ImGui::PushStyleColor( ImGuiCol_ChildBg, ImVec4( 0.f, 0.f, 0.f, 0.f ) );

		ImGui::BeginChild( "##tab_content", ImVec2( 0.f, height ), ImGuiChildFlags_AlwaysUseWindowPadding );

		std::size_t render_index = active_;
		float alpha              = fade_;

		if ( fading_out_ )
		{
			render_index = previous_;

			fade_ -= ImGui::GetIO( ).DeltaTime / fade_out_duration;

			if ( fade_ <= 0.f )
			{
				fade_       = 0.f;
				fading_out_ = false;

				if ( tabs_[ active_ ].page )
					tabs_[ active_ ].page->on_activate( );
			}
		}
		else if ( fade_ < 1.f )
		{
			fade_ += ImGui::GetIO( ).DeltaTime / fade_in_duration;
			fade_ = std::min( fade_, 1.f );
		}

		alpha = std::clamp( fade_, 0.f, 1.f );

		if ( render_index < tabs_.size( ) && tabs_[ render_index ].page )
		{
			ImGui::PushStyleVar( ImGuiStyleVar_Alpha, alpha );

			tabs_[ render_index ].page->render( );

			ImGui::PopStyleVar( );
		}

		ImGui::EndChild( );

		ImGui::PopStyleColor( );
		ImGui::PopStyleVar( 3 );
	}
}

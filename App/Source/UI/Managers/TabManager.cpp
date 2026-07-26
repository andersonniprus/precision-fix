#include "Stdafx.hpp"
#include "UI/Managers/TabManager.hpp"

namespace UI::Managers
{
	void TabManager::add_tab( std::string label, std::unique_ptr<IPage> page )
	{
		tabs_.push_back( { .label = std::move( label ), .page = std::move( page ) } );
	}

	void TabManager::render( )
	{
		if ( tabs_.empty( ) || !ImGui::BeginTabBar( "##tabs" ) )
			return;

		for ( std::size_t i = 0; i < tabs_.size( ); ++i )
		{
			const auto& tab = tabs_[ i ];

			if ( !ImGui::BeginTabItem( tab.label.c_str( ) ) )
				continue;

			if ( active_ != i )
			{
				active_ = i;

				if ( tab.page )
					tab.page->on_activate( );
			}

			if ( tab.page )
			{
				ImGui::BeginChild( "##tab_content" );
				tab.page->render( );
				ImGui::EndChild( );
			}

			ImGui::EndTabItem( );
		}

		ImGui::EndTabBar( );
	}
}

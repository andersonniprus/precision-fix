#include "Stdafx.hpp"
#include "UI/Managers/TabManager.hpp"

namespace UI::Managers
{
	void TabManager::add_tab ( std::string label, std::unique_ptr<IPage> page )
	{
		tabs_.push_back ( {.label = std::move ( label ), .page = std::move ( page )} );
	}

	void TabManager::render ( ) const
	{
		if ( tabs_.empty ( ) || !ImGui::BeginTabBar ( "##tabs" ) )
			return;

		for ( const auto& tab : tabs_ )
		{
			if ( !ImGui::BeginTabItem ( tab.label.c_str ( ) ) )
				continue;

			if ( tab.page )
				tab.page->render ( );

			ImGui::EndTabItem ( );
		}

		ImGui::EndTabBar ( );
	}
}
#pragma once
#include "UI/Interfaces/IPage.hpp"

namespace UI::Managers
{
	class TabManager
	{
	public:
		void add_tab ( std::string, std::unique_ptr<IPage> );

		void render ( ) const;

	private:
		struct Tab
		{
			std::string label;
			std::unique_ptr<IPage> page;
		};

		std::vector<Tab> tabs_;
	};
}
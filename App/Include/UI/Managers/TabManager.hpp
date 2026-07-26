#pragma once
#include "UI/Interfaces/IPage.hpp"

namespace UI::Managers
{
	class TabManager
	{
	public:
		void add_tab( std::string, std::unique_ptr<IPage> );

		void render( );

	private:
		static constexpr std::size_t none = static_cast<std::size_t>( -1 );

		struct Tab
		{
			std::string label;
			std::unique_ptr<IPage> page;
		};

		std::vector<Tab> tabs_;
		std::size_t active_ { none };
	};
}

#pragma once

#include "UI/Interfaces/IPage.hpp"

namespace UI::Pages
{
	class MousePage final : public IPage
	{
	public:
		void render ( ) override;
	};
}
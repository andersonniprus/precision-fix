#pragma once

#include "UI/Interfaces/IPage.hpp"

namespace UI::Pages
{
	class KeyboardPage final : public IPage
	{
	public:
		void render ( ) override;
	};
}
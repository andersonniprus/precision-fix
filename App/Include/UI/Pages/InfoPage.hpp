#pragma once

#include "UI/Interfaces/IPage.hpp"
#include "Settings.hpp"
#include "Intl.hpp"
#include "Logger.hpp"

namespace UI::Pages
{
	class InfoPage final : public IPage
	{
	public:
		InfoPage(
			std::shared_ptr<App::Settings> settings,
			std::shared_ptr<App::Intl> intl,
			std::shared_ptr<App::Logger> logger
		);

		void render( ) override;

	private:
		std::shared_ptr<App::Settings> settings_;
		std::shared_ptr<App::Intl> intl_;
		std::shared_ptr<App::Logger> logger_;
	};
}

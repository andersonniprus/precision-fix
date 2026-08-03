#pragma once

#include "UI/Interfaces/IPage.hpp"
#include "Logger.hpp"
#include "Intl.hpp"

namespace UI::Pages
{
	class LogsPage final : public IPage
	{
	public:
		LogsPage( std::shared_ptr<App::Logger> logger, std::shared_ptr<App::Intl> intl );

		void render( ) override;

	private:
		std::shared_ptr<App::Logger> logger_;
		std::shared_ptr<App::Intl> intl_;
	};
}

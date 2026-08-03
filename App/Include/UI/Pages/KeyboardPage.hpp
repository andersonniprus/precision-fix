#pragma once

#include "UI/Interfaces/IPage.hpp"
#include "Modules/KeyboardModule.hpp"
#include "Logger.hpp"
#include "Intl.hpp"

namespace UI::Pages
{
	class KeyboardPage final : public IPage
	{
	public:
		KeyboardPage(
			std::shared_ptr<Modules::KeyboardModule>,
			std::shared_ptr<App::Logger>,
			std::shared_ptr<App::Intl> );

		void render( ) override;
		void on_activate( ) override;

	private:
		void set_status( const Core::Status& );

		std::shared_ptr<Modules::KeyboardModule> keyboard_;
		std::shared_ptr<App::Logger> logger_;
		std::shared_ptr<App::Intl> intl_;
		std::string status_;
	};
}

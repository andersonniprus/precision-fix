#pragma once

#include "UI/Interfaces/IPage.hpp"
#include "Modules/MouseModule.hpp"
#include "Logger.hpp"
#include "Intl.hpp"

namespace UI::Pages
{
	class MousePage final : public IPage
	{
	public:
		MousePage(
			std::shared_ptr<Modules::MouseModule>,
			std::shared_ptr<App::Logger>,
			std::shared_ptr<App::Intl> );

		void render( ) override;
		void on_activate( ) override;

	private:
		void set_status( const Core::Status& );

		std::shared_ptr<Modules::MouseModule> mouse_;
		std::shared_ptr<App::Logger> logger_;
		std::shared_ptr<App::Intl> intl_;
		std::string status_;
	};
}

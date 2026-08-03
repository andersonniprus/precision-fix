#pragma once

#include "UI/Interfaces/IPage.hpp"
#include "Modules/SystemModule.hpp"
#include "Logger.hpp"
#include "Intl.hpp"

namespace UI::Pages
{
	class SystemPage final : public IPage
	{
	public:
		SystemPage(
			std::shared_ptr<Modules::SystemModule>,
			std::shared_ptr<App::Logger>,
			std::shared_ptr<App::Intl> );

		void render( ) override;
		void on_activate( ) override;

	private:
		void set_status( const Core::Status& );

		std::shared_ptr<Modules::SystemModule> system_;
		std::shared_ptr<App::Logger> logger_;
		std::shared_ptr<App::Intl> intl_;
		std::string status_;
	};
}

#pragma once

#include "UI/Interfaces/IPage.hpp"
#include "Modules/NetworkModule.hpp"
#include "Logger.hpp"
#include "Intl.hpp"

namespace UI::Pages
{
	class NetworkPage final : public IPage
	{
	public:
		NetworkPage(
			std::shared_ptr<Modules::NetworkModule>,
			std::shared_ptr<App::Logger>,
			std::shared_ptr<App::Intl> );

		void render( ) override;
		void on_activate( ) override;

	private:
		void set_status( const Core::Status& );

		std::shared_ptr<Modules::NetworkModule> network_;
		std::shared_ptr<App::Logger> logger_;
		std::shared_ptr<App::Intl> intl_;
		std::string status_;
	};
}

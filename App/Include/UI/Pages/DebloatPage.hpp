#pragma once

#include "UI/Interfaces/IPage.hpp"
#include "Modules/DebloatModule.hpp"
#include "Logger.hpp"
#include "Intl.hpp"

namespace UI::Pages
{
	class DebloatPage final : public IPage
	{
	public:
		DebloatPage(
			std::shared_ptr<Modules::DebloatModule>,
			std::shared_ptr<App::Logger>,
			std::shared_ptr<App::Intl> );

		void render( ) override;
		void on_activate( ) override;

	private:
		void set_status( const Core::Status& );

		std::shared_ptr<Modules::DebloatModule> debloat_;
		std::shared_ptr<App::Logger> logger_;
		std::shared_ptr<App::Intl> intl_;
		std::string status_;
	};
}

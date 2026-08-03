#pragma once

#include "UI/Interfaces/IPage.hpp"
#include "Modules/PrivacyModule.hpp"
#include "Logger.hpp"
#include "Intl.hpp"

namespace UI::Pages
{
	class PrivacyPage final : public IPage
	{
	public:
		PrivacyPage(
			std::shared_ptr<Modules::PrivacyModule>,
			std::shared_ptr<App::Logger>,
			std::shared_ptr<App::Intl> );

		void render( ) override;
		void on_activate( ) override;

	private:
		void set_status( const Core::Status& );

		std::shared_ptr<Modules::PrivacyModule> privacy_;
		std::shared_ptr<App::Logger> logger_;
		std::shared_ptr<App::Intl> intl_;
		std::string status_;
	};
}

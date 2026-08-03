#pragma once

#include "UI/Interfaces/IPage.hpp"
#include "Modules/AudioModule.hpp"
#include "Logger.hpp"
#include "Intl.hpp"

namespace UI::Pages
{
	class AudioPage final : public IPage
	{
	public:
		AudioPage(
			std::shared_ptr<Modules::AudioModule>,
			std::shared_ptr<App::Logger>,
			std::shared_ptr<App::Intl> );

		void render( ) override;
		void on_activate( ) override;

	private:
		void set_status( const Core::Status& );

		std::shared_ptr<Modules::AudioModule> audio_;
		std::shared_ptr<App::Logger> logger_;
		std::shared_ptr<App::Intl> intl_;
		std::string status_;
	};
}

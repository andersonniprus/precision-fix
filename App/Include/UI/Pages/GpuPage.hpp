#pragma once

#include "UI/Interfaces/IPage.hpp"
#include "Modules/GpuModule.hpp"
#include "Logger.hpp"
#include "Intl.hpp"

namespace UI::Pages
{
	class GpuPage final : public IPage
	{
	public:
		GpuPage(
			std::shared_ptr<Modules::GpuModule>,
			std::shared_ptr<App::Logger>,
			std::shared_ptr<App::Intl> );

		void render( ) override;
		void on_activate( ) override;

	private:
		void set_status( const Core::Status& );

		std::shared_ptr<Modules::GpuModule> gpu_;
		std::shared_ptr<App::Logger> logger_;
		std::shared_ptr<App::Intl> intl_;
		std::string status_;
	};
}

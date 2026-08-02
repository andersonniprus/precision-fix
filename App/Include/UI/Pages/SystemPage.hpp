#pragma once

#include "UI/Interfaces/IPage.hpp"
#include "Modules/SystemModule.hpp"

namespace UI::Pages
{
	class SystemPage final : public IPage
	{
	public:
		explicit SystemPage( std::shared_ptr<Modules::SystemModule> );

		void render( ) override;
		void on_activate( ) override;

	private:
		void set_status( const Core::Status& );

		std::shared_ptr<Modules::SystemModule> system_;
		std::string status_;
	};
}

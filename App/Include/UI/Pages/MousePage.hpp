#pragma once

#include "UI/Interfaces/IPage.hpp"
#include "Modules/MouseModule.hpp"

namespace UI::Pages
{
	class MousePage final : public IPage
	{
	public:
		explicit MousePage( std::shared_ptr<Modules::MouseModule> );

		void render( ) override;
		void on_activate( ) override;

	private:
		void set_status( const Core::Status& );

		std::shared_ptr<Modules::MouseModule> mouse_;
		std::string status_;
	};
}

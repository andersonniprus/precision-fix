#pragma once

#include "UI/Interfaces/IPage.hpp"
#include "Modules/KeyboardModule.hpp"

namespace UI::Pages
{
	class KeyboardPage final : public IPage
	{
	public:
		explicit KeyboardPage( std::shared_ptr<Modules::KeyboardModule> );

		void render( ) override;
		void on_activate( ) override;

	private:
		void set_status( const Core::Status& );

		std::shared_ptr<Modules::KeyboardModule> keyboard_;
		std::string status_;
	};
}

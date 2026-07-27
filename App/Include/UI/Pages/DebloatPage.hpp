#pragma once

#include "UI/Interfaces/IPage.hpp"
#include "Modules/DebloatModule.hpp"

namespace UI::Pages
{
	class DebloatPage final : public IPage
	{
	public:
		explicit DebloatPage( std::shared_ptr<Modules::DebloatModule> );

		void render( ) override;
		void on_activate( ) override;

	private:
		void set_status( const Core::Status& );

		std::shared_ptr<Modules::DebloatModule> debloat_;

		std::string status_;
	};
}

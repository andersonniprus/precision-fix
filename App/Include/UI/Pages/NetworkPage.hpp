#pragma once

#include "UI/Interfaces/IPage.hpp"
#include "Modules/NetworkModule.hpp"

namespace UI::Pages
{
	class NetworkPage final : public IPage
	{
	public:
		explicit NetworkPage( std::shared_ptr<Modules::NetworkModule> );

		void render( ) override;
		void on_activate( ) override;

	private:
		void set_status( const Core::Status& );

		std::shared_ptr<Modules::NetworkModule> network_;

		std::string status_;
	};
}

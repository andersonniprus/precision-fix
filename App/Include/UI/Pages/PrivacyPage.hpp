#pragma once

#include "UI/Interfaces/IPage.hpp"
#include "Modules/PrivacyModule.hpp"

namespace UI::Pages
{
	class PrivacyPage final : public IPage
	{
	public:
		explicit PrivacyPage( std::shared_ptr<Modules::PrivacyModule> );

		void render( ) override;
		void on_activate( ) override;

	private:
		void set_status( const Core::Status& );

		std::shared_ptr<Modules::PrivacyModule> privacy_;
		std::string status_;
	};
}

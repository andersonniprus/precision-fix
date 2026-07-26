#pragma once

#include "UI/Interfaces/IPage.hpp"
#include "Modules/AudioModule.hpp"

namespace UI::Pages
{
	class AudioPage final : public IPage
	{
	public:
		explicit AudioPage( std::shared_ptr<Modules::AudioModule> );

		void render( ) override;
		void on_activate( ) override;

	private:
		void set_status( const Core::Status& );

		std::shared_ptr<Modules::AudioModule> audio_;

		std::string status_;
	};
}

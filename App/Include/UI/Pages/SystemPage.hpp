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
		void render_scheduling( );
		void render_power( );
		void render_advanced( );
		void render_responsiveness( );

		template<Modules::SystemFeature F>
		void render_preset_row( const char* title, const char* description, std::uint32_t optimized );

		void set_status( const Core::Status& );

		std::shared_ptr<Modules::SystemModule> system_;

		std::string status_;
	};
}

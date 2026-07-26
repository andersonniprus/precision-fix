#pragma once

#include "UI/Interfaces/IPage.hpp"
#include "Modules/GpuModule.hpp"

namespace UI::Pages
{
	class GpuPage final : public IPage
	{
	public:
		explicit GpuPage( std::shared_ptr<Modules::GpuModule> );

		void render( ) override;
		void on_activate( ) override;

	private:
		void set_status( const Core::Status& );

		std::shared_ptr<Modules::GpuModule> gpu_;

		std::string status_;
	};
}

#include "Stdafx.hpp"
#include "UI/Pages/GpuPage.hpp"
#include "UI/Widgets/SettingsRow.hpp"
#include "UI/Widgets/Switch.hpp"

namespace UI::Pages
{
	GpuPage::GpuPage( std::shared_ptr<Modules::GpuModule> gpu )
		: gpu_( std::move( gpu ) )
	{
	}

	void GpuPage::on_activate( )
	{
		gpu_->refresh_all( );
	}

	void GpuPage::set_status( const Core::Status& status )
	{
		status_ = status ? std::string {} : std::string { Core::to_string( status.error( ) ) };
	}

	void GpuPage::render( )
	{
		const float switch_width  = ImGui::GetFrameHeight( ) * Widgets::switch_aspect;
		const float switch_height = ImGui::GetFrameHeight( );

		bool preemption = gpu_->get<Modules::GpuFeature::Preemption>( );

		Widgets::settings_row(
			"GPU preemption",
			"Lets the scheduler interrupt a running graphics task to service another one.",
			switch_width,
			switch_height,
			[ & ]
			{
				if ( Widgets::toggle_switch( "GPU preemption", &preemption ) )
					set_status( gpu_->set<Modules::GpuFeature::Preemption>( preemption ) );
			}
		);

		ImGui::Separator( );

		bool hdcp = gpu_->get<Modules::GpuFeature::Hdcp>( );

		Widgets::settings_row(
			"HDCP",
			"Content-protection handshake for the display output. NVIDIA only.",
			switch_width,
			switch_height,
			[ & ]
			{
				if ( Widgets::toggle_switch( "HDCP", &hdcp ) )
					set_status( gpu_->set<Modules::GpuFeature::Hdcp>( hdcp ) );
			}
		);

		if ( !status_.empty( ) )
			ImGui::TextColored( ImVec4( 1.f, 0.35f, 0.35f, 1.f ), "%s", status_.c_str( ) );
	}
}

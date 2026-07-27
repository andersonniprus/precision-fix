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
			Widgets::Level::Medium,
			Widgets::Level::Medium,
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
			Widgets::Level::Low,
			Widgets::Level::Low,
			[ & ]
			{
				if ( Widgets::toggle_switch( "HDCP", &hdcp ) )
					set_status( gpu_->set<Modules::GpuFeature::Hdcp>( hdcp ) );
			}
		);

		ImGui::Separator( );

		bool hags = gpu_->get<Modules::GpuFeature::HardwareGpuScheduling>( );

		Widgets::settings_row(
			"Hardware-accelerated GPU scheduling",
			"Lets the GPU manage its own video memory queue instead of the OS scheduler.",
			switch_width,
			switch_height,
			Widgets::Level::Medium,
			Widgets::Level::Low,
			[ & ]
			{
				if ( Widgets::toggle_switch( "Hardware-accelerated GPU scheduling", &hags ) )
					set_status( gpu_->set<Modules::GpuFeature::HardwareGpuScheduling>( hags ) );
			}
		);

		ImGui::Separator( );

		bool nvidia_preemption = gpu_->get<Modules::GpuFeature::NvidiaPreemptionOverride>( );

		Widgets::settings_row(
			"NVIDIA preemption override",
			"Disables NVIDIA driver-level task preemption. Undocumented vendor tweak, effect not guaranteed on all driver versions.",
			switch_width,
			switch_height,
			Widgets::Level::Medium,
			Widgets::Level::High,
			[ & ]
			{
				if ( Widgets::toggle_switch( "NVIDIA preemption override", &nvidia_preemption ) )
					set_status( gpu_->set<Modules::GpuFeature::NvidiaPreemptionOverride>( nvidia_preemption ) );
			}
		);

		ImGui::Separator( );

		bool amd_power_gating = gpu_->get<Modules::GpuFeature::AmdPowerGatingDisabled>( );

		Widgets::settings_row(
			"AMD power gating disabled",
			"Disables AMD driver-level power gating for lower latency at the cost of higher idle power draw. Undocumented vendor tweak.",
			switch_width,
			switch_height,
			Widgets::Level::Medium,
			Widgets::Level::High,
			[ & ]
			{
				if ( Widgets::toggle_switch( "AMD power gating disabled", &amd_power_gating ) )
					set_status( gpu_->set<Modules::GpuFeature::AmdPowerGatingDisabled>( amd_power_gating ) );
			}
		);

		ImGui::Separator( );

		bool latency_tolerance = gpu_->get<Modules::GpuFeature::GraphicsLatencyTolerance>( );

		Widgets::settings_row(
			"Graphics latency tolerance override",
			"Forces GPU/display power-management latency tolerances to their lowest values. Undocumented vendor tweak.",
			switch_width,
			switch_height,
			Widgets::Level::Low,
			Widgets::Level::Medium,
			[ & ]
			{
				if ( Widgets::toggle_switch( "Graphics latency tolerance override", &latency_tolerance ) )
					set_status( gpu_->set<Modules::GpuFeature::GraphicsLatencyTolerance>( latency_tolerance ) );
			}
		);

		if ( !status_.empty( ) )
			ImGui::TextColored( ImVec4( 1.f, 0.35f, 0.35f, 1.f ), "%s", status_.c_str( ) );
	}
}

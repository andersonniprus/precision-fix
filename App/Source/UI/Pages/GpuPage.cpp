#include "Stdafx.hpp"
#include "UI/Pages/GpuPage.hpp"
#include "UI/Theme.hpp"
#include "UI/Fonts/IconsLucide.h"
#include "UI/Widgets/Section.hpp"
#include "UI/Widgets/SettingsRow.hpp"
#include "UI/Widgets/Switch.hpp"

namespace UI::Pages
{
	using namespace Widgets;
	using enum Modules::GpuFeature;

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
		const float switch_width  = ImGui::GetFrameHeight( ) * switch_aspect;
		const float switch_height = ImGui::GetFrameHeight( );

		section( ICON_LC_CPU, "Scheduling", "GPU task scheduling and preemption.", [ & ]
		{
			bool preemption = gpu_->get<Preemption>( );

			settings_row(
				"GPU preemption",
				"Lets the scheduler interrupt a running graphics task to service another one.",
				switch_width, switch_height,
				Level::Medium, Level::Medium,
				"Off",
				[ & ]
				{
					if ( toggle_switch( "GPU preemption", &preemption ) )
					{
						set_status( gpu_->set<Preemption>( preemption ) );
					}
				}
			);

			bool hardware_scheduling = gpu_->get<HardwareGpuScheduling>( );

			settings_row(
				"Hardware-accelerated GPU scheduling",
				"Lets the GPU manage its own video memory queue instead of the OS scheduler.",
				switch_width, switch_height,
				Level::Medium, Level::Low,
				"On",
				[ & ]
				{
					if ( toggle_switch( "Hardware-accelerated GPU scheduling", &hardware_scheduling ) )
					{
						set_status( gpu_->set<HardwareGpuScheduling>( hardware_scheduling ) );
					}
				}
			);
		} );

		section( ICON_LC_CIRCUIT_BOARD, "Vendor", "Driver-level tweaks. Effect not guaranteed on all versions.", [ & ]
		{
			bool hdcp = gpu_->get<Hdcp>( );

			settings_row(
				"HDCP",
				"Content-protection handshake for the display output. NVIDIA only.",
				switch_width, switch_height,
				Level::Low, Level::Low,
				"Off",
				[ & ]
				{
					if ( toggle_switch( "HDCP", &hdcp ) )
					{
						set_status( gpu_->set<Hdcp>( hdcp ) );
					}
				}
			);

			bool nvidia_preemption = gpu_->get<NvidiaPreemptionOverride>( );

			settings_row(
				"NVIDIA preemption override",
				"Disables NVIDIA driver-level task preemption. Undocumented vendor tweak.",
				switch_width, switch_height,
				Level::Medium, Level::High,
				"On",
				[ & ]
				{
					if ( toggle_switch( "NVIDIA preemption override", &nvidia_preemption ) )
					{
						set_status( gpu_->set<NvidiaPreemptionOverride>( nvidia_preemption ) );
					}
				}
			);

			bool amd_power_gating = gpu_->get<AmdPowerGatingDisabled>( );

			settings_row(
				"AMD power gating disabled",
				"Disables AMD driver-level power gating for lower latency at higher idle power. Undocumented vendor tweak.",
				switch_width, switch_height,
				Level::Medium, Level::High,
				"On",
				[ & ]
				{
					if ( toggle_switch( "AMD power gating disabled", &amd_power_gating ) )
					{
						set_status( gpu_->set<AmdPowerGatingDisabled>( amd_power_gating ) );
					}
				}
			);

			bool latency_tolerance = gpu_->get<GraphicsLatencyTolerance>( );

			settings_row(
				"Graphics latency tolerance override",
				"Forces GPU/display power-management latency tolerances to their lowest values. Undocumented vendor tweak.",
				switch_width, switch_height,
				Level::Low, Level::Medium,
				"On",
				[ & ]
				{
					if ( toggle_switch( "Graphics latency tolerance override", &latency_tolerance ) )
					{
						set_status( gpu_->set<GraphicsLatencyTolerance>( latency_tolerance ) );
					}
				}
			);
		} );

		if ( !status_.empty( ) )
		{
			ImGui::TextColored( Theme::Danger, "%s", status_.c_str( ) );
		}
	}
}

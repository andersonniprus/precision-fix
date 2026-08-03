#include "Stdafx.hpp"
#include "UI/Pages/GpuPage.hpp"
#include "Logger.hpp"
#include "Intl.hpp"
#include "UI/Theme.hpp"
#include "UI/Fonts/IconsLucide.h"
#include "UI/Widgets/Section.hpp"
#include "UI/Widgets/SettingsRow.hpp"
#include "UI/Widgets/Switch.hpp"

namespace UI::Pages
{
	using namespace Widgets;
	using enum Modules::GpuFeature;

	GpuPage::GpuPage(
		std::shared_ptr<Modules::GpuModule> module,
		std::shared_ptr<App::Logger> logger,
		std::shared_ptr<App::Intl> intl
	)
		: gpu_( std::move( module ) ),
		  logger_( std::move( logger ) ),
		  intl_( std::move( intl ) )
	{
	}

	void GpuPage::on_activate( )
	{
		gpu_->refresh_all( );
	}

	void GpuPage::set_status( const Core::Status& status )
	{
		if ( status )
		{
			logger_->info( "GPU", "setting applied" );
			status_.clear( );
			return;
		}

		logger_->error( "GPU", std::string { Core::to_string( status.error( ) ) } );
		status_ = std::string { Core::to_string( status.error( ) ) };
	}


	void GpuPage::render( )
	{
		const float switch_width  = ImGui::GetFrameHeight( ) * switch_aspect;
		const float switch_height = ImGui::GetFrameHeight( );

		section( ICON_LC_CPU, intl_->tr( "Scheduling" ), intl_->tr( "GPU task scheduling and preemption." ), [ & ]
		{
			bool preemption = gpu_->get<Preemption>( );

			settings_row(
				intl_->tr( "GPU preemption" ),
				intl_->tr( "Lets the scheduler interrupt a running graphics task to service another one." ),
				switch_width, switch_height,
				Level::Medium, Level::Medium,
				intl_->tr( "Off" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "GPU preemption" ), &preemption ) )
					{
						set_status( gpu_->set<Preemption>( preemption ) );
					}
				}
			);

			bool hardware_scheduling = gpu_->get<HardwareGpuScheduling>( );

			settings_row(
				intl_->tr( "Hardware-accelerated GPU scheduling" ),
				intl_->tr( "Lets the GPU manage its own video memory queue instead of the OS scheduler." ),
				switch_width, switch_height,
				Level::Medium, Level::Low,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Hardware-accelerated GPU scheduling" ), &hardware_scheduling ) )
					{
						set_status( gpu_->set<HardwareGpuScheduling>( hardware_scheduling ) );
					}
				}
			);
		} );

		section( ICON_LC_CIRCUIT_BOARD, intl_->tr( "Vendor" ), intl_->tr( "Driver-level tweaks. Effect not guaranteed on all versions." ), [ & ]
		{
			bool hdcp = gpu_->get<Hdcp>( );

			settings_row(
				intl_->tr( "HDCP" ),
				intl_->tr( "Content-protection handshake for the display output. NVIDIA only." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "Off" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "HDCP" ), &hdcp ) )
					{
						set_status( gpu_->set<Hdcp>( hdcp ) );
					}
				}
			);

			bool nvidia_preemption = gpu_->get<NvidiaPreemptionOverride>( );

			settings_row(
				intl_->tr( "NVIDIA preemption override" ),
				intl_->tr( "Disables NVIDIA driver-level task preemption. Undocumented vendor tweak." ),
				switch_width, switch_height,
				Level::Medium, Level::High,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "NVIDIA preemption override" ), &nvidia_preemption ) )
					{
						set_status( gpu_->set<NvidiaPreemptionOverride>( nvidia_preemption ) );
					}
				}
			);

			bool amd_power_gating = gpu_->get<AmdPowerGatingDisabled>( );

			settings_row(
				intl_->tr( "AMD power gating disabled" ),
				intl_->tr( "Disables AMD driver-level power gating for lower latency at higher idle power. Undocumented vendor tweak." ),
				switch_width, switch_height,
				Level::Medium, Level::High,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "AMD power gating disabled" ), &amd_power_gating ) )
					{
						set_status( gpu_->set<AmdPowerGatingDisabled>( amd_power_gating ) );
					}
				}
			);

			bool latency_tolerance = gpu_->get<GraphicsLatencyTolerance>( );

			settings_row(
				intl_->tr( "Graphics latency tolerance override" ),
				intl_->tr( "Forces GPU/display power-management latency tolerances to their lowest values. Undocumented vendor tweak." ),
				switch_width, switch_height,
				Level::Low, Level::Medium,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Graphics latency tolerance override" ), &latency_tolerance ) )
					{
						set_status( gpu_->set<GraphicsLatencyTolerance>( latency_tolerance ) );
					}
				}
			);
		} );

		section( ICON_LC_MONITOR, intl_->tr( "Display" ), intl_->tr( "Desktop Window Manager composition paths." ), [ & ]
		{
			bool mpo = gpu_->get<MultiPlaneOverlays>( );

			settings_row(
				intl_->tr( "Multi-plane overlays (MPO)" ),
				intl_->tr( "Hardware overlay planes for the desktop. Off can fix stutter on some multi-monitor / VRR setups." ),
				switch_width, switch_height,
				Level::Medium, Level::Medium,
				intl_->tr( "Off" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Multi-plane overlays (MPO)" ), &mpo ) )
					{
						set_status( gpu_->set<MultiPlaneOverlays>( mpo ) );
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

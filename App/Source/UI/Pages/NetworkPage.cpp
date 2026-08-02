#include "Stdafx.hpp"
#include "UI/Pages/NetworkPage.hpp"
#include "UI/Theme.hpp"
#include "UI/Fonts/IconsLucide.h"
#include "UI/Widgets/Section.hpp"
#include "UI/Widgets/SettingsRow.hpp"
#include "UI/Widgets/Switch.hpp"

namespace UI::Pages
{
	using namespace Widgets;
	using enum Modules::NetworkFeature;

	NetworkPage::NetworkPage( std::shared_ptr<Modules::NetworkModule> network )
		: network_( std::move( network ) )
	{
	}

	void NetworkPage::on_activate( )
	{
		network_->refresh_all( );
	}

	void NetworkPage::set_status( const Core::Status& status )
	{
		status_ = status ? std::string {} : std::string { Core::to_string( status.error( ) ) };
	}

	void NetworkPage::render( )
	{
		const float switch_width  = ImGui::GetFrameHeight( ) * switch_aspect;
		const float switch_height = ImGui::GetFrameHeight( );

		section( ICON_LC_NETWORK, "Stack", "TCP/IP and DNS behavior.", [ & ]
		{
			bool nagle = network_->get<Nagle>( );

			settings_row(
				"Nagle's algorithm",
				"Bundles small outgoing packets to save bandwidth at the cost of latency.",
				switch_width, switch_height,
				Level::Medium, Level::Low,
				"Off",
				[ & ]
				{
					if ( toggle_switch( "Nagle's algorithm", &nagle ) )
					{
						set_status( network_->set<Nagle>( nagle ) );
					}
				}
			);

			bool active_probing = network_->get<ActiveProbing>( );

			settings_row(
				"Internet connectivity probing",
				"Periodically probes a Microsoft endpoint in the background to confirm connectivity.",
				switch_width, switch_height,
				Level::Low, Level::Low,
				"Off",
				[ & ]
				{
					if ( toggle_switch( "Internet connectivity probing", &active_probing ) )
					{
						set_status( network_->set<ActiveProbing>( active_probing ) );
					}
				}
			);

			bool fast_dns = network_->get<FastDns>( );

			settings_row(
				"Fast DNS (Cloudflare)",
				"Overrides every adapter's DNS servers with 1.1.1.1 / 1.0.0.1.",
				switch_width, switch_height,
				Level::Medium, Level::Low,
				"On",
				[ & ]
				{
					if ( toggle_switch( "Fast DNS (Cloudflare)", &fast_dns ) )
					{
						set_status( network_->set<FastDns>( fast_dns ) );
					}
				}
			);

			bool wide_port_range = network_->get<WidePortRange>( );

			settings_row(
				"Wide dynamic port range",
				"Raises the maximum ephemeral outgoing port to 65534 for more concurrent connections.",
				switch_width, switch_height,
				Level::Low, Level::Low,
				"On",
				[ & ]
				{
					if ( toggle_switch( "Wide dynamic port range", &wide_port_range ) )
					{
						set_status( network_->set<WidePortRange>( wide_port_range ) );
					}
				}
			);

			bool fast_port_recycling = network_->get<FastPortRecycling>( );

			settings_row(
				"Fast TCP port recycling",
				"Shortens the TIME_WAIT delay before a closed TCP port can be reused.",
				switch_width, switch_height,
				Level::Low, Level::Medium,
				"On",
				[ & ]
				{
					if ( toggle_switch( "Fast TCP port recycling", &fast_port_recycling ) )
					{
						set_status( network_->set<FastPortRecycling>( fast_port_recycling ) );
					}
				}
			);

			bool linux_like_ttl = network_->get<LinuxLikeTtl>( );

			settings_row(
				"Linux-like TTL",
				"Sets the default IP time-to-live to 64, matching Linux/macOS defaults.",
				switch_width, switch_height,
				Level::Low, Level::Low,
				"On",
				[ & ]
				{
					if ( toggle_switch( "Linux-like TTL", &linux_like_ttl ) )
					{
						set_status( network_->set<LinuxLikeTtl>( linux_like_ttl ) );
					}
				}
			);

			bool fast_name_resolution = network_->get<FastNameResolution>( );

			settings_row(
				"Fast name resolution",
				"Reprioritizes the DNS/hosts/NetBIOS name-resolution provider order.",
				switch_width, switch_height,
				Level::Low, Level::Low,
				"On",
				[ & ]
				{
					if ( toggle_switch( "Fast name resolution", &fast_name_resolution ) )
					{
						set_status( network_->set<FastNameResolution>( fast_name_resolution ) );
					}
				}
			);

			bool disable_ipv6 = network_->get<DisableIPv6>( );

			settings_row(
				"Disable IPv6",
				"Disables IPv6 on all interfaces and tunnels system-wide.",
				switch_width, switch_height,
				Level::Low, Level::Medium,
				"On",
				[ & ]
				{
					if ( toggle_switch( "Disable IPv6", &disable_ipv6 ) )
					{
						set_status( network_->set<DisableIPv6>( disable_ipv6 ) );
					}
				}
			);
		} );

		section( ICON_LC_WIFI, "Adapter", "NIC power and offload tuning.", [ & ]
		{
			bool nic_power_saving = network_->get<NicPowerSaving>( );

			settings_row(
				"NIC power saving",
				"Toggles adapter-level power-saving features (EEE, green Ethernet, link-speed reduction).",
				switch_width, switch_height,
				Level::Medium, Level::Medium,
				"Off",
				[ & ]
				{
					if ( toggle_switch( "NIC power saving", &nic_power_saving ) )
					{
						set_status( network_->set<NicPowerSaving>( nic_power_saving ) );
					}
				}
			);

			bool wake_on_lan = network_->get<WakeOnLan>( );

			settings_row(
				"Wake-on-LAN",
				"Toggles adapter-level Wake-on-LAN and magic-packet wake support.",
				switch_width, switch_height,
				Level::Low, Level::Low,
				"Off",
				[ & ]
				{
					if ( toggle_switch( "Wake-on-LAN", &wake_on_lan ) )
					{
						set_status( network_->set<WakeOnLan>( wake_on_lan ) );
					}
				}
			);

			bool nic_offload_tuning = network_->get<NicOffloadTuning>( );

			settings_row(
				"NIC offload tuning",
				"Disables hardware checksum/segmentation offloading and tunes RSS and buffer sizes for lower latency.",
				switch_width, switch_height,
				Level::Medium, Level::Medium,
				"On",
				[ & ]
				{
					if ( toggle_switch( "NIC offload tuning", &nic_offload_tuning ) )
					{
						set_status( network_->set<NicOffloadTuning>( nic_offload_tuning ) );
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

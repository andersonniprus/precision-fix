#include "Stdafx.hpp"
#include "UI/Pages/NetworkPage.hpp"
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
	using enum Modules::NetworkFeature;

	NetworkPage::NetworkPage(
		std::shared_ptr<Modules::NetworkModule> module,
		std::shared_ptr<App::Logger> logger,
		std::shared_ptr<App::Intl> intl
	)
		: network_( std::move( module ) ),
		  logger_( std::move( logger ) ),
		  intl_( std::move( intl ) )
	{
	}

	void NetworkPage::on_activate( )
	{
		network_->refresh_all( );
	}

	void NetworkPage::set_status( const Core::Status& status )
	{
		if ( status )
		{
			logger_->info( "Network", "setting applied" );
			status_.clear( );
			return;
		}

		logger_->error( "Network", std::string { Core::to_string( status.error( ) ) } );
		status_ = std::string { Core::to_string( status.error( ) ) };
	}


	void NetworkPage::render( )
	{
		const float switch_width  = ImGui::GetFrameHeight( ) * switch_aspect;
		const float switch_height = ImGui::GetFrameHeight( );

		section( ICON_LC_NETWORK, intl_->tr( "Stack" ), intl_->tr( "TCP/IP and DNS behavior." ), [ & ]
		{
			bool nagle = network_->get<Nagle>( );

			settings_row(
				intl_->tr( "Nagle's algorithm" ),
				intl_->tr( "Bundles small outgoing packets to save bandwidth at the cost of latency." ),
				switch_width, switch_height,
				Level::Medium, Level::Low,
				intl_->tr( "Off" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Nagle's algorithm" ), &nagle ) )
					{
						set_status( network_->set<Nagle>( nagle ) );
					}
				}
			);

			bool active_probing = network_->get<ActiveProbing>( );

			settings_row(
				intl_->tr( "Internet connectivity probing" ),
				intl_->tr( "Periodically probes a Microsoft endpoint in the background to confirm connectivity." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "Off" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Internet connectivity probing" ), &active_probing ) )
					{
						set_status( network_->set<ActiveProbing>( active_probing ) );
					}
				}
			);

			bool fast_dns = network_->get<FastDns>( );

			settings_row(
				intl_->tr( "Fast DNS (Cloudflare)" ),
				intl_->tr( "Overrides every adapter's DNS servers with 1.1.1.1 / 1.0.0.1." ),
				switch_width, switch_height,
				Level::Medium, Level::Low,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Fast DNS (Cloudflare)" ), &fast_dns ) )
					{
						set_status( network_->set<FastDns>( fast_dns ) );
					}
				}
			);

			bool wide_port_range = network_->get<WidePortRange>( );

			settings_row(
				intl_->tr( "Wide dynamic port range" ),
				intl_->tr( "Raises the maximum ephemeral outgoing port to 65534 for more concurrent connections." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Wide dynamic port range" ), &wide_port_range ) )
					{
						set_status( network_->set<WidePortRange>( wide_port_range ) );
					}
				}
			);

			bool fast_port_recycling = network_->get<FastPortRecycling>( );

			settings_row(
				intl_->tr( "Fast TCP port recycling" ),
				intl_->tr( "Shortens the TIME_WAIT delay before a closed TCP port can be reused." ),
				switch_width, switch_height,
				Level::Low, Level::Medium,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Fast TCP port recycling" ), &fast_port_recycling ) )
					{
						set_status( network_->set<FastPortRecycling>( fast_port_recycling ) );
					}
				}
			);

			bool linux_like_ttl = network_->get<LinuxLikeTtl>( );

			settings_row(
				intl_->tr( "Linux-like TTL" ),
				intl_->tr( "Sets the default IP time-to-live to 64, matching Linux/macOS defaults." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Linux-like TTL" ), &linux_like_ttl ) )
					{
						set_status( network_->set<LinuxLikeTtl>( linux_like_ttl ) );
					}
				}
			);

			bool fast_name_resolution = network_->get<FastNameResolution>( );

			settings_row(
				intl_->tr( "Fast name resolution" ),
				intl_->tr( "Reprioritizes the DNS/hosts/NetBIOS name-resolution provider order." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Fast name resolution" ), &fast_name_resolution ) )
					{
						set_status( network_->set<FastNameResolution>( fast_name_resolution ) );
					}
				}
			);

			bool disable_ipv6 = network_->get<DisableIPv6>( );

			settings_row(
				intl_->tr( "Disable IPv6" ),
				intl_->tr( "Disables IPv6 on all interfaces and tunnels system-wide." ),
				switch_width, switch_height,
				Level::Low, Level::Medium,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Disable IPv6" ), &disable_ipv6 ) )
					{
						set_status( network_->set<DisableIPv6>( disable_ipv6 ) );
					}
				}
			);
		} );

		section( ICON_LC_WIFI, intl_->tr( "Adapter" ), intl_->tr( "NIC power and offload tuning." ), [ & ]
		{
			bool nic_power_saving = network_->get<NicPowerSaving>( );

			settings_row(
				intl_->tr( "NIC power saving" ),
				intl_->tr( "Toggles adapter-level power-saving features (EEE, green Ethernet, link-speed reduction)." ),
				switch_width, switch_height,
				Level::Medium, Level::Medium,
				intl_->tr( "Off" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "NIC power saving" ), &nic_power_saving ) )
					{
						set_status( network_->set<NicPowerSaving>( nic_power_saving ) );
					}
				}
			);

			bool wake_on_lan = network_->get<WakeOnLan>( );

			settings_row(
				intl_->tr( "Wake-on-LAN" ),
				intl_->tr( "Toggles adapter-level Wake-on-LAN and magic-packet wake support." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "Off" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Wake-on-LAN" ), &wake_on_lan ) )
					{
						set_status( network_->set<WakeOnLan>( wake_on_lan ) );
					}
				}
			);

			bool nic_offload_tuning = network_->get<NicOffloadTuning>( );

			settings_row(
				intl_->tr( "NIC offload tuning" ),
				intl_->tr( "Disables hardware checksum/segmentation offloading and tunes RSS and buffer sizes for lower latency." ),
				switch_width, switch_height,
				Level::Medium, Level::Medium,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "NIC offload tuning" ), &nic_offload_tuning ) )
					{
						set_status( network_->set<NicOffloadTuning>( nic_offload_tuning ) );
					}
				}
			);
		} );

		section( ICON_LC_GAUGE, intl_->tr( "TCP global" ), intl_->tr( "Stack-level latency and bufferbloat controls via netsh." ), [ & ]
		{
			bool tcp_auto_tuning = network_->get<TcpAutoTuning>( );

			settings_row(
				intl_->tr( "TCP receive window auto-tuning" ),
				intl_->tr( "Dynamically grows the receive window for high throughput. Off can cut bufferbloat and jitter (may lower Mbps)." ),
				switch_width, switch_height,
				Level::High, Level::Medium,
				intl_->tr( "Off" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "TCP receive window auto-tuning" ), &tcp_auto_tuning ) )
					{
						set_status( network_->set<TcpAutoTuning>( tcp_auto_tuning ) );
					}
				}
			);

			bool ecn = network_->get<EcnCapability>( );

			settings_row(
				intl_->tr( "ECN capability" ),
				intl_->tr( "Explicit Congestion Notification. Helps routers signal congestion without dropping packets." ),
				switch_width, switch_height,
				Level::Medium, Level::Low,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "ECN capability" ), &ecn ) )
					{
						set_status( network_->set<EcnCapability>( ecn ) );
					}
				}
			);

			bool ctcp = network_->get<CongestionCtcp>( );

			settings_row(
				intl_->tr( "CTCP congestion provider" ),
				intl_->tr( "Uses Compound TCP instead of the default congestion control algorithm when supported." ),
				switch_width, switch_height,
				Level::Medium, Level::Medium,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "CTCP congestion provider" ), &ctcp ) )
					{
						set_status( network_->set<CongestionCtcp>( ctcp ) );
					}
				}
			);

			bool timestamps = network_->get<TcpTimestamps>( );

			settings_row(
				intl_->tr( "TCP timestamps (RFC 1323)" ),
				intl_->tr( "Adds timestamp options to TCP segments. Off can slightly reduce per-packet overhead." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "Off" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "TCP timestamps (RFC 1323)" ), &timestamps ) )
					{
						set_status( network_->set<TcpTimestamps>( timestamps ) );
					}
				}
			);

			bool global_rss = network_->get<GlobalRss>( );

			settings_row(
				intl_->tr( "Receive-side scaling (RSS)" ),
				intl_->tr( "Spreads inbound packet processing across CPU cores at the TCP stack level." ),
				switch_width, switch_height,
				Level::Medium, Level::Low,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Receive-side scaling (RSS)" ), &global_rss ) )
					{
						set_status( network_->set<GlobalRss>( global_rss ) );
					}
				}
			);

			bool doh = network_->get<DnsOverHttps>( );

			settings_row(
				intl_->tr( "DNS over HTTPS (auto)" ),
				intl_->tr( "Lets Windows automatically use known DoH servers for encrypted name resolution." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "On" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "DNS over HTTPS (auto)" ), &doh ) )
					{
						set_status( network_->set<DnsOverHttps>( doh ) );
					}
				}
			);

			bool qos_limit = network_->get<QosBandwidthLimit>( );

			settings_row(
				intl_->tr( "QoS non-best-effort bandwidth reserve" ),
				intl_->tr( "Legacy QoS reservation that can cap throughput. Off sets NonBestEffortLimit to 0." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "Off" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "QoS non-best-effort bandwidth reserve" ), &qos_limit ) )
					{
						set_status( network_->set<QosBandwidthLimit>( qos_limit ) );
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

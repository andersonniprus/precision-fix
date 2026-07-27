#include "Stdafx.hpp"
#include "UI/Pages/NetworkPage.hpp"
#include "UI/Widgets/SettingsRow.hpp"
#include "UI/Widgets/Switch.hpp"

namespace UI::Pages
{
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
		const float switch_width  = ImGui::GetFrameHeight( ) * Widgets::switch_aspect;
		const float switch_height = ImGui::GetFrameHeight( );

		bool nagle = network_->get<Modules::NetworkFeature::Nagle>( );

		Widgets::settings_row(
			"Nagle's algorithm",
			"Bundles small outgoing packets to save bandwidth at the cost of latency.",
			switch_width,
			switch_height,
			Widgets::Level::Medium,
			Widgets::Level::Low,
			[ & ]
			{
				if ( Widgets::toggle_switch( "Nagle's algorithm", &nagle ) )
					set_status( network_->set<Modules::NetworkFeature::Nagle>( nagle ) );
			}
		);

		ImGui::Separator( );

		bool active_probing = network_->get<Modules::NetworkFeature::ActiveProbing>( );

		Widgets::settings_row(
			"Internet connectivity probing",
			"Periodically probes a Microsoft endpoint in the background to confirm connectivity.",
			switch_width,
			switch_height,
			Widgets::Level::Low,
			Widgets::Level::Low,
			[ & ]
			{
				if ( Widgets::toggle_switch( "Internet connectivity probing", &active_probing ) )
					set_status( network_->set<Modules::NetworkFeature::ActiveProbing>( active_probing ) );
			}
		);

		ImGui::Separator( );

		bool fast_dns = network_->get<Modules::NetworkFeature::FastDns>( );

		Widgets::settings_row(
			"Fast DNS (Cloudflare)",
			"Overrides every adapter's DNS servers with 1.1.1.1 / 1.0.0.1.",
			switch_width,
			switch_height,
			Widgets::Level::Medium,
			Widgets::Level::Low,
			[ & ]
			{
				if ( Widgets::toggle_switch( "Fast DNS (Cloudflare)", &fast_dns ) )
					set_status( network_->set<Modules::NetworkFeature::FastDns>( fast_dns ) );
			}
		);

		ImGui::Separator( );

		bool wide_port_range = network_->get<Modules::NetworkFeature::WidePortRange>( );

		Widgets::settings_row(
			"Wide dynamic port range",
			"Raises the maximum ephemeral outgoing port to 65534 for more concurrent connections.",
			switch_width,
			switch_height,
			Widgets::Level::Low,
			Widgets::Level::Low,
			[ & ]
			{
				if ( Widgets::toggle_switch( "Wide dynamic port range", &wide_port_range ) )
					set_status( network_->set<Modules::NetworkFeature::WidePortRange>( wide_port_range ) );
			}
		);

		ImGui::Separator( );

		bool fast_port_recycling = network_->get<Modules::NetworkFeature::FastPortRecycling>( );

		Widgets::settings_row(
			"Fast TCP port recycling",
			"Shortens the TIME_WAIT delay before a closed TCP port can be reused.",
			switch_width,
			switch_height,
			Widgets::Level::Low,
			Widgets::Level::Medium,
			[ & ]
			{
				if ( Widgets::toggle_switch( "Fast TCP port recycling", &fast_port_recycling ) )
					set_status( network_->set<Modules::NetworkFeature::FastPortRecycling>( fast_port_recycling ) );
			}
		);

		ImGui::Separator( );

		bool linux_like_ttl = network_->get<Modules::NetworkFeature::LinuxLikeTtl>( );

		Widgets::settings_row(
			"Linux-like TTL",
			"Sets the default IP time-to-live to 64, matching Linux/macOS defaults.",
			switch_width,
			switch_height,
			Widgets::Level::Low,
			Widgets::Level::Low,
			[ & ]
			{
				if ( Widgets::toggle_switch( "Linux-like TTL", &linux_like_ttl ) )
					set_status( network_->set<Modules::NetworkFeature::LinuxLikeTtl>( linux_like_ttl ) );
			}
		);

		ImGui::Separator( );

		bool fast_name_resolution = network_->get<Modules::NetworkFeature::FastNameResolution>( );

		Widgets::settings_row(
			"Fast name resolution",
			"Reprioritizes the DNS/hosts/NetBIOS name-resolution provider order.",
			switch_width,
			switch_height,
			Widgets::Level::Low,
			Widgets::Level::Low,
			[ & ]
			{
				if ( Widgets::toggle_switch( "Fast name resolution", &fast_name_resolution ) )
					set_status( network_->set<Modules::NetworkFeature::FastNameResolution>( fast_name_resolution ) );
			}
		);

		ImGui::Separator( );

		bool disable_ipv6 = network_->get<Modules::NetworkFeature::DisableIPv6>( );

		Widgets::settings_row(
			"Disable IPv6",
			"Disables IPv6 on all interfaces and tunnels system-wide.",
			switch_width,
			switch_height,
			Widgets::Level::Low,
			Widgets::Level::Medium,
			[ & ]
			{
				if ( Widgets::toggle_switch( "Disable IPv6", &disable_ipv6 ) )
					set_status( network_->set<Modules::NetworkFeature::DisableIPv6>( disable_ipv6 ) );
			}
		);

		ImGui::Separator( );

		bool nic_power_saving = network_->get<Modules::NetworkFeature::NicPowerSaving>( );

		Widgets::settings_row(
			"NIC power saving",
			"Toggles adapter-level power-saving features (EEE, green Ethernet, link-speed reduction).",
			switch_width,
			switch_height,
			Widgets::Level::Medium,
			Widgets::Level::Medium,
			[ & ]
			{
				if ( Widgets::toggle_switch( "NIC power saving", &nic_power_saving ) )
					set_status( network_->set<Modules::NetworkFeature::NicPowerSaving>( nic_power_saving ) );
			}
		);

		ImGui::Separator( );

		bool wake_on_lan = network_->get<Modules::NetworkFeature::WakeOnLan>( );

		Widgets::settings_row(
			"Wake-on-LAN",
			"Toggles adapter-level Wake-on-LAN and magic-packet wake support.",
			switch_width,
			switch_height,
			Widgets::Level::Low,
			Widgets::Level::Low,
			[ & ]
			{
				if ( Widgets::toggle_switch( "Wake-on-LAN", &wake_on_lan ) )
					set_status( network_->set<Modules::NetworkFeature::WakeOnLan>( wake_on_lan ) );
			}
		);

		ImGui::Separator( );

		bool nic_offload_tuning = network_->get<Modules::NetworkFeature::NicOffloadTuning>( );

		Widgets::settings_row(
			"NIC offload tuning",
			"Disables hardware checksum/segmentation offloading and tunes RSS and buffer sizes for lower latency.",
			switch_width,
			switch_height,
			Widgets::Level::Medium,
			Widgets::Level::Medium,
			[ & ]
			{
				if ( Widgets::toggle_switch( "NIC offload tuning", &nic_offload_tuning ) )
					set_status( network_->set<Modules::NetworkFeature::NicOffloadTuning>( nic_offload_tuning ) );
			}
		);

		if ( !status_.empty( ) )
			ImGui::TextColored( ImVec4( 1.f, 0.35f, 0.35f, 1.f ), "%s", status_.c_str( ) );
	}
}

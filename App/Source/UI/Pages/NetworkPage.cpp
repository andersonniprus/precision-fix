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
			[ & ]
			{
				if ( Widgets::toggle_switch( "Fast DNS (Cloudflare)", &fast_dns ) )
					set_status( network_->set<Modules::NetworkFeature::FastDns>( fast_dns ) );
			}
		);

		if ( !status_.empty( ) )
			ImGui::TextColored( ImVec4( 1.f, 0.35f, 0.35f, 1.f ), "%s", status_.c_str( ) );
	}
}

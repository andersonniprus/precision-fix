#pragma once

#include "Modules/ModuleBase.hpp"

namespace Modules
{
	enum class NetworkFeature : std::uint8_t
	{
		Nagle,
		ActiveProbing,
		FastDns,
		WidePortRange,
		FastPortRecycling,
		LinuxLikeTtl,
		FastNameResolution,
		DisableIPv6,
		NicPowerSaving,
		WakeOnLan,
		NicOffloadTuning,
		TcpAutoTuning,
		EcnCapability,
		CongestionCtcp,
		TcpTimestamps,
		GlobalRss,
		DnsOverHttps,
		QosBandwidthLimit,
		Count,
	};

	class NetworkModule final : public ModuleBase<NetworkModule, NetworkFeature>
	{
	public:
		NetworkModule( );

	private:
		template<auto>
		friend struct FeatureTraits;

		[[nodiscard]] static Core::Result<bool> load_nagle( );
		static Core::Status apply_nagle( const bool& );

		[[nodiscard]] static Core::Result<bool> load_active_probing( );
		static Core::Status apply_active_probing( const bool& );

		[[nodiscard]] static Core::Result<bool> load_fast_dns( );
		static Core::Status apply_fast_dns( const bool& );

		[[nodiscard]] static Core::Result<bool> load_wide_port_range( );
		static Core::Status apply_wide_port_range( const bool& );

		[[nodiscard]] static Core::Result<bool> load_fast_port_recycling( );
		static Core::Status apply_fast_port_recycling( const bool& );

		[[nodiscard]] static Core::Result<bool> load_linux_like_ttl( );
		static Core::Status apply_linux_like_ttl( const bool& );

		[[nodiscard]] static Core::Result<bool> load_fast_name_resolution( );
		static Core::Status apply_fast_name_resolution( const bool& );

		[[nodiscard]] static Core::Result<bool> load_disable_ipv6( );
		static Core::Status apply_disable_ipv6( const bool& );

		[[nodiscard]] static Core::Result<bool> load_nic_power_saving( );
		static Core::Status apply_nic_power_saving( const bool& );

		[[nodiscard]] static Core::Result<bool> load_wake_on_lan( );
		static Core::Status apply_wake_on_lan( const bool& );

		[[nodiscard]] static Core::Result<bool> load_nic_offload_tuning( );
		static Core::Status apply_nic_offload_tuning( const bool& );

		[[nodiscard]] static Core::Result<bool> load_tcp_auto_tuning( );
		static Core::Status apply_tcp_auto_tuning( const bool& );

		[[nodiscard]] static Core::Result<bool> load_ecn_capability( );
		static Core::Status apply_ecn_capability( const bool& );

		[[nodiscard]] static Core::Result<bool> load_congestion_ctcp( );
		static Core::Status apply_congestion_ctcp( const bool& );

		[[nodiscard]] static Core::Result<bool> load_tcp_timestamps( );
		static Core::Status apply_tcp_timestamps( const bool& );

		[[nodiscard]] static Core::Result<bool> load_global_rss( );
		static Core::Status apply_global_rss( const bool& );

		[[nodiscard]] static Core::Result<bool> load_dns_over_https( );
		static Core::Status apply_dns_over_https( const bool& );

		[[nodiscard]] static Core::Result<bool> load_qos_bandwidth_limit( );
		static Core::Status apply_qos_bandwidth_limit( const bool& );
	};

	template<>
	struct FeatureTraits<NetworkFeature::Nagle>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &NetworkModule::load_nagle;
		static constexpr auto apply = &NetworkModule::apply_nagle;
	};

	template<>
	struct FeatureTraits<NetworkFeature::ActiveProbing>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &NetworkModule::load_active_probing;
		static constexpr auto apply = &NetworkModule::apply_active_probing;
	};

	template<>
	struct FeatureTraits<NetworkFeature::FastDns>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return false;
		}

		static constexpr auto load  = &NetworkModule::load_fast_dns;
		static constexpr auto apply = &NetworkModule::apply_fast_dns;
	};

	template<>
	struct FeatureTraits<NetworkFeature::WidePortRange>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return false;
		}

		static constexpr auto load  = &NetworkModule::load_wide_port_range;
		static constexpr auto apply = &NetworkModule::apply_wide_port_range;
	};

	template<>
	struct FeatureTraits<NetworkFeature::FastPortRecycling>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return false;
		}

		static constexpr auto load  = &NetworkModule::load_fast_port_recycling;
		static constexpr auto apply = &NetworkModule::apply_fast_port_recycling;
	};

	template<>
	struct FeatureTraits<NetworkFeature::LinuxLikeTtl>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return false;
		}

		static constexpr auto load  = &NetworkModule::load_linux_like_ttl;
		static constexpr auto apply = &NetworkModule::apply_linux_like_ttl;
	};

	template<>
	struct FeatureTraits<NetworkFeature::FastNameResolution>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return false;
		}

		static constexpr auto load  = &NetworkModule::load_fast_name_resolution;
		static constexpr auto apply = &NetworkModule::apply_fast_name_resolution;
	};

	template<>
	struct FeatureTraits<NetworkFeature::DisableIPv6>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return false;
		}

		static constexpr auto load  = &NetworkModule::load_disable_ipv6;
		static constexpr auto apply = &NetworkModule::apply_disable_ipv6;
	};

	template<>
	struct FeatureTraits<NetworkFeature::NicPowerSaving>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &NetworkModule::load_nic_power_saving;
		static constexpr auto apply = &NetworkModule::apply_nic_power_saving;
	};

	template<>
	struct FeatureTraits<NetworkFeature::WakeOnLan>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &NetworkModule::load_wake_on_lan;
		static constexpr auto apply = &NetworkModule::apply_wake_on_lan;
	};

	template<>
	struct FeatureTraits<NetworkFeature::NicOffloadTuning>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &NetworkModule::load_nic_offload_tuning;
		static constexpr auto apply = &NetworkModule::apply_nic_offload_tuning;
	};

	template<>
	struct FeatureTraits<NetworkFeature::TcpAutoTuning>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &NetworkModule::load_tcp_auto_tuning;
		static constexpr auto apply = &NetworkModule::apply_tcp_auto_tuning;
	};

	template<>
	struct FeatureTraits<NetworkFeature::EcnCapability>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return false;
		}

		static constexpr auto load  = &NetworkModule::load_ecn_capability;
		static constexpr auto apply = &NetworkModule::apply_ecn_capability;
	};

	template<>
	struct FeatureTraits<NetworkFeature::CongestionCtcp>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return false;
		}

		static constexpr auto load  = &NetworkModule::load_congestion_ctcp;
		static constexpr auto apply = &NetworkModule::apply_congestion_ctcp;
	};

	template<>
	struct FeatureTraits<NetworkFeature::TcpTimestamps>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return false;
		}

		static constexpr auto load  = &NetworkModule::load_tcp_timestamps;
		static constexpr auto apply = &NetworkModule::apply_tcp_timestamps;
	};

	template<>
	struct FeatureTraits<NetworkFeature::GlobalRss>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &NetworkModule::load_global_rss;
		static constexpr auto apply = &NetworkModule::apply_global_rss;
	};

	template<>
	struct FeatureTraits<NetworkFeature::DnsOverHttps>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return false;
		}

		static constexpr auto load  = &NetworkModule::load_dns_over_https;
		static constexpr auto apply = &NetworkModule::apply_dns_over_https;
	};

	template<>
	struct FeatureTraits<NetworkFeature::QosBandwidthLimit>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &NetworkModule::load_qos_bandwidth_limit;
		static constexpr auto apply = &NetworkModule::apply_qos_bandwidth_limit;
	};
}

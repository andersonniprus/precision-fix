#include "Stdafx.hpp"
#include "Modules/NetworkModule.hpp"
#include "Utils/Registry.hpp"

namespace
{
	constexpr wchar_t interfaces_key[ ]   = LR"(SYSTEM\CurrentControlSet\Services\Tcpip\Parameters\Interfaces)";
	constexpr wchar_t nla_internet_key[ ] = LR"(SYSTEM\CurrentControlSet\Services\NlaSvc\Parameters\Internet)";
	constexpr wchar_t fast_dns_servers[ ] = L"1.1.1.1,1.0.0.1";

	[[nodiscard]] std::wstring interface_key( const std::wstring_view guid )
	{
		return std::wstring { interfaces_key } + L'\\' + std::wstring { guid };
	}

	[[nodiscard]] Core::Status set_nagle_override( const std::wstring_view sub, const bool disabled )
	{
		if ( !disabled )
		{
			if ( const auto status = Utils::Registry::delete_value( HKEY_LOCAL_MACHINE, sub.data( ), L"TcpAckFrequency" ); !status )
				return status;

			if ( const auto status = Utils::Registry::delete_value( HKEY_LOCAL_MACHINE, sub.data( ), L"TCPNoDelay" ); !status )
				return status;

			return Utils::Registry::delete_value( HKEY_LOCAL_MACHINE, sub.data( ), L"TcpDelAckTicks" );
		}

		if ( const auto status = Utils::Registry::write_dword( HKEY_LOCAL_MACHINE, sub.data( ), L"TcpAckFrequency", 1 ); !status )
			return status;

		if ( const auto status = Utils::Registry::write_dword( HKEY_LOCAL_MACHINE, sub.data( ), L"TCPNoDelay", 1 ); !status )
			return status;

		return Utils::Registry::write_dword( HKEY_LOCAL_MACHINE, sub.data( ), L"TcpDelAckTicks", 0 );
	}

	constexpr wchar_t tcpip_parameters_key[ ]  = LR"(SYSTEM\CurrentControlSet\Services\Tcpip\Parameters)";
	constexpr wchar_t service_provider_key[ ]  = LR"(SYSTEM\CurrentControlSet\Services\Tcpip\ServiceProvider)";
	constexpr wchar_t tcpip6_parameters_key[ ] = LR"(SYSTEM\CurrentControlSet\Services\TCPIP6\Parameters)";
	constexpr wchar_t net_class_key[ ]         = LR"(SYSTEM\CurrentControlSet\Control\Class\{4D36E972-E325-11CE-BFC1-08002BE10318})";

	[[nodiscard]] Core::Result<std::vector<std::wstring>> find_network_adapter_keys( )
	{
		const auto subkeys = Utils::Registry::enumerate_subkeys( HKEY_LOCAL_MACHINE, net_class_key );

		if ( !subkeys )
			return std::unexpected( subkeys.error( ) );

		std::vector<std::wstring> matches;

		for ( const auto& name : *subkeys )
		{
			const auto sub = std::wstring { net_class_key } + L'\\' + name;

			if ( Utils::Registry::read_sz( HKEY_LOCAL_MACHINE, sub.c_str( ), L"NetCfgInstanceId" ) )
				matches.push_back( sub );
		}

		return matches;
	}

	constexpr const wchar_t* nic_power_saving_names[ ] = {
		L"*EEE", L"AdvancedEEE", L"EEE", L"EEELinkAdvertisement", L"EnablePME", L"AutoPowerSaveModeEnabled",
		L"DisabledelayedPowerUp", L"NicAutoPowerSaver", L"PowerDownPll", L"PowerSavingMode", L"EnableSavePowerNow",
		L"ReduceSpeedOnPowerDown", L"S5NicKeepOverrideMacAddrV2", L"EnableGreenEthernet", L"AutoDisablingGigabit",
		L"GigaLite", L"EnableDynamicPowerGating", L"EnableConnectedPowerGating", L"ULPMode", L"EnablePowerManagement",
	};

	constexpr const wchar_t* nic_power_saving_values[ ] = {
		L"0", L"0", L"0", L"0", L"0", L"0", L"2", L"2", L"0", L"0", L"0", L"0", L"0", L"0", L"0", L"0", L"0", L"0", L"0", L"0",
	};

	constexpr const wchar_t* wake_on_lan_names[ ] = {
		L"*WakeOnMagicPacket", L"*WakeOnPattern", L"S5WakeOnLan", L"WakeOnDisconnect", L"WakeOnLink", L"EnableWakeOnLan", L"WolShutdownLinkSpeed",
	};

	constexpr const wchar_t* wake_on_lan_values[ ] = {
		L"0", L"0", L"0", L"0", L"0", L"0", L"2",
	};

	constexpr const wchar_t* nic_offload_tuning_names[ ] = {
		L"RSS", L"RSSProfile", L"*NumRssQueues", L"IPChecksumOffloadIPv4", L"TCPChecksumOffloadIPv4", L"TCPChecksumOffloadIPv6",
		L"UDPChecksumOffloadIPv4", L"UDPChecksumOffloadIPv6", L"LsoV1IPv4", L"LsoV2IPv4", L"LsoV2IPv6", L"ReceiveBuffers",
		L"TransmitBuffers", L"*FlowControl", L"FlowControlCap", L"PMARPOffload", L"PMNSOffload", L"RxAbsIntDelay",
		L"TxAbsIntDelay", L"TxIntDelay", L"FatChannelIntolerant", L"JumboPacket",
	};

	constexpr const wchar_t* nic_offload_tuning_values[ ] = {
		L"1", L"3", L"2", L"0", L"0", L"0", L"0", L"0", L"0", L"0", L"0", L"1024",
		L"2048", L"0", L"0", L"0", L"0", L"0", L"0", L"0", L"0", L"1514",
	};

	[[nodiscard]] Core::Status apply_nic_bundle(
		const wchar_t* const* names, const wchar_t* const* values, const std::size_t count, const bool enabled
	)
	{
		const auto adapters = find_network_adapter_keys( );

		if ( !adapters )
			return std::unexpected( adapters.error( ) );

		if ( adapters->empty( ) )
			return std::unexpected( Core::Error::Unsupported );

		for ( const auto& sub : *adapters )
		{
			for ( std::size_t i = 0; i < count; ++i )
			{
				const auto status = enabled
				                    ? Utils::Registry::delete_value( HKEY_LOCAL_MACHINE, sub.c_str( ), names[ i ] )
				                    : Utils::Registry::write_sz( HKEY_LOCAL_MACHINE, sub.c_str( ), names[ i ], values[ i ] );

				if ( !status )
					return status;
			}
		}

		return {};
	}

	[[nodiscard]] Core::Result<bool> load_nic_bundle( const wchar_t* representative_name, const wchar_t* representative_value )
	{
		const auto adapters = find_network_adapter_keys( );

		if ( !adapters )
			return std::unexpected( adapters.error( ) );

		if ( adapters->empty( ) )
			return std::unexpected( Core::Error::Unsupported );

		const auto value = Utils::Registry::read_sz( HKEY_LOCAL_MACHINE, adapters->front( ).c_str( ), representative_name );

		if ( !value )
			return true;

		return *value != representative_value;
	}
}

namespace Modules
{
	NetworkModule::NetworkModule( )
	{
		refresh_all( );
	}

	Core::Result<bool> NetworkModule::load_nagle( )
	{
		const auto interfaces = Utils::Registry::enumerate_subkeys( HKEY_LOCAL_MACHINE, interfaces_key );

		if ( !interfaces )
			return std::unexpected( interfaces.error( ) );

		for ( const auto& guid : *interfaces )
		{
			const auto value = Utils::Registry::read_dword( HKEY_LOCAL_MACHINE, interface_key( guid ).c_str( ), L"TcpAckFrequency" );
			if ( value && *value == 1 )
				return false;
		}

		return true;
	}

	Core::Status NetworkModule::apply_nagle( const bool& enabled )
	{
		const auto interfaces = Utils::Registry::enumerate_subkeys( HKEY_LOCAL_MACHINE, interfaces_key );

		if ( !interfaces )
			return std::unexpected( interfaces.error( ) );

		for ( const auto& guid : *interfaces )
		{
			if ( const auto status = set_nagle_override( interface_key( guid ), !enabled ); !status )
				return status;
		}

		return {};
	}

	Core::Result<bool> NetworkModule::load_active_probing( )
	{
		const auto value = Utils::Registry::read_dword( HKEY_LOCAL_MACHINE, nla_internet_key, L"EnableActiveProbing" );

		if ( !value )
			return true;

		return *value != 0;
	}

	Core::Status NetworkModule::apply_active_probing( const bool& enabled )
	{
		return Utils::Registry::write_dword( HKEY_LOCAL_MACHINE, nla_internet_key, L"EnableActiveProbing", enabled ? 1u : 0u );
	}

	Core::Result<bool> NetworkModule::load_fast_dns( )
	{
		const auto interfaces = Utils::Registry::enumerate_subkeys( HKEY_LOCAL_MACHINE, interfaces_key );

		if ( !interfaces )
			return std::unexpected( interfaces.error( ) );

		for ( const auto& guid : *interfaces )
		{
			const auto value = Utils::Registry::read_sz( HKEY_LOCAL_MACHINE, interface_key( guid ).c_str( ), L"NameServer" );

			if ( value && *value == fast_dns_servers )
				return true;
		}

		return false;
	}

	Core::Status NetworkModule::apply_fast_dns( const bool& enabled )
	{
		const auto interfaces = Utils::Registry::enumerate_subkeys( HKEY_LOCAL_MACHINE, interfaces_key );

		if ( !interfaces )
			return std::unexpected( interfaces.error( ) );

		for ( const auto& guid : *interfaces )
		{
			const auto status = Utils::Registry::write_sz(
				HKEY_LOCAL_MACHINE, interface_key( guid ).c_str( ), L"NameServer", enabled ? fast_dns_servers : L""
			);

			if ( !status )
				return status;
		}

		return {};
	}

	Core::Result<bool> NetworkModule::load_wide_port_range( )
	{
		const auto value = Utils::Registry::read_dword( HKEY_LOCAL_MACHINE, tcpip_parameters_key, L"MaxUserPort" );

		if ( !value )
			return false;

		return *value == 65534;
	}

	Core::Status NetworkModule::apply_wide_port_range( const bool& enabled )
	{
		if ( enabled )
			return Utils::Registry::write_dword( HKEY_LOCAL_MACHINE, tcpip_parameters_key, L"MaxUserPort", 65534 );

		return Utils::Registry::delete_value( HKEY_LOCAL_MACHINE, tcpip_parameters_key, L"MaxUserPort" );
	}

	Core::Result<bool> NetworkModule::load_fast_port_recycling( )
	{
		const auto value = Utils::Registry::read_dword( HKEY_LOCAL_MACHINE, tcpip_parameters_key, L"TcpTimedWaitDelay" );

		if ( !value )
			return false;

		return *value == 30;
	}

	Core::Status NetworkModule::apply_fast_port_recycling( const bool& enabled )
	{
		if ( enabled )
			return Utils::Registry::write_dword( HKEY_LOCAL_MACHINE, tcpip_parameters_key, L"TcpTimedWaitDelay", 30 );

		return Utils::Registry::delete_value( HKEY_LOCAL_MACHINE, tcpip_parameters_key, L"TcpTimedWaitDelay" );
	}

	Core::Result<bool> NetworkModule::load_linux_like_ttl( )
	{
		const auto value = Utils::Registry::read_dword( HKEY_LOCAL_MACHINE, tcpip_parameters_key, L"DefaultTTL" );

		if ( !value )
			return false;

		return *value == 64;
	}

	Core::Status NetworkModule::apply_linux_like_ttl( const bool& enabled )
	{
		if ( enabled )
			return Utils::Registry::write_dword( HKEY_LOCAL_MACHINE, tcpip_parameters_key, L"DefaultTTL", 64 );

		return Utils::Registry::delete_value( HKEY_LOCAL_MACHINE, tcpip_parameters_key, L"DefaultTTL" );
	}

	Core::Result<bool> NetworkModule::load_fast_name_resolution( )
	{
		const auto value = Utils::Registry::read_dword( HKEY_LOCAL_MACHINE, service_provider_key, L"DnsPriority" );

		if ( !value )
			return false;

		return *value == 6;
	}

	Core::Status NetworkModule::apply_fast_name_resolution( const bool& enabled )
	{
		if ( !enabled )
		{
			if ( const auto status = Utils::Registry::delete_value( HKEY_LOCAL_MACHINE, service_provider_key, L"DnsPriority" ); !status )
				return status;

			if ( const auto status = Utils::Registry::delete_value( HKEY_LOCAL_MACHINE, service_provider_key, L"HostsPriority" ); !status )
				return status;

			if ( const auto status = Utils::Registry::delete_value( HKEY_LOCAL_MACHINE, service_provider_key, L"LocalPriority" ); !status )
				return status;

			return Utils::Registry::delete_value( HKEY_LOCAL_MACHINE, service_provider_key, L"NetbtPriority" );
		}

		if ( const auto status = Utils::Registry::write_dword( HKEY_LOCAL_MACHINE, service_provider_key, L"DnsPriority", 6 ); !status )
			return status;

		if ( const auto status = Utils::Registry::write_dword( HKEY_LOCAL_MACHINE, service_provider_key, L"HostsPriority", 5 ); !status )
			return status;

		if ( const auto status = Utils::Registry::write_dword( HKEY_LOCAL_MACHINE, service_provider_key, L"LocalPriority", 4 ); !status )
			return status;

		return Utils::Registry::write_dword( HKEY_LOCAL_MACHINE, service_provider_key, L"NetbtPriority", 7 );
	}

	Core::Result<bool> NetworkModule::load_disable_ipv6( )
	{
		const auto value = Utils::Registry::read_dword( HKEY_LOCAL_MACHINE, tcpip6_parameters_key, L"DisabledComponents" );

		if ( !value )
			return false;

		return *value != 0;
	}

	Core::Status NetworkModule::apply_disable_ipv6( const bool& enabled )
	{
		if ( enabled )
			return Utils::Registry::write_dword( HKEY_LOCAL_MACHINE, tcpip6_parameters_key, L"DisabledComponents", 0xFF );

		return Utils::Registry::delete_value( HKEY_LOCAL_MACHINE, tcpip6_parameters_key, L"DisabledComponents" );
	}

	Core::Result<bool> NetworkModule::load_nic_power_saving( )
	{
		return load_nic_bundle( L"*EEE", L"0" );
	}

	Core::Status NetworkModule::apply_nic_power_saving( const bool& enabled )
	{
		return apply_nic_bundle( nic_power_saving_names, nic_power_saving_values, std::size( nic_power_saving_names ), enabled );
	}

	Core::Result<bool> NetworkModule::load_wake_on_lan( )
	{
		return load_nic_bundle( L"EnableWakeOnLan", L"0" );
	}

	Core::Status NetworkModule::apply_wake_on_lan( const bool& enabled )
	{
		return apply_nic_bundle( wake_on_lan_names, wake_on_lan_values, std::size( wake_on_lan_names ), enabled );
	}

	Core::Result<bool> NetworkModule::load_nic_offload_tuning( )
	{
		return load_nic_bundle( L"RSS", L"1" );
	}

	Core::Status NetworkModule::apply_nic_offload_tuning( const bool& enabled )
	{
		return apply_nic_bundle( nic_offload_tuning_names, nic_offload_tuning_values, std::size( nic_offload_tuning_names ), enabled );
	}
}

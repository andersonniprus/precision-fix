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
}

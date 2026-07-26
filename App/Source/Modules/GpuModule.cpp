#include "Stdafx.hpp"
#include "Modules/GpuModule.hpp"
#include "Utils/Registry.hpp"

namespace
{
	constexpr wchar_t scheduler_key[ ]     = LR"(SYSTEM\CurrentControlSet\Control\GraphicsDrivers\Scheduler)";
	constexpr wchar_t display_class_key[ ] = LR"(SYSTEM\CurrentControlSet\Control\Class\{4d36e968-e325-11ce-bfc1-08002be10318})";

	[[nodiscard]] bool contains_ci( const std::wstring_view haystack, const std::wstring_view needle )
	{
		const auto upper = []( const wchar_t c ) noexcept
		{
			return static_cast<wchar_t>( std::towupper( c ) );
		};

		std::wstring hs( haystack.size( ), L'\0' );
		std::wstring nd( needle.size( ), L'\0' );

		std::ranges::transform( haystack, hs.begin( ), upper );
		std::ranges::transform( needle, nd.begin( ), upper );

		return hs.find( nd ) != std::wstring::npos;
	}

	[[nodiscard]] Core::Result<std::vector<std::wstring>> find_nvidia_adapter_keys( )
	{
		const auto adapters = Utils::Registry::enumerate_subkeys( HKEY_LOCAL_MACHINE, display_class_key );

		if ( !adapters )
			return std::unexpected( adapters.error( ) );

		std::vector<std::wstring> matches;

		for ( const auto& name : *adapters )
		{
			const auto sub = std::wstring { display_class_key } + L'\\' + name;

			const auto provider = Utils::Registry::read_sz( HKEY_LOCAL_MACHINE, sub.c_str( ), L"ProviderName" );

			if ( provider && contains_ci( *provider, L"NVIDIA" ) )
				matches.push_back( sub );
		}

		return matches;
	}
}

namespace Modules
{
	GpuModule::GpuModule( )
	{
		refresh_all( );
	}

	Core::Result<bool> GpuModule::load_preemption( )
	{
		const auto value = Utils::Registry::read_dword( HKEY_LOCAL_MACHINE, scheduler_key, L"DisablePreemption" );

		if ( !value )
			return true;

		return *value == 0;
	}

	Core::Status GpuModule::apply_preemption( const bool& enabled )
	{
		if ( enabled )
			return Utils::Registry::delete_value( HKEY_LOCAL_MACHINE, scheduler_key, L"DisablePreemption" );

		return Utils::Registry::write_dword( HKEY_LOCAL_MACHINE, scheduler_key, L"DisablePreemption", 1 );
	}

	Core::Result<bool> GpuModule::load_hdcp( )
	{
		const auto adapters = find_nvidia_adapter_keys( );

		if ( !adapters )
			return std::unexpected( adapters.error( ) );

		if ( adapters->empty( ) )
			return std::unexpected( Core::Error::Unsupported );

		const auto value = Utils::Registry::read_dword( HKEY_LOCAL_MACHINE, adapters->front( ).c_str( ), L"RMHdcpKeyglobZero" );

		if ( !value )
			return true;

		return *value == 0;
	}

	Core::Status GpuModule::apply_hdcp( const bool& enabled )
	{
		const auto adapters = find_nvidia_adapter_keys( );

		if ( !adapters )
			return std::unexpected( adapters.error( ) );

		if ( adapters->empty( ) )
			return std::unexpected( Core::Error::Unsupported );

		for ( const auto& sub : *adapters )
		{
			if ( const auto status = Utils::Registry::write_dword( HKEY_LOCAL_MACHINE, sub.c_str( ), L"RMHdcpKeyglobZero", enabled ? 0u : 1u );
				!status )
				return status;
		}

		return {};
	}
}

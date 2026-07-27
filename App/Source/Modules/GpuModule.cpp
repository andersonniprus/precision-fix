#include "Stdafx.hpp"
#include "Modules/GpuModule.hpp"
#include "Utils/Registry.hpp"

namespace
{
	constexpr wchar_t scheduler_key[ ]              = LR"(SYSTEM\CurrentControlSet\Control\GraphicsDrivers\Scheduler)";
	constexpr wchar_t graphics_drivers_key[ ]       = LR"(SYSTEM\CurrentControlSet\Control\GraphicsDrivers)";
	constexpr wchar_t graphics_drivers_power_key[ ] = LR"(SYSTEM\CurrentControlSet\Control\GraphicsDrivers\Power)";
	constexpr wchar_t power_key[ ]                  = LR"(SYSTEM\CurrentControlSet\Control\Power)";
	constexpr wchar_t nvlddmkm_key[ ]               = LR"(SYSTEM\CurrentControlSet\Services\nvlddmkm)";
	constexpr wchar_t display_class_key[ ]          = LR"(SYSTEM\CurrentControlSet\Control\Class\{4d36e968-e325-11ce-bfc1-08002be10318})";

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

	[[nodiscard]] Core::Result<std::vector<std::wstring>> find_adapter_keys( const std::wstring_view vendor_substring )
	{
		const auto adapters = Utils::Registry::enumerate_subkeys( HKEY_LOCAL_MACHINE, display_class_key );

		if ( !adapters )
			return std::unexpected( adapters.error( ) );

		std::vector<std::wstring> matches;

		for ( const auto& name : *adapters )
		{
			const auto sub = std::wstring { display_class_key } + L'\\' + name;

			const auto provider = Utils::Registry::read_sz( HKEY_LOCAL_MACHINE, sub.c_str( ), L"ProviderName" );

			if ( provider && contains_ci( *provider, vendor_substring ) )
				matches.push_back( sub );
		}

		return matches;
	}

	constexpr const wchar_t* nvidia_preemption_values[ ] = {
		L"DisablePreemption", L"DisableCudaContextPreemption", L"DisablePreemptionOnS3S4", L"EnableCEPreemption", L"ComputePreemption",
	};

	constexpr const wchar_t* amd_power_gating_values[ ] = {
		L"DisableSAMUPowerGating",
		L"DisableUVDPowerGatingDynamic",
		L"DisableVCEPowerGating",
		L"DisabledrmdmaPowerGating",
		L"PP_GPUPowerDownEnabled",
		L"PP_SclkDeepSleepDisable",
		L"EnableUlps",
	};

	constexpr std::uint32_t amd_power_gating_defaults[ ] = { 0, 0, 0, 0, 1, 0, 1 };

	constexpr std::uint32_t amd_power_gating_tweaks[ ] = { 1, 1, 1, 1, 0, 1, 0 };

	constexpr const wchar_t* latency_tolerance_values[ ] = {
		L"DefaultD3TransitionLatencyIdleLongTime", L"DefaultD3TransitionLatencyIdleMonitorOff",
		L"DefaultD3TransitionLatencyIdleNoContext", L"DefaultD3TransitionLatencyIdleShortTime",
		L"DefaultD3TransitionLatencyIdleVeryLongTime", L"DefaultLatencyToleranceIdle0",
		L"DefaultLatencyToleranceIdle0MonitorOff", L"DefaultLatencyToleranceIdle1",
		L"DefaultLatencyToleranceIdle1MonitorOff", L"DefaultLatencyToleranceMemory",
		L"DefaultLatencyToleranceNoContext", L"DefaultLatencyToleranceNoContextMonitorOff",
		L"DefaultLatencyToleranceOther", L"DefaultLatencyToleranceTimerPeriod",
		L"DefaultMemoryRefreshLatencyToleranceActivelyUsed", L"DefaultMemoryRefreshLatencyToleranceMonitorOff",
		L"DefaultMemoryRefreshLatencyToleranceNoContext", L"ExitLatency", L"ExitLatencyCheckEnabled", L"Latency",
		L"LatencyToleranceDefault", L"LatencyToleranceFSVP", L"LatencyTolerancePerfOverride",
		L"LatencyToleranceScreenOffIR", L"LatencyToleranceVSyncEnabled", L"MiracastPerfTrackGraphicsLatency",
		L"MonitorLatencyTolerance", L"MonitorRefreshLatencyTolerance", L"RtlCapabilityCheckLatency", L"TransitionLatency",
	};

	constexpr const wchar_t* latency_tolerance_keys[ ] = { graphics_drivers_power_key, power_key };
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
		const auto adapters = find_adapter_keys( L"NVIDIA" );

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
		const auto adapters = find_adapter_keys( L"NVIDIA" );

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

	Core::Result<bool> GpuModule::load_hardware_gpu_scheduling( )
	{
		const auto value = Utils::Registry::read_dword( HKEY_LOCAL_MACHINE, graphics_drivers_key, L"HwSchMode" );

		if ( !value )
			return false;

		return *value == 2;
	}

	Core::Status GpuModule::apply_hardware_gpu_scheduling( const bool& enabled )
	{
		return Utils::Registry::write_dword( HKEY_LOCAL_MACHINE, graphics_drivers_key, L"HwSchMode", enabled ? 2u : 1u );
	}

	Core::Result<bool> GpuModule::load_nvidia_preemption_override( )
	{
		const auto key = Utils::Registry::open( HKEY_LOCAL_MACHINE, nvlddmkm_key, KEY_READ );

		if ( !key )
			return std::unexpected( Core::Error::Unsupported );

		const auto value = Utils::Registry::read_dword( HKEY_LOCAL_MACHINE, nvlddmkm_key, nvidia_preemption_values[ 0 ] );

		if ( !value )
			return true;

		return *value == 0;
	}

	Core::Status GpuModule::apply_nvidia_preemption_override( const bool& enabled )
	{
		const auto key = Utils::Registry::open( HKEY_LOCAL_MACHINE, nvlddmkm_key, KEY_READ );

		if ( !key )
			return std::unexpected( Core::Error::Unsupported );

		for ( const auto* name : nvidia_preemption_values )
		{
			const auto status = enabled
			                    ? Utils::Registry::delete_value( HKEY_LOCAL_MACHINE, nvlddmkm_key, name )
			                    : Utils::Registry::write_dword( HKEY_LOCAL_MACHINE, nvlddmkm_key, name, 1 );

			if ( !status )
				return status;
		}

		return {};
	}

	Core::Result<bool> GpuModule::load_amd_power_gating_disabled( )
	{
		const auto adapters = find_adapter_keys( L"Advanced Micro Devices" );

		if ( !adapters )
			return std::unexpected( adapters.error( ) );

		if ( adapters->empty( ) )
			return std::unexpected( Core::Error::Unsupported );

		const auto value = Utils::Registry::read_dword( HKEY_LOCAL_MACHINE, adapters->front( ).c_str( ), L"PP_GPUPowerDownEnabled" );

		if ( !value )
			return true;

		return *value != 0;
	}

	Core::Status GpuModule::apply_amd_power_gating_disabled( const bool& enabled )
	{
		const auto adapters = find_adapter_keys( L"Advanced Micro Devices" );

		if ( !adapters )
			return std::unexpected( adapters.error( ) );

		if ( adapters->empty( ) )
			return std::unexpected( Core::Error::Unsupported );

		for ( const auto& sub : *adapters )
		{
			for ( std::size_t i = 0; i < std::size( amd_power_gating_values ); ++i )
			{
				const auto value = enabled ? amd_power_gating_defaults[ i ] : amd_power_gating_tweaks[ i ];

				if ( const auto status = Utils::Registry::write_dword( HKEY_LOCAL_MACHINE, sub.c_str( ), amd_power_gating_values[ i ], value );
					!status )
					return status;
			}
		}

		return {};
	}

	Core::Result<bool> GpuModule::load_graphics_latency_tolerance( )
	{
		const auto value = Utils::Registry::read_dword( HKEY_LOCAL_MACHINE, graphics_drivers_power_key, L"Latency" );

		if ( !value )
			return false;

		return *value != 0;
	}

	Core::Status GpuModule::apply_graphics_latency_tolerance( const bool& enabled )
	{
		for ( const auto* key : latency_tolerance_keys )
		{
			for ( const auto* name : latency_tolerance_values )
			{
				const auto status = enabled
				                    ? Utils::Registry::write_dword( HKEY_LOCAL_MACHINE, key, name, 1 )
				                    : Utils::Registry::delete_value( HKEY_LOCAL_MACHINE, key, name );

				if ( !status )
					return status;
			}
		}

		return {};
	}
}

#pragma once

#include "Modules/ModuleBase.hpp"

namespace Modules
{
	enum class GpuFeature : std::uint8_t
	{
		Preemption,
		Hdcp,
		HardwareGpuScheduling,
		NvidiaPreemptionOverride,
		AmdPowerGatingDisabled,
		GraphicsLatencyTolerance,
		MultiPlaneOverlays,
		Count,
	};

	class GpuModule final : public ModuleBase<GpuModule, GpuFeature>
	{
	public:
		GpuModule( );

	private:
		template<auto>
		friend struct FeatureTraits;

		[[nodiscard]] static Core::Result<bool> load_preemption( );
		static Core::Status apply_preemption( const bool& );

		[[nodiscard]] static Core::Result<bool> load_hdcp( );
		static Core::Status apply_hdcp( const bool& );

		[[nodiscard]] static Core::Result<bool> load_hardware_gpu_scheduling( );
		static Core::Status apply_hardware_gpu_scheduling( const bool& );

		[[nodiscard]] static Core::Result<bool> load_nvidia_preemption_override( );
		static Core::Status apply_nvidia_preemption_override( const bool& );

		[[nodiscard]] static Core::Result<bool> load_amd_power_gating_disabled( );
		static Core::Status apply_amd_power_gating_disabled( const bool& );

		[[nodiscard]] static Core::Result<bool> load_graphics_latency_tolerance( );
		static Core::Status apply_graphics_latency_tolerance( const bool& );

		[[nodiscard]] static Core::Result<bool> load_multi_plane_overlays( );
		static Core::Status apply_multi_plane_overlays( const bool& );
	};

	template<>
	struct FeatureTraits<GpuFeature::Preemption>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &GpuModule::load_preemption;
		static constexpr auto apply = &GpuModule::apply_preemption;
	};

	template<>
	struct FeatureTraits<GpuFeature::Hdcp>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &GpuModule::load_hdcp;
		static constexpr auto apply = &GpuModule::apply_hdcp;
	};

	template<>
	struct FeatureTraits<GpuFeature::HardwareGpuScheduling>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return false;
		}

		static constexpr auto load  = &GpuModule::load_hardware_gpu_scheduling;
		static constexpr auto apply = &GpuModule::apply_hardware_gpu_scheduling;
	};

	template<>
	struct FeatureTraits<GpuFeature::NvidiaPreemptionOverride>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &GpuModule::load_nvidia_preemption_override;
		static constexpr auto apply = &GpuModule::apply_nvidia_preemption_override;
	};

	template<>
	struct FeatureTraits<GpuFeature::AmdPowerGatingDisabled>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &GpuModule::load_amd_power_gating_disabled;
		static constexpr auto apply = &GpuModule::apply_amd_power_gating_disabled;
	};

	template<>
	struct FeatureTraits<GpuFeature::GraphicsLatencyTolerance>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return false;
		}

		static constexpr auto load  = &GpuModule::load_graphics_latency_tolerance;
		static constexpr auto apply = &GpuModule::apply_graphics_latency_tolerance;
	};

	template<>
	struct FeatureTraits<GpuFeature::MultiPlaneOverlays>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &GpuModule::load_multi_plane_overlays;
		static constexpr auto apply = &GpuModule::apply_multi_plane_overlays;
	};
}

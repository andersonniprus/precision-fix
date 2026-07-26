#pragma once

#include "Modules/ModuleBase.hpp"

namespace Modules
{
	enum class GpuFeature : std::uint8_t
	{
		Preemption,
		Hdcp,
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
}

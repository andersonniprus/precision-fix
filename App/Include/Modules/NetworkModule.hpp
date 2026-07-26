#pragma once

#include "Modules/ModuleBase.hpp"

namespace Modules
{
	enum class NetworkFeature : std::uint8_t
	{
		Nagle,
		ActiveProbing,
		FastDns,
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
}

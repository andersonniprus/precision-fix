#pragma once

#include "Modules/ModuleBase.hpp"

namespace Modules
{
	enum class SystemFeature : std::uint8_t
	{
		PrioritySeparation,
		Responsiveness,
		NetworkThrottling,
		Prefetch,
		PowerThrottling,
		TimerCoalescing,
		ProcessorIdleStates,
		Tsx,
		ForegroundLockTimeout,
		MenuShowDelay,
		UiAnimations,
		Count,
	};

	class SystemModule final : public ModuleBase<SystemModule, SystemFeature>
	{
	public:
		SystemModule( );

	private:
		template<auto>
		friend struct FeatureTraits;

		[[nodiscard]] static Core::Result<std::uint32_t> load_priority_separation( );
		static Core::Status apply_priority_separation( const std::uint32_t& );

		[[nodiscard]] static Core::Result<std::uint32_t> load_responsiveness( );
		static Core::Status apply_responsiveness( const std::uint32_t& );

		[[nodiscard]] static Core::Result<std::uint32_t> load_network_throttling( );
		static Core::Status apply_network_throttling( const std::uint32_t& );

		[[nodiscard]] static Core::Result<bool> load_prefetch( );
		static Core::Status apply_prefetch( const bool& );

		[[nodiscard]] static Core::Result<bool> load_power_throttling( );
		static Core::Status apply_power_throttling( const bool& );

		[[nodiscard]] static Core::Result<bool> load_timer_coalescing( );
		static Core::Status apply_timer_coalescing( const bool& );

		[[nodiscard]] static Core::Result<bool> load_processor_idle_states( );
		static Core::Status apply_processor_idle_states( const bool& );

		[[nodiscard]] static Core::Result<bool> load_tsx( );
		static Core::Status apply_tsx( const bool& );

		[[nodiscard]] static Core::Result<std::uint32_t> load_foreground_lock_timeout( );
		static Core::Status apply_foreground_lock_timeout( const std::uint32_t& );

		[[nodiscard]] static Core::Result<std::uint32_t> load_menu_show_delay( );
		static Core::Status apply_menu_show_delay( const std::uint32_t& );

		[[nodiscard]] static Core::Result<bool> load_ui_animations( );
		static Core::Status apply_ui_animations( const bool& );
	};

	template<>
	struct FeatureTraits<SystemFeature::PrioritySeparation>
	{
		using value_type = std::uint32_t;

		static constexpr value_type fallback( ) noexcept
		{
			return 2;
		}

		static constexpr auto load  = &SystemModule::load_priority_separation;
		static constexpr auto apply = &SystemModule::apply_priority_separation;
	};

	template<>
	struct FeatureTraits<SystemFeature::Responsiveness>
	{
		using value_type = std::uint32_t;

		static constexpr value_type fallback( ) noexcept
		{
			return 20;
		}

		static constexpr auto load  = &SystemModule::load_responsiveness;
		static constexpr auto apply = &SystemModule::apply_responsiveness;
	};

	template<>
	struct FeatureTraits<SystemFeature::NetworkThrottling>
	{
		using value_type = std::uint32_t;

		static constexpr value_type fallback( ) noexcept
		{
			return 0xA;
		}

		static constexpr auto load  = &SystemModule::load_network_throttling;
		static constexpr auto apply = &SystemModule::apply_network_throttling;
	};

	template<>
	struct FeatureTraits<SystemFeature::Prefetch>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &SystemModule::load_prefetch;
		static constexpr auto apply = &SystemModule::apply_prefetch;
	};

	template<>
	struct FeatureTraits<SystemFeature::PowerThrottling>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &SystemModule::load_power_throttling;
		static constexpr auto apply = &SystemModule::apply_power_throttling;
	};

	template<>
	struct FeatureTraits<SystemFeature::TimerCoalescing>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &SystemModule::load_timer_coalescing;
		static constexpr auto apply = &SystemModule::apply_timer_coalescing;
	};

	template<>
	struct FeatureTraits<SystemFeature::ProcessorIdleStates>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &SystemModule::load_processor_idle_states;
		static constexpr auto apply = &SystemModule::apply_processor_idle_states;
	};

	template<>
	struct FeatureTraits<SystemFeature::Tsx>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &SystemModule::load_tsx;
		static constexpr auto apply = &SystemModule::apply_tsx;
	};

	template<>
	struct FeatureTraits<SystemFeature::ForegroundLockTimeout>
	{
		using value_type = std::uint32_t;

		static constexpr value_type fallback( ) noexcept
		{
			return 200000;
		}

		static constexpr auto load  = &SystemModule::load_foreground_lock_timeout;
		static constexpr auto apply = &SystemModule::apply_foreground_lock_timeout;
	};

	template<>
	struct FeatureTraits<SystemFeature::MenuShowDelay>
	{
		using value_type = std::uint32_t;

		static constexpr value_type fallback( ) noexcept
		{
			return 400;
		}

		static constexpr auto load  = &SystemModule::load_menu_show_delay;
		static constexpr auto apply = &SystemModule::apply_menu_show_delay;
	};

	template<>
	struct FeatureTraits<SystemFeature::UiAnimations>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &SystemModule::load_ui_animations;
		static constexpr auto apply = &SystemModule::apply_ui_animations;
	};
}

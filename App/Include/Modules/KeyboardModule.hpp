#pragma once

#include "Modules/ModuleBase.hpp"

namespace Modules
{
	enum class KeyboardFeature : std::uint8_t
	{
		FilterKeys,
		StickyKeys,
		ToggleKeys,
		RepeatDelay,
		RepeatRate,
		UsbSelectiveSuspend,
		Count,
	};

	class KeyboardModule final : public ModuleBase<KeyboardModule, KeyboardFeature>
	{
	public:
		KeyboardModule( );

	private:
		template<auto>
		friend struct FeatureTraits;

		[[nodiscard]] Core::Result<bool> load_filter_keys( ) const;
		Core::Status apply_filter_keys( const bool& );

		[[nodiscard]] Core::Result<bool> load_sticky_keys( ) const;
		Core::Status apply_sticky_keys( const bool& );

		[[nodiscard]] Core::Result<bool> load_toggle_keys( ) const;
		Core::Status apply_toggle_keys( const bool& );

		[[nodiscard]] Core::Result<std::uint32_t> load_repeat_delay( ) const;
		Core::Status apply_repeat_delay( const std::uint32_t& );

		[[nodiscard]] Core::Result<std::uint32_t> load_repeat_rate( ) const;
		Core::Status apply_repeat_rate( const std::uint32_t& );

		[[nodiscard]] Core::Result<bool> load_usb_selective_suspend( ) const;
		Core::Status apply_usb_selective_suspend( const bool& );
	};

	template<>
	struct FeatureTraits<KeyboardFeature::FilterKeys>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return false;
		}

		static constexpr auto load  = &KeyboardModule::load_filter_keys;
		static constexpr auto apply = &KeyboardModule::apply_filter_keys;
	};

	template<>
	struct FeatureTraits<KeyboardFeature::StickyKeys>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return false;
		}

		static constexpr auto load  = &KeyboardModule::load_sticky_keys;
		static constexpr auto apply = &KeyboardModule::apply_sticky_keys;
	};

	template<>
	struct FeatureTraits<KeyboardFeature::ToggleKeys>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return false;
		}

		static constexpr auto load  = &KeyboardModule::load_toggle_keys;
		static constexpr auto apply = &KeyboardModule::apply_toggle_keys;
	};

	template<>
	struct FeatureTraits<KeyboardFeature::RepeatDelay>
	{
		using value_type = std::uint32_t;

		static constexpr value_type fallback( ) noexcept
		{
			return 1;
		}

		static constexpr auto load  = &KeyboardModule::load_repeat_delay;
		static constexpr auto apply = &KeyboardModule::apply_repeat_delay;
	};

	template<>
	struct FeatureTraits<KeyboardFeature::RepeatRate>
	{
		using value_type = std::uint32_t;

		static constexpr value_type fallback( ) noexcept
		{
			return 31;
		}

		static constexpr auto load  = &KeyboardModule::load_repeat_rate;
		static constexpr auto apply = &KeyboardModule::apply_repeat_rate;
	};

	template<>
	struct FeatureTraits<KeyboardFeature::UsbSelectiveSuspend>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &KeyboardModule::load_usb_selective_suspend;
		static constexpr auto apply = &KeyboardModule::apply_usb_selective_suspend;
	};
}

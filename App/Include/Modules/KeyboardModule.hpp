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
		DataQueueSize,
		UsbSelectiveSuspendGlobal,
		MouseKeys,
		Count,
	};

	class KeyboardModule final : public ModuleBase<KeyboardModule, KeyboardFeature>
	{
	public:
		KeyboardModule( );

	private:
		template<auto>
		friend struct FeatureTraits;

		[[nodiscard]] static Core::Result<bool> load_filter_keys( );
		static Core::Status apply_filter_keys( const bool& );

		[[nodiscard]] static Core::Result<bool> load_sticky_keys( );
		static Core::Status apply_sticky_keys( const bool& );

		[[nodiscard]] static Core::Result<bool> load_toggle_keys( );
		static Core::Status apply_toggle_keys( const bool& );

		[[nodiscard]] static Core::Result<std::uint32_t> load_repeat_delay( );
		static Core::Status apply_repeat_delay( const std::uint32_t& );

		[[nodiscard]] static Core::Result<std::uint32_t> load_repeat_rate( );
		static Core::Status apply_repeat_rate( const std::uint32_t& );

		[[nodiscard]] static Core::Result<bool> load_usb_selective_suspend( );
		static Core::Status apply_usb_selective_suspend( const bool& );

		[[nodiscard]] static Core::Result<std::uint32_t> load_data_queue_size( );
		static Core::Status apply_data_queue_size( const std::uint32_t& );

		[[nodiscard]] static Core::Result<bool> load_usb_selective_suspend_global( );
		static Core::Status apply_usb_selective_suspend_global( const bool& );

		[[nodiscard]] static Core::Result<bool> load_mouse_keys( );
		static Core::Status apply_mouse_keys( const bool& );
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

	template<>
	struct FeatureTraits<KeyboardFeature::DataQueueSize>
	{
		using value_type = std::uint32_t;

		static constexpr value_type fallback( ) noexcept
		{
			return 100;
		}

		static constexpr auto load  = &KeyboardModule::load_data_queue_size;
		static constexpr auto apply = &KeyboardModule::apply_data_queue_size;
	};

	template<>
	struct FeatureTraits<KeyboardFeature::UsbSelectiveSuspendGlobal>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &KeyboardModule::load_usb_selective_suspend_global;
		static constexpr auto apply = &KeyboardModule::apply_usb_selective_suspend_global;
	};

	template<>
	struct FeatureTraits<KeyboardFeature::MouseKeys>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return false;
		}

		static constexpr auto load  = &KeyboardModule::load_mouse_keys;
		static constexpr auto apply = &KeyboardModule::apply_mouse_keys;
	};
}

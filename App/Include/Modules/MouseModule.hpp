#pragma once

#include "Modules/ModuleBase.hpp"

namespace Modules
{
	enum class MouseFeature : std::uint8_t
	{
		PointerPrecision,
		SmoothMouseXCurve,
		SmoothMouseYCurve,
		DataQueueSize,
		PointerSpeed,
		Count,
	};

	class MouseModule final : public ModuleBase<MouseModule, MouseFeature>
	{
	public:
		MouseModule( );

	private:
		template<auto>
		friend struct FeatureTraits;

		[[nodiscard]] static Core::Result<bool> load_pointer_precision( );
		static Core::Status apply_pointer_precision( const bool& );

		[[nodiscard]] static Core::Result<CurveData> load_smooth_x_curve( );
		static Core::Status apply_smooth_x_curve( const CurveData& );

		[[nodiscard]] static Core::Result<CurveData> load_smooth_y_curve( );
		static Core::Status apply_smooth_y_curve( const CurveData& );

		[[nodiscard]] static Core::Result<std::uint32_t> load_data_queue_size( );
		static Core::Status apply_data_queue_size( const std::uint32_t& );

		[[nodiscard]] static Core::Result<std::uint32_t> load_pointer_speed( );
		static Core::Status apply_pointer_speed( const std::uint32_t& );
	};

	template<>
	struct FeatureTraits<MouseFeature::PointerPrecision>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &MouseModule::load_pointer_precision;
		static constexpr auto apply = &MouseModule::apply_pointer_precision;
	};

	template<>
	struct FeatureTraits<MouseFeature::SmoothMouseXCurve>
	{
		using value_type = CurveData;

		static constexpr value_type fallback( ) noexcept
		{
			return {
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x15, 0x6E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x40, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x29, 0xDC, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00,
			};
		}

		static constexpr auto load  = &MouseModule::load_smooth_x_curve;
		static constexpr auto apply = &MouseModule::apply_smooth_x_curve;
	};

	template<>
	struct FeatureTraits<MouseFeature::SmoothMouseYCurve>
	{
		using value_type = CurveData;

		static constexpr value_type fallback( ) noexcept
		{
			return {
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0xB8, 0x5E, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
				0xCD, 0x4C, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00,
				0xCD, 0x4C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x38, 0x02, 0x00, 0x00, 0x00, 0x00,
			};
		}

		static constexpr auto load  = &MouseModule::load_smooth_y_curve;
		static constexpr auto apply = &MouseModule::apply_smooth_y_curve;
	};

	template<>
	struct FeatureTraits<MouseFeature::DataQueueSize>
	{
		using value_type = std::uint32_t;

		static constexpr value_type fallback( ) noexcept
		{
			return 100;
		}

		static constexpr auto load  = &MouseModule::load_data_queue_size;
		static constexpr auto apply = &MouseModule::apply_data_queue_size;
	};

	template<>
	struct FeatureTraits<MouseFeature::PointerSpeed>
	{
		using value_type = std::uint32_t;

		static constexpr value_type fallback( ) noexcept
		{
			return 10;
		}

		static constexpr auto load  = &MouseModule::load_pointer_speed;
		static constexpr auto apply = &MouseModule::apply_pointer_speed;
	};
}

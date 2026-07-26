#pragma once

namespace Modules
{
	using CurveData = std::array<std::uint8_t, 40>;

	using FeatureValue = std::variant<bool, std::uint32_t, CurveData>;

	template<auto Feature>
	struct FeatureTraits;

	template<typename Derived, typename FeatureEnum>
	class ModuleBase
	{
		static constexpr std::size_t count = static_cast<std::size_t>( std::to_underlying(
			FeatureEnum::Count ) );

	public:
		template<FeatureEnum F>
		[[nodiscard]] const FeatureTraits<F>::value_type& get( ) const noexcept
		{
			return std::get<typename FeatureTraits<F>::value_type>( values_[ index<F>( ) ] );
		}

		template<FeatureEnum F>
		Core::Status set( const FeatureTraits<F>::value_type& value )
		{
			if ( auto status = FeatureTraits<F>::apply( value ); !status )
				return status;

			values_[ index<F>( ) ] = value;

			return {};
		}

		void refresh_all( ) noexcept
		{
			[this]<std::size_t... I>( std::index_sequence<I...> ) noexcept
			{
				( load<static_cast<FeatureEnum>( I )>( ), ... );
			}( std::make_index_sequence<count> {} );
		}

	protected:
		ModuleBase( ) = default;

	private:
		template<FeatureEnum F>
		[[nodiscard]] static constexpr std::size_t index( ) noexcept
		{
			return static_cast<std::size_t>( std::to_underlying( F ) );
		}

		template<FeatureEnum F>
		void load( ) noexcept
		{
			values_[ index<F>( ) ] = FeatureTraits<F>::load( ).value_or( FeatureTraits<F>::fallback( ) );
		}

		std::array<FeatureValue, count> values_ {};
	};
}

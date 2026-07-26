#include "Stdafx.hpp"
#include "Modules/MouseModule.hpp"
#include "Utils/Registry.hpp"

namespace
{
	constexpr wchar_t mouse_key[ ]      = LR"(Control Panel\Mouse)";
	constexpr wchar_t queue_size_key[ ] = LR"(SYSTEM\CurrentControlSet\Services\mouclass\Parameters)";

	constexpr std::size_t curve_byte_count = 40;

	constexpr int precision_on[ 3 ]  = { 6, 10, 1 };
	constexpr int precision_off[ 3 ] = { 0, 0, 0 };

	[[nodiscard]] Core::Status reload_ballistics( )
	{
		int current[ 3 ] {};

		if ( !SystemParametersInfoW( SPI_GETMOUSE, 0, current, 0 ) )
			return std::unexpected( Core::Error::Unknown );

		if ( !SystemParametersInfoW( SPI_SETMOUSE, 0, current, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE ) )
			return std::unexpected( Core::Error::Unknown );

		return {};
	}

	[[nodiscard]] Core::Result<Modules::CurveData> load_curve( const wchar_t* value )
	{
		const auto data = Utils::Registry::read_binary( HKEY_CURRENT_USER, mouse_key, value );

		if ( !data )
			return std::unexpected( data.error( ) );

		if ( data->size( ) != curve_byte_count )
			return std::unexpected( Core::Error::InvalidValue );

		Modules::CurveData curve;
		std::memcpy( curve.data( ), data->data( ), curve_byte_count );

		return curve;
	}

	[[nodiscard]] Core::Status apply_curve( const wchar_t* value, const Modules::CurveData& curve )
	{
		if ( const auto status = Utils::Registry::write_binary( HKEY_CURRENT_USER, mouse_key, value, curve ); !status )
			return status;

		return reload_ballistics( );
	}
}

namespace Modules
{
	MouseModule::MouseModule( )
	{
		refresh_all( );
	}

	Core::Result<bool> MouseModule::load_pointer_precision( )
	{
		int current[ 3 ] {};

		if ( !SystemParametersInfoW( SPI_GETMOUSE, 0, current, 0 ) )
			return std::unexpected( Core::Error::Unknown );

		return current[ 2 ] != 0;
	}

	Core::Status MouseModule::apply_pointer_precision( const bool& enabled )
	{
		int values[ 3 ] {};
		std::memcpy( values, enabled ? precision_on : precision_off, sizeof( values ) );

		if ( !SystemParametersInfoW( SPI_SETMOUSE, 0, values, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE ) )
			return std::unexpected( Core::Error::Unknown );

		return {};
	}

	Core::Result<CurveData> MouseModule::load_smooth_x_curve( )
	{
		return load_curve( L"SmoothMouseXCurve" );
	}

	Core::Status MouseModule::apply_smooth_x_curve( const CurveData& curve )
	{
		return apply_curve( L"SmoothMouseXCurve", curve );
	}

	Core::Result<CurveData> MouseModule::load_smooth_y_curve( )
	{
		return load_curve( L"SmoothMouseYCurve" );
	}

	Core::Status MouseModule::apply_smooth_y_curve( const CurveData& curve )
	{
		return apply_curve( L"SmoothMouseYCurve", curve );
	}

	Core::Result<std::uint32_t> MouseModule::load_data_queue_size( )
	{
		return Utils::Registry::read_dword( HKEY_LOCAL_MACHINE, queue_size_key, L"MouseDataQueueSize" );
	}

	Core::Status MouseModule::apply_data_queue_size( const std::uint32_t& value )
	{
		if ( value == 0 || value > 100 )
			return std::unexpected( Core::Error::InvalidValue );

		return Utils::Registry::write_dword( HKEY_LOCAL_MACHINE, queue_size_key, L"MouseDataQueueSize", value );
	}
}

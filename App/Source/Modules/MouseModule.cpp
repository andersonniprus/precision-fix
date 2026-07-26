#include "Stdafx.hpp"
#include "Modules/MouseModule.hpp"
#include "Utils/Registry.hpp"

namespace Modules
{
	MouseModule::MouseModule( )
	{
		refresh_all( );
	}

	Core::Result<bool> MouseModule::load_pointer_precision( ) const
	{
		return std::unexpected( Core::Error::NotImplemented );
	}

	Core::Status MouseModule::apply_pointer_precision( const bool& )
	{
		return std::unexpected( Core::Error::NotImplemented );
	}

	Core::Result<CurveData> MouseModule::load_smooth_x_curve( ) const
	{
		return std::unexpected( Core::Error::NotImplemented );
	}

	Core::Status MouseModule::apply_smooth_x_curve( const CurveData& )
	{
		return std::unexpected( Core::Error::NotImplemented );
	}

	Core::Result<CurveData> MouseModule::load_smooth_y_curve( ) const
	{
		return std::unexpected( Core::Error::NotImplemented );
	}

	Core::Status MouseModule::apply_smooth_y_curve( const CurveData& )
	{
		return std::unexpected( Core::Error::NotImplemented );
	}

	Core::Result<std::uint32_t> MouseModule::load_data_queue_size( ) const
	{
		return std::unexpected( Core::Error::NotImplemented );
	}

	Core::Status MouseModule::apply_data_queue_size( const std::uint32_t& )
	{
		return std::unexpected( Core::Error::NotImplemented );
	}
}

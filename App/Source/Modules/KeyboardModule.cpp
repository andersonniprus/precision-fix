#include "Stdafx.hpp"
#include "Modules/KeyboardModule.hpp"
#include "Utils/Power.hpp"
#include "Utils/Registry.hpp"

namespace Modules
{
	KeyboardModule::KeyboardModule( )
	{
		refresh_all( );
	}

	Core::Result<bool> KeyboardModule::load_filter_keys( ) const
	{
		return std::unexpected( Core::Error::NotImplemented );
	}

	Core::Status KeyboardModule::apply_filter_keys( const bool& )
	{
		return std::unexpected( Core::Error::NotImplemented );
	}

	Core::Result<bool> KeyboardModule::load_sticky_keys( ) const
	{
		return std::unexpected( Core::Error::NotImplemented );
	}

	Core::Status KeyboardModule::apply_sticky_keys( const bool& )
	{
		return std::unexpected( Core::Error::NotImplemented );
	}

	Core::Result<bool> KeyboardModule::load_toggle_keys( ) const
	{
		return std::unexpected( Core::Error::NotImplemented );
	}

	Core::Status KeyboardModule::apply_toggle_keys( const bool& )
	{
		return std::unexpected( Core::Error::NotImplemented );
	}

	Core::Result<std::uint32_t> KeyboardModule::load_repeat_delay( ) const
	{
		return std::unexpected( Core::Error::NotImplemented );
	}

	Core::Status KeyboardModule::apply_repeat_delay( const std::uint32_t& )
	{
		return std::unexpected( Core::Error::NotImplemented );
	}

	Core::Result<std::uint32_t> KeyboardModule::load_repeat_rate( ) const
	{
		return std::unexpected( Core::Error::NotImplemented );
	}

	Core::Status KeyboardModule::apply_repeat_rate( const std::uint32_t& )
	{
		return std::unexpected( Core::Error::NotImplemented );
	}

	Core::Result<bool> KeyboardModule::load_usb_selective_suspend( ) const
	{
		return std::unexpected( Core::Error::NotImplemented );
	}

	Core::Status KeyboardModule::apply_usb_selective_suspend( const bool& )
	{
		return std::unexpected( Core::Error::NotImplemented );
	}
}

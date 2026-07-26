#pragma once

#include "Utils/Registry.hpp"

namespace Utils::Power
{
	inline constexpr GUID usb_settings_subgroup = {
		0x2a737441, 0x1930, 0x4402, { 0x8d, 0x77, 0xb2, 0xbe, 0xbb, 0xa3, 0x08, 0xa3 }
	};

	inline constexpr GUID usb_selective_suspend = {
		0x48e6b7a6, 0x50f5, 0x4782, { 0xa5, 0xd4, 0x53, 0xbb, 0x8f, 0x07, 0xe2, 0x26 }
	};
	inline constexpr GUID processor_settings_subgroup = {
		0x54533251, 0x82be, 0x4824, { 0x96, 0xc1, 0x47, 0xb6, 0x0b, 0x74, 0x0d, 0x00 }
	};

	inline constexpr GUID processor_idle_disable = {
		0x5d76a2ca, 0xe8c0, 0x402f, { 0xa1, 0x33, 0x21, 0x58, 0x49, 0x2d, 0x58, 0xad }
	};

	namespace Detail
	{
		using Scheme = std::unique_ptr<GUID, decltype ( &LocalFree )>;

		[[nodiscard]] inline Core::Result<Scheme> active_scheme( )
		{
			GUID* scheme {};

			if ( const auto status = static_cast<LSTATUS>( PowerGetActiveScheme( nullptr, &scheme ) ); status != ERROR_SUCCESS )
				return std::unexpected( Registry::map_status( status ) );

			return Scheme { scheme, &LocalFree };
		}
	}

	[[nodiscard]] inline Core::Result<std::uint32_t> read_ac_value( const GUID& subgroup, const GUID& setting )
	{
		const auto scheme = Detail::active_scheme( );

		if ( !scheme )
			return std::unexpected( scheme.error( ) );

		DWORD index {};

		const auto status = static_cast<LSTATUS>( PowerReadACValueIndex( nullptr, scheme->get( ), &subgroup, &setting, &index ) );
		if ( status != ERROR_SUCCESS )
			return std::unexpected( Registry::map_status( status ) );

		return index;
	}

	inline Core::Status write_ac_value( const GUID& subgroup, const GUID& setting, const std::uint32_t index )
	{
		const auto scheme = Detail::active_scheme( );

		if ( !scheme )
			return std::unexpected( scheme.error( ) );

		if ( const auto status = static_cast<LSTATUS>( PowerWriteACValueIndex( nullptr, scheme->get( ), &subgroup, &setting, index ) );
			status != ERROR_SUCCESS )
			return std::unexpected( Registry::map_status( status ) );

		if ( const auto status = static_cast<LSTATUS>( PowerSetActiveScheme( nullptr, scheme->get( ) ) ); status != ERROR_SUCCESS )
			return std::unexpected( Registry::map_status( status ) );

		return {};
	}
}

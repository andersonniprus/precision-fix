#pragma once

namespace Utils::Registry
{
	class Key
	{
	public:
		Key( ) = default;

		explicit Key( const HKEY handle ) noexcept
			: handle_( handle )
		{
		}

		~Key( )
		{
			if ( handle_ )
				RegCloseKey( handle_ );
		}

		Key( const Key& )            = delete;
		Key& operator=( const Key& ) = delete;

		Key( Key&& other ) noexcept
			: handle_( std::exchange( other.handle_, nullptr ) )
		{
		}

		Key& operator=( Key&& other ) noexcept
		{
			if ( this != &other )
			{
				if ( handle_ )
					RegCloseKey( handle_ );

				handle_ = std::exchange( other.handle_, nullptr );
			}

			return *this;
		}

		[[nodiscard]] HKEY get( ) const noexcept
		{
			return handle_;
		}

	private:
		HKEY handle_ {};
	};

	[[nodiscard]] inline Core::Error map_status( const LSTATUS status ) noexcept
	{
		switch ( status )
		{
			case ERROR_ACCESS_DENIED:
			case ERROR_PRIVILEGE_NOT_HELD:
				return Core::Error::AccessDenied;

			case ERROR_FILE_NOT_FOUND:
			case ERROR_PATH_NOT_FOUND:
				return Core::Error::NotFound;

			case ERROR_INVALID_DATA:
			case ERROR_INVALID_PARAMETER:
				return Core::Error::InvalidValue;

			default:
				return Core::Error::Unknown;
		}
	}

	[[nodiscard]] inline Core::Result<Key> open( const HKEY root, const wchar_t* sub, const REGSAM access )
	{
		HKEY handle {};

		if ( const LSTATUS status = RegOpenKeyExW( root, sub, 0, access, &handle );
			status != ERROR_SUCCESS )
			return std::unexpected( map_status( status ) );

		return Key { handle };
	}

	[[nodiscard]] inline Core::Result<std::wstring> read_sz( const HKEY root, const wchar_t* sub, const wchar_t* value )
	{
		DWORD size {};

		if ( const LSTATUS status = RegGetValueW( root, sub, value, RRF_RT_REG_SZ, nullptr, nullptr, &size ); status != ERROR_SUCCESS )
			return std::unexpected( map_status( status ) );

		std::wstring data( size / sizeof( wchar_t ), L'\0' );

		if ( const LSTATUS status = RegGetValueW( root, sub, value, RRF_RT_REG_SZ, nullptr, data.data( ), &size ); status != ERROR_SUCCESS )
			return std::unexpected( map_status( status ) );

		data.resize( size / sizeof( wchar_t ) );

		while ( !data.empty( ) && data.back( ) == L'\0' )
		{
			data.pop_back( );
		}

		return data;
	}

	[[nodiscard]] inline Core::Result<std::uint32_t> read_dword( const HKEY root, const wchar_t* sub, const wchar_t* value )
	{
		DWORD data {};
		DWORD size = sizeof ( data );

		if ( const LSTATUS status = RegGetValueW( root, sub, value, RRF_RT_REG_DWORD, nullptr, &data, &size ); status != ERROR_SUCCESS )
			return std::unexpected( map_status( status ) );

		return data;
	}

	[[nodiscard]] inline Core::Result<std::vector<std::uint8_t>> read_binary( const HKEY root, const wchar_t* sub, const wchar_t* value )
	{
		DWORD size {};

		if ( const LSTATUS status = RegGetValueW( root, sub, value, RRF_RT_REG_BINARY, nullptr, nullptr, &size ); status != ERROR_SUCCESS )
			return std::unexpected( map_status( status ) );

		std::vector<std::uint8_t> data( size );

		if ( const LSTATUS status = RegGetValueW( root, sub, value, RRF_RT_REG_BINARY, nullptr, data.data( ), &size ); status != ERROR_SUCCESS )
			return std::unexpected( map_status( status ) );

		data.resize( size );

		return data;
	}

	inline Core::Status write_sz( const HKEY root, const wchar_t* sub, const wchar_t* value, const wchar_t* data )
	{
		const auto size = static_cast<DWORD>( ( std::wcslen( data ) + 1 ) * sizeof( wchar_t ) );

		if ( const LSTATUS status = RegSetKeyValueW( root, sub, value, REG_SZ, data, size ); status != ERROR_SUCCESS )
			return std::unexpected( map_status( status ) );

		return {};
	}

	inline Core::Status write_dword( const HKEY root, const wchar_t* sub, const wchar_t* value, const std::uint32_t data )
	{
		const DWORD raw = data;

		if ( const LSTATUS status = RegSetKeyValueW( root, sub, value, REG_DWORD, &raw, sizeof ( raw ) ); status != ERROR_SUCCESS )
			return std::unexpected( map_status( status ) );

		return {};
	}

	inline Core::Status write_binary( const HKEY root, const wchar_t* sub, const wchar_t* value, const std::span<const std::uint8_t> data )
	{
		if ( const LSTATUS status = RegSetKeyValueW(
			root, sub, value, REG_BINARY, data.data( ), static_cast<DWORD>( data.size_bytes( ) )
		); status != ERROR_SUCCESS )
			return std::unexpected( map_status( status ) );

		return {};
	}
}

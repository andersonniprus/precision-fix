#pragma once

namespace Utils::Service
{
	[[nodiscard]] inline Core::Error map_status( const DWORD status ) noexcept
	{
		switch ( status )
		{
			case ERROR_ACCESS_DENIED:
				return Core::Error::AccessDenied;

			case ERROR_SERVICE_DOES_NOT_EXIST:
				return Core::Error::NotFound;

			default:
				return Core::Error::Unknown;
		}
	}

	class Manager
	{
	public:
		Manager( ) noexcept
			: handle_( OpenSCManagerW( nullptr, nullptr, SC_MANAGER_CONNECT ) )
		{
		}

		~Manager( )
		{
			if ( handle_ )
				CloseServiceHandle( handle_ );
		}

		Manager( const Manager& )            = delete;
		Manager& operator=( const Manager& ) = delete;

		[[nodiscard]] SC_HANDLE get( ) const noexcept
		{
			return handle_;
		}

		[[nodiscard]] bool ok( ) const noexcept
		{
			return handle_ != nullptr;
		}

	private:
		SC_HANDLE handle_;
	};

	class Handle
	{
	public:
		Handle( ) = default;

		explicit Handle( const SC_HANDLE handle ) noexcept
			: handle_( handle )
		{
		}

		~Handle( )
		{
			if ( handle_ )
				CloseServiceHandle( handle_ );
		}

		Handle( const Handle& )            = delete;
		Handle& operator=( const Handle& ) = delete;

		Handle( Handle&& other ) noexcept
			: handle_( std::exchange( other.handle_, nullptr ) )
		{
		}

		Handle& operator=( Handle&& other ) noexcept
		{
			if ( this != &other )
			{
				if ( handle_ )
					CloseServiceHandle( handle_ );

				handle_ = std::exchange( other.handle_, nullptr );
			}

			return *this;
		}

		[[nodiscard]] SC_HANDLE get( ) const noexcept
		{
			return handle_;
		}

		[[nodiscard]] bool ok( ) const noexcept
		{
			return handle_ != nullptr;
		}

	private:
		SC_HANDLE handle_ {};
	};

	[[nodiscard]] inline Core::Result<Handle> open( const wchar_t* name, const DWORD access )
	{
		const Manager manager;

		if ( !manager.ok( ) )
			return std::unexpected( map_status( GetLastError( ) ) );

		const SC_HANDLE handle = OpenServiceW( manager.get( ), name, access );

		if ( !handle )
			return std::unexpected( map_status( GetLastError( ) ) );

		return Handle { handle };
	}

	[[nodiscard]] inline Core::Result<bool> is_enabled( const wchar_t* name )
	{
		const auto service = open( name, SERVICE_QUERY_CONFIG );

		if ( !service )
			return std::unexpected( service.error( ) );

		DWORD needed {};
		QueryServiceConfigW( service->get( ), nullptr, 0, &needed );

		if ( GetLastError( ) != ERROR_INSUFFICIENT_BUFFER )
			return std::unexpected( map_status( GetLastError( ) ) );

		std::vector<std::uint8_t> buffer( needed );
		const auto config = reinterpret_cast<QUERY_SERVICE_CONFIGW*>( buffer.data( ) );

		if ( !QueryServiceConfigW( service->get( ), config, needed, &needed ) )
			return std::unexpected( map_status( GetLastError( ) ) );

		return config->dwStartType != SERVICE_DISABLED;
	}

	[[nodiscard]] inline Core::Status set_enabled( const wchar_t* name, const bool enabled )
	{
		const auto service = open( name, SERVICE_CHANGE_CONFIG | SERVICE_STOP | SERVICE_QUERY_STATUS );

		if ( !service )
			return std::unexpected( service.error( ) );

		if ( !ChangeServiceConfigW(
			service->get( ), SERVICE_NO_CHANGE, enabled ? SERVICE_AUTO_START : SERVICE_DISABLED, SERVICE_NO_CHANGE, nullptr, nullptr,
			nullptr, nullptr, nullptr, nullptr, nullptr
		) )
			return std::unexpected( map_status( GetLastError( ) ) );

		if ( !enabled )
		{
			SERVICE_STATUS status {};
			ControlService( service->get( ), SERVICE_CONTROL_STOP, &status );
		}

		return {};
	}
}

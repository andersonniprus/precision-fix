#pragma once

#include "Error.hpp"
#include "Utils/Com.hpp"

#include <taskschd.h>
#include <wrl/client.h>

namespace Utils::TaskScheduler
{
	namespace Detail
	{
		class BstrGuard
		{
		public:
			explicit BstrGuard( const wchar_t* value ) noexcept
				: value_( SysAllocString( value ) )
			{
			}

			~BstrGuard( )
			{
				if ( value_ )
					SysFreeString( value_ );
			}

			BstrGuard( const BstrGuard& )            = delete;
			BstrGuard& operator=( const BstrGuard& ) = delete;

			[[nodiscard]] BSTR get( ) const noexcept
			{
				return value_;
			}

		private:
			BSTR value_;
		};

		[[nodiscard]] inline Core::Error map_hresult( const HRESULT hr ) noexcept
		{
			if ( hr == E_ACCESSDENIED )
				return Core::Error::AccessDenied;

			if ( hr == HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND ) || hr == HRESULT_FROM_WIN32( ERROR_PATH_NOT_FOUND ) )
				return Core::Error::NotFound;

			return Core::Error::Unknown;
		}

		[[nodiscard]] inline Core::Result<Microsoft::WRL::ComPtr<IRegisteredTask>> get_task( const wchar_t* task_path )
		{
			using Microsoft::WRL::ComPtr;

			ComPtr<ITaskService> service;

			if ( const HRESULT hr = CoCreateInstance( __uuidof( ::TaskScheduler), nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS( &service ) );
				FAILED( hr ) )
				return std::unexpected( map_hresult( hr ) );

			VARIANT empty;
			VariantInit( &empty );

			if ( const HRESULT hr = service->Connect( empty, empty, empty, empty ); FAILED( hr ) )
				return std::unexpected( map_hresult( hr ) );

			const std::wstring_view path { task_path };
			const auto separator = path.find_last_of( L'\\' );

			std::wstring folder_path;
			std::wstring task_name;

			if ( separator == std::wstring_view::npos || separator == 0 )
			{
				folder_path = L"\\";
				task_name   = separator == std::wstring_view::npos ? std::wstring { path } : std::wstring { path.substr( 1 ) };
			}
			else
			{
				folder_path = std::wstring { path.substr( 0, separator ) };
				task_name   = std::wstring { path.substr( separator + 1 ) };
			}

			const BstrGuard folder_bstr { folder_path.c_str( ) };
			const BstrGuard name_bstr { task_name.c_str( ) };

			ComPtr<ITaskFolder> folder;

			if ( const HRESULT hr = service->GetFolder( folder_bstr.get( ), &folder ); FAILED( hr ) )
				return std::unexpected( map_hresult( hr ) );

			ComPtr<IRegisteredTask> task;

			if ( const HRESULT hr = folder->GetTask( name_bstr.get( ), &task ); FAILED( hr ) )
				return std::unexpected( map_hresult( hr ) );

			return task;
		}
	}

	[[nodiscard]] inline Core::Result<bool> is_enabled( const wchar_t* task_path )
	{
		const Com::Guard com;

		if ( !com.ok( ) )
			return std::unexpected( Core::Error::Unknown );

		const auto task = Detail::get_task( task_path );

		if ( !task )
			return std::unexpected( task.error( ) );

		VARIANT_BOOL enabled = VARIANT_FALSE;

		if ( const HRESULT hr = ( *task )->get_Enabled( &enabled ); FAILED( hr ) )
			return std::unexpected( Detail::map_hresult( hr ) );

		return enabled != VARIANT_FALSE;
	}

	inline Core::Status set_enabled( const wchar_t* task_path, const bool enabled )
	{
		const Com::Guard com;

		if ( !com.ok( ) )
			return std::unexpected( Core::Error::Unknown );

		const auto task = Detail::get_task( task_path );

		if ( !task )
			return std::unexpected( task.error( ) );

		if ( const HRESULT hr = ( *task )->put_Enabled( enabled ? VARIANT_TRUE : VARIANT_FALSE ); FAILED( hr ) )
			return std::unexpected( Detail::map_hresult( hr ) );

		return {};
	}
}

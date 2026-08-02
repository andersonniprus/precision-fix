#pragma once

#include "Error.hpp"
#include "Utils/Com.hpp"

#include <winrt/Windows.ApplicationModel.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Management.Deployment.h>

namespace Utils::AppX
{
	namespace Detail
	{
		using winrt::Windows::ApplicationModel::Package;
		using winrt::Windows::Management::Deployment::DeploymentOptions;
		using winrt::Windows::Management::Deployment::PackageManager;
		using winrt::Windows::Management::Deployment::RemovalOptions;

		[[nodiscard]] inline Core::Error map_hresult( const winrt::hresult hr ) noexcept
		{
			if ( hr == winrt::hresult { E_ACCESSDENIED } )
				return Core::Error::AccessDenied;

			return Core::Error::Unknown;
		}

		[[nodiscard]] inline bool contains_ci( const std::wstring_view haystack, const std::wstring_view needle ) noexcept
		{
			const auto upper = []( const wchar_t c ) noexcept
			{
				return static_cast<wchar_t>( std::towupper( c ) );
			};

			std::wstring hs( haystack.size( ), L'\0' );
			std::wstring nd( needle.size( ), L'\0' );

			std::ranges::transform( haystack, hs.begin( ), upper );
			std::ranges::transform( needle, nd.begin( ), upper );

			return hs.find( nd ) != std::wstring::npos;
		}

		[[nodiscard]] inline std::vector<Package> find_installed( const std::wstring_view name_substring )
		{
			std::vector<Package> matches;

			for ( const auto& package : PackageManager {}.FindPackagesForUser( L"" ) )
			{
				if ( contains_ci( std::wstring_view { package.Id( ).Name( ) }, name_substring ) )
					matches.push_back( package );
			}

			return matches;
		}
	}

	[[nodiscard]] inline Core::Result<bool> is_installed( const std::wstring_view name_substring )
	{
		if ( const Com::Guard com; !com.ok( ) )
			return std::unexpected( Core::Error::Unknown );

		try
		{
			return !Detail::find_installed( name_substring ).empty( );
		}
		catch ( const winrt::hresult_error& e )
		{
			return std::unexpected( Detail::map_hresult( e.code( ) ) );
		}
	}

	inline Core::Status remove( const std::wstring_view name_substring )
	{
		if ( const Com::Guard com; !com.ok( ) )
			return std::unexpected( Core::Error::Unknown );

		try
		{
			for ( const auto& package : Detail::find_installed( name_substring ) )
			{
				const Detail::PackageManager manager;

				const auto result = manager.RemovePackageAsync( package.Id( ).FullName( ), Detail::RemovalOptions::RemoveForAllUsers ).get( );

				if ( const auto hr = result.ExtendedErrorCode( ); FAILED( hr ) )
					return std::unexpected( Detail::map_hresult( hr ) );
			}

			return {};
		}
		catch ( const winrt::hresult_error& e )
		{
			return std::unexpected( Detail::map_hresult( e.code( ) ) );
		}
	}

	inline Core::Status restore( const std::wstring_view name_substring )
	{
		if ( const Com::Guard com; !com.ok( ) )
			return std::unexpected( Core::Error::Unknown );

		try
		{
			bool matched = false;

			for ( const Detail::PackageManager manager; const auto& package : manager.FindProvisionedPackages( ) )
			{
				if ( !Detail::contains_ci( std::wstring_view { package.Id( ).Name( ) }, name_substring ) )
					continue;

				matched = true;

				const auto result = manager
				                    .RegisterPackageByFamilyNameAsync(
					                    package.Id( ).FamilyName( ), nullptr, Detail::DeploymentOptions::None, nullptr, nullptr
				                    )
				                    .get( );

				if ( const auto hr = result.ExtendedErrorCode( ); FAILED( hr ) )
					return std::unexpected( Detail::map_hresult( hr ) );
			}

			if ( !matched )
				return std::unexpected( Core::Error::NotFound );

			return {};
		}
		catch ( const winrt::hresult_error& e )
		{
			return std::unexpected( Detail::map_hresult( e.code( ) ) );
		}
	}
}

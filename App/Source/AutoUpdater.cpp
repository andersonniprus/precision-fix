#include "Stdafx.hpp"
#include "AutoUpdater.hpp"
#include "Version.hpp"

#include <winhttp.h>

#pragma comment( lib, "winhttp.lib" )

namespace
{
	using HInternet = std::unique_ptr<std::remove_pointer_t<HINTERNET>, decltype( []( HINTERNET handle )
	{
		if ( handle )
			WinHttpCloseHandle( handle );
	} )>;

	[[nodiscard]] std::optional<std::string> extract_json_string( const std::string_view json, const std::string_view key )
	{
		const std::string needle = std::format( "\"{}\"", key );
		const auto key_pos       = json.find( needle );

		if ( key_pos == std::string_view::npos )
			return std::nullopt;

		const auto colon = json.find( ':', key_pos + needle.size( ) );

		if ( colon == std::string_view::npos )
			return std::nullopt;

		const auto first_quote = json.find( '"', colon + 1 );

		if ( first_quote == std::string_view::npos )
			return std::nullopt;

		const auto second_quote = json.find( '"', first_quote + 1 );

		if ( second_quote == std::string_view::npos )
			return std::nullopt;

		return std::string { json.substr( first_quote + 1, second_quote - first_quote - 1 ) };
	}

	[[nodiscard]] std::optional<std::string> http_get( const std::wstring& host, const std::wstring& path )
	{
		HInternet session {
			WinHttpOpen(
				L"precision-fix",
				WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
				nullptr,
				nullptr,
				0
			)
		};

		if ( !session )
			return std::nullopt;

		DWORD timeout = 4000;
		WinHttpSetTimeouts( session.get( ), timeout, timeout, timeout, timeout );

		HInternet connect { WinHttpConnect( session.get( ), host.c_str( ), INTERNET_DEFAULT_HTTPS_PORT, 0 ) };

		if ( !connect )
			return std::nullopt;

		HInternet request {
			WinHttpOpenRequest(
				connect.get( ),
				L"GET",
				path.c_str( ),
				nullptr,
				nullptr,
				nullptr,
				WINHTTP_FLAG_SECURE
			)
		};

		if ( !request )
			return std::nullopt;

		constexpr wchar_t headers[ ] = L"Accept: application/vnd.github+json\r\n";

		if ( !WinHttpSendRequest(
			request.get( ),
			headers,
			static_cast<DWORD>( -1 ),
			nullptr,
			0,
			0,
			0
		) )
			return std::nullopt;

		if ( !WinHttpReceiveResponse( request.get( ), nullptr ) )
			return std::nullopt;

		std::string body;
		DWORD available = 0;

		while ( WinHttpQueryDataAvailable( request.get( ), &available ) && available > 0 )
		{
			std::string chunk( available, '\0' );
			DWORD read = 0;

			if ( !WinHttpReadData( request.get( ), chunk.data( ), available, &read ) )
				return std::nullopt;

			chunk.resize( read );
			body.append( chunk );
		}

		return body;
	}
}

namespace App
{
	AutoUpdater::AutoUpdater( std::shared_ptr<Logger> logger, std::shared_ptr<Intl> intl )
		: logger_( std::move( logger ) ),
		  intl_( std::move( intl ) )
	{
	}

	std::optional<UpdateInfo> AutoUpdater::fetch_latest( ) const
	{
		const auto body = http_get( L"api.github.com", L"/repos/andersonniprus/precision-fix/releases/latest" );

		if ( !body )
		{
			if ( logger_ )
				logger_->warn( "Updater", "failed to query GitHub releases" );

			return std::nullopt;
		}

		const auto tag = extract_json_string( *body, "tag_name" );
		const auto url = extract_json_string( *body, "html_url" );

		if ( !tag )
		{
			if ( logger_ )
				logger_->warn( "Updater", "release payload missing tag_name" );

			return std::nullopt;
		}

		UpdateInfo info {
			.tag = *tag,
			.html_url = url.value_or( std::string { kReleasesUrl } ),
		};

		if ( logger_ )
			logger_->info( "Updater", std::format( "latest release {}", info.tag ) );

		return info;
	}

	bool AutoUpdater::prompt_if_newer( ) const
	{
		const auto latest = fetch_latest( );

		if ( !latest )
			return true;

		if ( compare_semver( latest->tag, kVersion ) <= 0 )
			return true;

		const std::wstring title = intl_ ? intl_->tr_w( "Update available" ) : L"Update available";
		const std::wstring body  = intl_
		                           ? std::format(
			                           L"{}\n\n{} → {}\n\n{} | {} | {}",
			                           intl_->tr_w( "A new version is available on GitHub." ),
			                           std::wstring( kVersion.begin( ), kVersion.end( ) ),
			                           std::wstring( latest->tag.begin( ), latest->tag.end( ) ),
			                           intl_->tr_w( "Yes = continue" ),
			                           intl_->tr_w( "No = open GitHub and exit" ),
			                           intl_->tr_w( "Cancel = exit" )
		                           )
		                           : L"A new version is available.";

		const int choice = MessageBoxW(
			nullptr,
			body.c_str( ),
			title.c_str( ),
			MB_YESNOCANCEL | MB_ICONINFORMATION | MB_TOPMOST
		);

		if ( choice == IDNO )
		{
			const std::wstring url( latest->html_url.begin( ), latest->html_url.end( ) );
			ShellExecuteW( nullptr, L"open", url.c_str( ), nullptr, nullptr, SW_SHOWNORMAL );

			if ( logger_ )
				logger_->info( "Updater", "opened GitHub releases; exiting" );

			return false;
		}

		if ( choice == IDCANCEL )
		{
			if ( logger_ )
				logger_->info( "Updater", "update prompt cancelled" );

			return false;
		}

		if ( logger_ )
			logger_->info( "Updater", "continuing with current version" );

		return true;
	}
}

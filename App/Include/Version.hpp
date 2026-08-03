#pragma once

namespace App
{
	inline constexpr std::string_view kName        = "precision-fix";
	inline constexpr std::string_view kVersion     = "0.3.0";
	inline constexpr std::string_view kAuthor      = "andersonniprus";
	inline constexpr std::string_view kGitHubUrl   = "https://github.com/andersonniprus/precision-fix";
	inline constexpr std::string_view kReleasesUrl = "https://github.com/andersonniprus/precision-fix/releases";

	[[nodiscard]] constexpr std::array<int, 3> parse_semver( std::string_view text ) noexcept
	{
		std::array<int, 3> parts {};

		if ( !text.empty( ) && ( text.front( ) == 'v' || text.front( ) == 'V' ) )
			text.remove_prefix( 1 );

		std::size_t index = 0;

		for ( std::size_t i = 0; i < text.size( ) && index < parts.size( ); ++i )
		{
			if ( text[ i ] == '.' )
			{
				++index;
				continue;
			}

			if ( text[ i ] < '0' || text[ i ] > '9' )
				break;

			parts[ index ] = parts[ index ] * 10 + ( text[ i ] - '0' );
		}

		return parts;
	}

	[[nodiscard]] constexpr int compare_semver( const std::string_view lhs, const std::string_view rhs ) noexcept
	{
		const auto a = parse_semver( lhs );
		const auto b = parse_semver( rhs );

		if ( a[ 0 ] != b[ 0 ] )
			return a[ 0 ] < b[ 0 ] ? -1 : 1;

		if ( a[ 1 ] != b[ 1 ] )
			return a[ 1 ] < b[ 1 ] ? -1 : 1;

		if ( a[ 2 ] != b[ 2 ] )
			return a[ 2 ] < b[ 2 ] ? -1 : 1;

		return 0;
	}
}

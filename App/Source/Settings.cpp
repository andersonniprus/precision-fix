#include "Stdafx.hpp"
#include "Settings.hpp"
#include "Utils/Registry.hpp"

namespace
{
	constexpr wchar_t kKey[ ]      = LR"(Software\precision-fix)";
	constexpr wchar_t kTheme[ ]    = L"Theme";
	constexpr wchar_t kLanguage[ ] = L"Language";

	[[nodiscard]] const wchar_t* language_to_sz( const App::Language language ) noexcept
	{
		switch ( language )
		{
			case App::Language::Es:
				return L"es";

			case App::Language::PtBr:
				return L"pt-BR";

			default:
				return L"en";
		}
	}

	[[nodiscard]] App::Language language_from_sz( const std::wstring& value ) noexcept
	{
		if ( value == L"es" )
			return App::Language::Es;

		if ( value == L"pt-BR" || value == L"pt_BR" || value == L"pt" )
			return App::Language::PtBr;

		return App::Language::En;
	}
}

namespace App
{
	Core::Status Settings::load( )
	{
		if ( const auto theme = Utils::Registry::read_dword( HKEY_CURRENT_USER, kKey, kTheme ); theme )
			theme_ = ( *theme == 0 ) ? ThemeMode::Dark : ThemeMode::Light;

		if ( const auto language = Utils::Registry::read_sz( HKEY_CURRENT_USER, kKey, kLanguage ); language )
			language_ = language_from_sz( *language );

		return {};
	}

	Core::Status Settings::save( ) const
	{
		if ( const auto status = Utils::Registry::write_dword(
				HKEY_CURRENT_USER, kKey, kTheme, theme_ == ThemeMode::Dark ? 0u : 1u );
			!status )
			return status;

		return Utils::Registry::write_sz( HKEY_CURRENT_USER, kKey, kLanguage, language_to_sz( language_ ) );
	}
}

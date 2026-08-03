#pragma once

namespace App
{
	enum class ThemeMode : std::uint8_t
	{
		Dark,
		Light,
	};

	enum class Language : std::uint8_t
	{
		En,
		Es,
		PtBr,
	};

	class Settings
	{
	public:
		[[nodiscard]] Core::Status load( );
		[[nodiscard]] Core::Status save( ) const;

		[[nodiscard]] ThemeMode theme( ) const noexcept
		{
			return theme_;
		}

		void set_theme( const ThemeMode theme ) noexcept
		{
			theme_ = theme;
		}

		[[nodiscard]] Language language( ) const noexcept
		{
			return language_;
		}

		void set_language( const Language language ) noexcept
		{
			language_ = language;
		}

	private:
		ThemeMode theme_ { ThemeMode::Dark };
		Language language_ { Language::En };
	};
}

#pragma once

#include "Settings.hpp"

namespace App
{
	class Intl
	{
	public:
		void set_language( const Language language ) noexcept
		{
			language_ = language;
		}

		[[nodiscard]] Language language( ) const noexcept
		{
			return language_;
		}

		[[nodiscard]] const char* tr( std::string_view ) const noexcept;
		[[nodiscard]] std::wstring tr_w( std::string_view ) const;

	private:
		Language language_ { Language::En };
	};
}

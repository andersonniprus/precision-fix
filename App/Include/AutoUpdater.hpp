#pragma once

#include "Intl.hpp"
#include "Logger.hpp"

namespace App
{
	struct UpdateInfo
	{
		std::string tag;
		std::string html_url;
	};

	class AutoUpdater
	{
	public:
		AutoUpdater( std::shared_ptr<Logger>, std::shared_ptr<Intl> );

		[[nodiscard]] std::optional<UpdateInfo> fetch_latest( ) const;
		[[nodiscard]] bool prompt_if_newer( ) const;

	private:
		std::shared_ptr<Logger> logger_;
		std::shared_ptr<Intl> intl_;
	};
}

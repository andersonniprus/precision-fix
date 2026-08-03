#pragma once
#include "UI/Interfaces/IPage.hpp"
#include "UI/Managers/FontManager.hpp"
#include "Intl.hpp"

namespace UI::Managers
{
	class TabManager
	{
	public:
		void add_tab( std::string key, const char* icon, std::unique_ptr<IPage> );

		void set_font_manager( FontManager& font_manager )
		{
			font_manager_ = &font_manager;
		}

		void set_intl( std::shared_ptr<App::Intl> intl )
		{
			intl_ = std::move( intl );
		}

		void render( );

	private:
		static constexpr std::size_t none        = static_cast<std::size_t>( -1 );
		static constexpr float sidebar_width     = 216.f;
		static constexpr float fade_out_duration = 0.16f;
		static constexpr float fade_in_duration  = 0.20f;

		struct Tab
		{
			std::string key;
			const char* icon;
			std::unique_ptr<IPage> page;
		};

		void render_sidebar( float height );
		void render_content( float height );

		[[nodiscard]] const char* tab_label( const Tab& tab ) const noexcept;

		std::vector<Tab> tabs_;
		std::size_t active_ { none };
		std::size_t previous_ { none };

		FontManager* font_manager_ = nullptr;
		std::shared_ptr<App::Intl> intl_;

		float fade_ { 1.f };
		bool fading_out_ { false };
	};
}

#include "Stdafx.hpp"
#include "UI/Pages/InfoPage.hpp"
#include "UI/Theme.hpp"
#include "UI/Fonts/IconsLucide.h"
#include "UI/Widgets/Section.hpp"
#include "UI/Widgets/Button.hpp"
#include "UI/Widgets/SettingsRow.hpp"
#include "Version.hpp"

namespace UI::Pages
{
	InfoPage::InfoPage(
		std::shared_ptr<App::Settings> settings,
		std::shared_ptr<App::Intl> intl,
		std::shared_ptr<App::Logger> logger
	)
		: settings_( std::move( settings ) ),
		  intl_( std::move( intl ) ),
		  logger_( std::move( logger ) )
	{
	}

	void InfoPage::render( )
	{
		const float control_width  = Widgets::preset_row_width( 2 );
		const float control_height = Widgets::preset_button_height( );

		Widgets::section(
			ICON_LC_PALETTE,
			intl_->tr( "Appearance" ),
			intl_->tr( "Theme and language preferences." ),
			[ & ]
			{
				ImGui::PushID( "theme" );
				Widgets::settings_row(
					intl_->tr( "Theme" ),
					intl_->tr( "Theme and language preferences." ),
					control_width,
					control_height,
					Widgets::Level::Low,
					Widgets::Level::Low,
					"",
					[ & ]
					{
						const bool dark = settings_->theme( ) == App::ThemeMode::Dark;

						if ( Widgets::preset_button( intl_->tr( "Dark" ), dark ? Widgets::ButtonVariant::Primary : Widgets::ButtonVariant::Ghost ) )
						{
							settings_->set_theme( App::ThemeMode::Dark );
							( void ) settings_->save( );
							logger_->info( "Settings", "theme → Dark" );
						}

						Widgets::preset_same_line( );

						if ( Widgets::preset_button( intl_->tr( "Light" ), !dark ? Widgets::ButtonVariant::Primary : Widgets::ButtonVariant::Ghost ) )
						{
							settings_->set_theme( App::ThemeMode::Light );
							( void ) settings_->save( );
							logger_->info( "Settings", "theme → Light" );
						}
					}
				);
				ImGui::PopID( );

				ImGui::PushID( "language" );
				const float lang_width = Widgets::preset_row_width( 3 );

				Widgets::settings_row(
					intl_->tr( "Language" ),
					intl_->tr( "Theme and language preferences." ),
					lang_width,
					control_height,
					Widgets::Level::Low,
					Widgets::Level::Low,
					"",
					[ & ]
					{
						const auto language = settings_->language( );

						if ( Widgets::preset_button(
							intl_->tr( "English" ),
							language == App::Language::En ? Widgets::ButtonVariant::Primary : Widgets::ButtonVariant::Ghost
						) )
						{
							settings_->set_language( App::Language::En );
							intl_->set_language( App::Language::En );
							( void ) settings_->save( );
							logger_->info( "Settings", "language → en" );
						}

						Widgets::preset_same_line( );

						if ( Widgets::preset_button(
							intl_->tr( "Spanish" ),
							language == App::Language::Es ? Widgets::ButtonVariant::Primary : Widgets::ButtonVariant::Ghost
						) )
						{
							settings_->set_language( App::Language::Es );
							intl_->set_language( App::Language::Es );
							( void ) settings_->save( );
							logger_->info( "Settings", "language → es" );
						}

						Widgets::preset_same_line( );

						if ( Widgets::preset_button(
							intl_->tr( "Portuguese (Brazil)" ),
							language == App::Language::PtBr ? Widgets::ButtonVariant::Primary : Widgets::ButtonVariant::Ghost
						) )
						{
							settings_->set_language( App::Language::PtBr );
							intl_->set_language( App::Language::PtBr );
							( void ) settings_->save( );
							logger_->info( "Settings", "language → pt-BR" );
						}
					}
				);
				ImGui::PopID( );
			}
		);

		Widgets::section(
			ICON_LC_INFO,
			intl_->tr( "About" ),
			intl_->tr( "Application information and licensing." ),
			[ & ]
			{
				ImGui::PushStyleColor( ImGuiCol_Text, Theme::MutedText );
				ImGui::Text( "%s: %.*s", intl_->tr( "Author" ), static_cast<int>( App::kAuthor.size( ) ), App::kAuthor.data( ) );
				ImGui::TextUnformatted( intl_->tr( "Open source — not authorized for sale" ) );
				ImGui::Text( "%s: %.*s", intl_->tr( "Version" ), static_cast<int>( App::kVersion.size( ) ), App::kVersion.data( ) );
				ImGui::PopStyleColor( );

				ImGui::Dummy( ImVec2( 0.f, Theme::px( 8.f ) ) );

				if ( Widgets::Button( intl_->tr( "Open GitHub" ), Widgets::ButtonVariant::Primary ) )
				{
					constexpr std::wstring url( App::kGitHubUrl.begin( ), App::kGitHubUrl.end( ) );
					ShellExecuteW( nullptr, L"open", url.c_str( ), nullptr, nullptr, SW_SHOWNORMAL );
				}
			}
		);
	}
}

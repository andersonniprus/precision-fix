#include "Stdafx.hpp"
#include "UI/Pages/LogsPage.hpp"
#include "UI/Theme.hpp"
#include "UI/Fonts/IconsLucide.h"
#include "UI/Widgets/Section.hpp"

namespace UI::Pages
{
	LogsPage::LogsPage( std::shared_ptr<App::Logger> logger, std::shared_ptr<App::Intl> intl )
		: logger_( std::move( logger ) ),
		  intl_( std::move( intl ) )
	{
	}

	void LogsPage::render( )
	{
		Widgets::section(
			ICON_LC_CLIPBOARD_LIST,
			intl_->tr( "Logs" ),
			intl_->tr( "Runtime events from features and the application." ),
			[ & ]
			{
				const auto entries = logger_->entries( );

				if ( entries.empty( ) )
				{
					ImGui::PushStyleColor( ImGuiCol_Text, Theme::MutedText );
					ImGui::TextUnformatted( intl_->tr( "No logs yet." ) );
					ImGui::PopStyleColor( );
					return;
				}

				ImGui::BeginChild( "##log_list", ImVec2( 0.f, 0.f ), ImGuiChildFlags_None );

				for ( const auto& entry : entries )
				{
					const ImVec4 color = [ & ]
					{
						switch ( entry.level )
						{
							case App::LogLevel::Warning:
								return Theme::Warning;

							case App::LogLevel::Error:
								return Theme::Danger;

							default:
								return Theme::Foreground;
						}
					}( );

					const auto seconds     = std::chrono::floor<std::chrono::seconds>( entry.time );
					const std::string line = std::format(
						"{:%H:%M:%S}  [{}]  {}",
						seconds,
						entry.category,
						entry.message
					);

					ImGui::PushStyleColor( ImGuiCol_Text, color );
					ImGui::TextUnformatted( line.c_str( ) );
					ImGui::PopStyleColor( );
				}

				if ( ImGui::GetScrollY( ) >= ImGui::GetScrollMaxY( ) - 4.f )
					ImGui::SetScrollHereY( 1.f );

				ImGui::EndChild( );
			}
		);
	}
}

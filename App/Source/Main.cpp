#include "Stdafx.hpp"
#include "UI/Window.hpp"
#include "Settings.hpp"
#include "Logger.hpp"
#include "Intl.hpp"
#include "AutoUpdater.hpp"
#include "UI/Theme.hpp"

#ifdef NDEBUG
int __stdcall WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
#else
int main( int, char*[ ] )
#endif
{
	auto settings = std::make_shared<App::Settings>( );
	auto logger   = std::make_shared<App::Logger>( );
	auto intl     = std::make_shared<App::Intl>( );

	( void ) settings->load( );
	intl->set_language( settings->language( ) );
	UI::Theme::set_mode( settings->theme( ) == App::ThemeMode::Light ? UI::Theme::Mode::Light : UI::Theme::Mode::Dark );

	logger->info( "App", "starting" );

	{
		const App::AutoUpdater updater( logger, intl );

		if ( !updater.prompt_if_newer( ) )
			return 0;
	}

	try
	{
		UI::Window window( L"precision-fix", ImVec2( 760.f, 560.f ), settings, logger, intl );
		window.run( );
		logger->info( "App", "exit" );
		return 0;
	}
	catch ( const std::exception& e )
	{
		logger->error( "App", e.what( ) );
		MessageBoxA( nullptr, e.what( ), "Error", MB_OK | MB_ICONERROR );
		return 1;
	}
}

#include "Stdafx.hpp"
#include "UI/Window.hpp"

#ifdef NDEBUG
int __stdcall WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
#else
int main( int, char*[ ] )
#endif
{
	try
	{
		UI::Window window( L"precision-fix", ImVec2( 760.f, 560.f ) );

		window.run( );

		return 0;
	}
	catch ( const std::exception& e )
	{
		MessageBoxA( nullptr, e.what( ), "Error", MB_OK | MB_ICONERROR );
		return 1;
	}
}

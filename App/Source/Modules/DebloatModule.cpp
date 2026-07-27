#include "Stdafx.hpp"
#include "Modules/DebloatModule.hpp"
#include "Utils/AppX.hpp"

namespace
{
	constexpr std::array<const wchar_t*, static_cast<std::size_t>( std::to_underlying( Modules::DebloatFeature::Count ) )> package_names = {
		L"Microsoft.BingWeather",
		L"Microsoft.GetHelp",
		L"Microsoft.Getstarted",
		L"Microsoft.HEIFImageExtension",
		L"Microsoft.Messaging",
		L"Microsoft.Microsoft3DViewer",
		L"Microsoft.MicrosoftSolitaireCollection",
		L"Microsoft.MicrosoftStickyNotes",
		L"Microsoft.MixedReality.Portal",
		L"Microsoft.OneConnect",
		L"Microsoft.People",
		L"Microsoft.Print3D",
		L"Microsoft.SkypeApp",
		L"Microsoft.WebMediaExtensions",
		L"Microsoft.WebpImageExtension",
		L"Microsoft.WindowsAlarms",
		L"Microsoft.WindowsCamera",
		L"Microsoft.WindowsFeedbackHub",
		L"Microsoft.WindowsMaps",
		L"Microsoft.WindowsSoundRecorder",
		L"Microsoft.YourPhone",
		L"Microsoft.ZuneMusic",
		L"microsoft.windowscommunicationsapps",
		L"bing",
		L"Drawboard PDF",
		L"Sway",
		L"Microsoft.549981C3F5F10",
		L"Microsoft.Windows.Ai.Copilot.Provider",
	};
}

namespace Modules
{
	DebloatModule::DebloatModule( )
	{
		refresh_all( );
	}

	template<DebloatFeature F>
	Core::Result<bool> DebloatModule::load( )
	{
		return Utils::AppX::is_installed( package_names[ static_cast<std::size_t>( std::to_underlying( F ) ) ] );
	}

	template<DebloatFeature F>
	Core::Status DebloatModule::apply( const bool& enabled )
	{
		const auto* name = package_names[ static_cast<std::size_t>( std::to_underlying( F ) ) ];
		return enabled ? Utils::AppX::restore( name ) : Utils::AppX::remove( name );
	}

	template Core::Result<bool> DebloatModule::load<DebloatFeature::BingWeather>( );
	template Core::Status DebloatModule::apply<DebloatFeature::BingWeather>( const bool& );

	template Core::Result<bool> DebloatModule::load<DebloatFeature::GetHelp>( );
	template Core::Status DebloatModule::apply<DebloatFeature::GetHelp>( const bool& );

	template Core::Result<bool> DebloatModule::load<DebloatFeature::GetStarted>( );
	template Core::Status DebloatModule::apply<DebloatFeature::GetStarted>( const bool& );

	template Core::Result<bool> DebloatModule::load<DebloatFeature::HeifImageExtension>( );
	template Core::Status DebloatModule::apply<DebloatFeature::HeifImageExtension>( const bool& );

	template Core::Result<bool> DebloatModule::load<DebloatFeature::Messaging>( );
	template Core::Status DebloatModule::apply<DebloatFeature::Messaging>( const bool& );

	template Core::Result<bool> DebloatModule::load<DebloatFeature::Microsoft3DViewer>( );
	template Core::Status DebloatModule::apply<DebloatFeature::Microsoft3DViewer>( const bool& );

	template Core::Result<bool> DebloatModule::load<DebloatFeature::SolitaireCollection>( );
	template Core::Status DebloatModule::apply<DebloatFeature::SolitaireCollection>( const bool& );

	template Core::Result<bool> DebloatModule::load<DebloatFeature::StickyNotes>( );
	template Core::Status DebloatModule::apply<DebloatFeature::StickyNotes>( const bool& );

	template Core::Result<bool> DebloatModule::load<DebloatFeature::MixedRealityPortal>( );
	template Core::Status DebloatModule::apply<DebloatFeature::MixedRealityPortal>( const bool& );

	template Core::Result<bool> DebloatModule::load<DebloatFeature::OneConnect>( );
	template Core::Status DebloatModule::apply<DebloatFeature::OneConnect>( const bool& );

	template Core::Result<bool> DebloatModule::load<DebloatFeature::People>( );
	template Core::Status DebloatModule::apply<DebloatFeature::People>( const bool& );

	template Core::Result<bool> DebloatModule::load<DebloatFeature::Print3D>( );
	template Core::Status DebloatModule::apply<DebloatFeature::Print3D>( const bool& );

	template Core::Result<bool> DebloatModule::load<DebloatFeature::SkypeApp>( );
	template Core::Status DebloatModule::apply<DebloatFeature::SkypeApp>( const bool& );

	template Core::Result<bool> DebloatModule::load<DebloatFeature::WebMediaExtensions>( );
	template Core::Status DebloatModule::apply<DebloatFeature::WebMediaExtensions>( const bool& );

	template Core::Result<bool> DebloatModule::load<DebloatFeature::WebpImageExtension>( );
	template Core::Status DebloatModule::apply<DebloatFeature::WebpImageExtension>( const bool& );

	template Core::Result<bool> DebloatModule::load<DebloatFeature::WindowsAlarms>( );
	template Core::Status DebloatModule::apply<DebloatFeature::WindowsAlarms>( const bool& );

	template Core::Result<bool> DebloatModule::load<DebloatFeature::WindowsCamera>( );
	template Core::Status DebloatModule::apply<DebloatFeature::WindowsCamera>( const bool& );

	template Core::Result<bool> DebloatModule::load<DebloatFeature::FeedbackHub>( );
	template Core::Status DebloatModule::apply<DebloatFeature::FeedbackHub>( const bool& );

	template Core::Result<bool> DebloatModule::load<DebloatFeature::WindowsMaps>( );
	template Core::Status DebloatModule::apply<DebloatFeature::WindowsMaps>( const bool& );

	template Core::Result<bool> DebloatModule::load<DebloatFeature::SoundRecorder>( );
	template Core::Status DebloatModule::apply<DebloatFeature::SoundRecorder>( const bool& );

	template Core::Result<bool> DebloatModule::load<DebloatFeature::YourPhone>( );
	template Core::Status DebloatModule::apply<DebloatFeature::YourPhone>( const bool& );

	template Core::Result<bool> DebloatModule::load<DebloatFeature::ZuneMusic>( );
	template Core::Status DebloatModule::apply<DebloatFeature::ZuneMusic>( const bool& );

	template Core::Result<bool> DebloatModule::load<DebloatFeature::MailAndCalendar>( );
	template Core::Status DebloatModule::apply<DebloatFeature::MailAndCalendar>( const bool& );

	template Core::Result<bool> DebloatModule::load<DebloatFeature::BingApps>( );
	template Core::Status DebloatModule::apply<DebloatFeature::BingApps>( const bool& );

	template Core::Result<bool> DebloatModule::load<DebloatFeature::DrawboardPdf>( );
	template Core::Status DebloatModule::apply<DebloatFeature::DrawboardPdf>( const bool& );

	template Core::Result<bool> DebloatModule::load<DebloatFeature::Sway>( );
	template Core::Status DebloatModule::apply<DebloatFeature::Sway>( const bool& );

	template Core::Result<bool> DebloatModule::load<DebloatFeature::Cortana>( );
	template Core::Status DebloatModule::apply<DebloatFeature::Cortana>( const bool& );

	template Core::Result<bool> DebloatModule::load<DebloatFeature::Copilot>( );
	template Core::Status DebloatModule::apply<DebloatFeature::Copilot>( const bool& );
}

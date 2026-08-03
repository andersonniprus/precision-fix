#include "Stdafx.hpp"
#include "UI/Pages/MousePage.hpp"
#include "Logger.hpp"
#include "Intl.hpp"
#include "UI/Theme.hpp"
#include "UI/Fonts/IconsLucide.h"
#include "UI/Widgets/Section.hpp"
#include "UI/Widgets/SettingsRow.hpp"
#include "UI/Widgets/Switch.hpp"

namespace
{
	constexpr Modules::CurveData optimized_curve = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00,
	};

	constexpr std::uint32_t optimized_queue_size = 16;
}

namespace UI::Pages
{
	using namespace Widgets;
	using enum Modules::MouseFeature;
	using Modules::FeatureTraits;

	MousePage::MousePage(
		std::shared_ptr<Modules::MouseModule> module,
		std::shared_ptr<App::Logger> logger,
		std::shared_ptr<App::Intl> intl
	)
		: mouse_( std::move( module ) ),
		  logger_( std::move( logger ) ),
		  intl_( std::move( intl ) )
	{
	}

	void MousePage::on_activate( )
	{
		mouse_->refresh_all( );
	}

	void MousePage::set_status( const Core::Status& status )
	{
		if ( status )
		{
			logger_->info( "Mouse", "setting applied" );
			status_.clear( );
			return;
		}

		logger_->error( "Mouse", std::string { Core::to_string( status.error( ) ) } );
		status_ = std::string { Core::to_string( status.error( ) ) };
	}


	void MousePage::render( )
	{
		const float switch_width  = ImGui::GetFrameHeight( ) * switch_aspect;
		const float switch_height = ImGui::GetFrameHeight( );

		const float control_width  = preset_row_width( 2 );
		const float control_height = preset_button_height( );

		section( ICON_LC_CROSSHAIR, intl_->tr( "Precision" ), intl_->tr( "Pointer acceleration and speed." ), [ & ]
		{
			bool precision = mouse_->get<PointerPrecision>( );

			settings_row(
				intl_->tr( "Enhance pointer precision" ),
				intl_->tr( "Applies Windows' acceleration curve based on movement speed." ),
				switch_width, switch_height,
				Level::Low, Level::Low,
				intl_->tr( "Off" ),
				[ & ]
				{
					if ( toggle_switch( intl_->tr( "Enhance pointer precision" ), &precision ) )
					{
						set_status( mouse_->set<PointerPrecision>( precision ) );
					}
				}
			);

			ImGui::PushID( "Smooth mouse X curve" );
			settings_row(
				intl_->tr( "Smooth mouse X curve" ),
				intl_->tr( "Horizontal acceleration curve. Optimized removes added acceleration." ),
				control_width, control_height,
				Level::Low, Level::Low,
				intl_->tr( "Optimized" ),
				[ & ]
				{
					if ( preset_button( intl_->tr( "Default" ), ButtonVariant::Ghost ) )
					{
						set_status( mouse_->set<SmoothMouseXCurve>( FeatureTraits<SmoothMouseXCurve>::fallback( ) ) );
					}

					preset_same_line( );

					if ( preset_button( intl_->tr( "Optimized" ), ButtonVariant::Primary ) )
					{
						set_status( mouse_->set<SmoothMouseXCurve>( optimized_curve ) );
					}
				}
			);
			ImGui::PopID( );

			ImGui::PushID( "Smooth mouse Y curve" );
			settings_row(
				intl_->tr( "Smooth mouse Y curve" ),
				intl_->tr( "Vertical acceleration curve. Optimized removes added acceleration." ),
				control_width, control_height,
				Level::Low, Level::Low,
				intl_->tr( "Optimized" ),
				[ & ]
				{
					if ( preset_button( intl_->tr( "Default" ), ButtonVariant::Ghost ) )
					{
						set_status( mouse_->set<SmoothMouseYCurve>( FeatureTraits<SmoothMouseYCurve>::fallback( ) ) );
					}

					preset_same_line( );

					if ( preset_button( intl_->tr( "Optimized" ), ButtonVariant::Primary ) )
					{
						set_status( mouse_->set<SmoothMouseYCurve>( optimized_curve ) );
					}
				}
			);
			ImGui::PopID( );

			ImGui::PushID( "Pointer speed" );
			settings_row(
				intl_->tr( "Pointer speed" ),
				intl_->tr( "Windows pointer-speed slider setting, from 1 (slowest) to 20 (fastest)." ),
				control_width, control_height,
				Level::Low, Level::Low,
				"Fastest",
				[ & ]
				{
					if ( preset_button( intl_->tr( "Default" ), ButtonVariant::Ghost ) )
					{
						set_status( mouse_->set<PointerSpeed>( FeatureTraits<PointerSpeed>::fallback( ) ) );
					}

					preset_same_line( );

					if ( preset_button( intl_->tr( "Fastest" ), ButtonVariant::Primary ) )
					{
						set_status( mouse_->set<PointerSpeed>( 20u ) );
					}
				}
			);
			ImGui::PopID( );
		} );

		section( ICON_LC_TIMER, intl_->tr( "Latency" ), intl_->tr( "Input buffering and queue size." ), [ & ]
		{
			ImGui::PushID( "Mouse data queue size" );
			settings_row(
				intl_->tr( "Mouse data queue size" ),
				intl_->tr( "Buffered input events the driver keeps before reporting movement." ),
				control_width, control_height,
				Level::Medium, Level::Low,
				intl_->tr( "Optimized" ),
				[ & ]
				{
					if ( preset_button( intl_->tr( "Default" ), ButtonVariant::Ghost ) )
					{
						set_status( mouse_->set<DataQueueSize>( FeatureTraits<DataQueueSize>::fallback( ) ) );
					}

					preset_same_line( );

					if ( preset_button( intl_->tr( "Optimized" ), ButtonVariant::Primary ) )
					{
						set_status( mouse_->set<DataQueueSize>( optimized_queue_size ) );
					}
				}
			);
			ImGui::PopID( );
		} );

		if ( !status_.empty( ) )
		{
			ImGui::TextColored( Theme::Danger, "%s", status_.c_str( ) );
		}
	}
}

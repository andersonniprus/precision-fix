#include "Stdafx.hpp"
#include "UI/Pages/MousePage.hpp"
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

	MousePage::MousePage( std::shared_ptr<Modules::MouseModule> mouse )
		: mouse_( std::move( mouse ) )
	{
	}

	void MousePage::on_activate( )
	{
		mouse_->refresh_all( );
	}

	void MousePage::set_status( const Core::Status& status )
	{
		status_ = status ? std::string {} : std::string { Core::to_string( status.error( ) ) };
	}

	void MousePage::render( )
	{
		const float switch_width  = ImGui::GetFrameHeight( ) * switch_aspect;
		const float switch_height = ImGui::GetFrameHeight( );

		const float control_width  = preset_row_width( 2 );
		const float control_height = preset_button_height( );

		section( ICON_LC_CROSSHAIR, "Precision", "Pointer acceleration and speed.", [ & ]
		{
			bool precision = mouse_->get<PointerPrecision>( );

			settings_row(
				"Enhance pointer precision",
				"Applies Windows' acceleration curve based on movement speed.",
				switch_width, switch_height,
				Level::Low, Level::Low,
				"Off",
				[ & ]
				{
					if ( toggle_switch( "Enhance pointer precision", &precision ) )
					{
						set_status( mouse_->set<PointerPrecision>( precision ) );
					}
				}
			);

			ImGui::PushID( "Smooth mouse X curve" );
			settings_row(
				"Smooth mouse X curve",
				"Horizontal acceleration curve. Optimized removes added acceleration.",
				control_width, control_height,
				Level::Low, Level::Low,
				"Optimized",
				[ & ]
				{
					if ( preset_button( "Default", ButtonVariant::Ghost ) )
					{
						set_status( mouse_->set<SmoothMouseXCurve>( FeatureTraits<SmoothMouseXCurve>::fallback( ) ) );
					}

					preset_same_line( );

					if ( preset_button( "Optimized", ButtonVariant::Primary ) )
					{
						set_status( mouse_->set<SmoothMouseXCurve>( optimized_curve ) );
					}
				}
			);
			ImGui::PopID( );

			ImGui::PushID( "Smooth mouse Y curve" );
			settings_row(
				"Smooth mouse Y curve",
				"Vertical acceleration curve. Optimized removes added acceleration.",
				control_width, control_height,
				Level::Low, Level::Low,
				"Optimized",
				[ & ]
				{
					if ( preset_button( "Default", ButtonVariant::Ghost ) )
					{
						set_status( mouse_->set<SmoothMouseYCurve>( FeatureTraits<SmoothMouseYCurve>::fallback( ) ) );
					}

					preset_same_line( );

					if ( preset_button( "Optimized", ButtonVariant::Primary ) )
					{
						set_status( mouse_->set<SmoothMouseYCurve>( optimized_curve ) );
					}
				}
			);
			ImGui::PopID( );

			ImGui::PushID( "Pointer speed" );
			settings_row(
				"Pointer speed",
				"Windows pointer-speed slider setting, from 1 (slowest) to 20 (fastest).",
				control_width, control_height,
				Level::Low, Level::Low,
				"Fastest",
				[ & ]
				{
					if ( preset_button( "Default", ButtonVariant::Ghost ) )
					{
						set_status( mouse_->set<PointerSpeed>( FeatureTraits<PointerSpeed>::fallback( ) ) );
					}

					preset_same_line( );

					if ( preset_button( "Fastest", ButtonVariant::Primary ) )
					{
						set_status( mouse_->set<PointerSpeed>( 20u ) );
					}
				}
			);
			ImGui::PopID( );
		} );

		section( ICON_LC_TIMER, "Latency", "Input buffering and queue size.", [ & ]
		{
			ImGui::PushID( "Mouse data queue size" );
			settings_row(
				"Mouse data queue size",
				"Buffered input events the driver keeps before reporting movement.",
				control_width, control_height,
				Level::Medium, Level::Low,
				"Optimized",
				[ & ]
				{
					if ( preset_button( "Default", ButtonVariant::Ghost ) )
					{
						set_status( mouse_->set<DataQueueSize>( FeatureTraits<DataQueueSize>::fallback( ) ) );
					}

					preset_same_line( );

					if ( preset_button( "Optimized", ButtonVariant::Primary ) )
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

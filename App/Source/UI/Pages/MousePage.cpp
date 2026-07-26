#include "Stdafx.hpp"
#include "UI/Pages/MousePage.hpp"
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

	template<Modules::MouseFeature F>
	void MousePage::render_curve_presets( const char* title, const char* description )
	{
		ImGui::PushID( title );

		Widgets::settings_row( title, description, Widgets::preset_row_width( 2 ), ImGui::GetFrameHeight( ), [ & ]
		{
			if ( ImGui::Button( "Default", ImVec2( Widgets::preset_button_width, 0.f ) ) )
				set_status( mouse_->set<F>( Modules::FeatureTraits<F>::fallback( ) ) );

			ImGui::SameLine( );

			if ( ImGui::Button( "Optimized", ImVec2( Widgets::preset_button_width, 0.f ) ) )
				set_status( mouse_->set<F>( optimized_curve ) );
		} );

		ImGui::PopID( );
	}

	void MousePage::render_precision( )
	{
		const float switch_width  = ImGui::GetFrameHeight( ) * Widgets::switch_aspect;
		const float switch_height = ImGui::GetFrameHeight( );

		bool precision = mouse_->get<Modules::MouseFeature::PointerPrecision>( );

		Widgets::settings_row(
			"Enhance pointer precision",
			"Applies Windows' acceleration curve based on movement speed.",
			switch_width,
			switch_height,
			[ & ]
			{
				if ( Widgets::toggle_switch( "Enhance pointer precision", &precision ) )
					set_status( mouse_->set<Modules::MouseFeature::PointerPrecision>( precision ) );
			}
		);

		ImGui::Separator( );

		render_curve_presets<Modules::MouseFeature::SmoothMouseXCurve>(
			"Smooth mouse X curve", "Horizontal acceleration curve. Optimized removes added acceleration."
		);

		ImGui::Separator( );

		render_curve_presets<Modules::MouseFeature::SmoothMouseYCurve>(
			"Smooth mouse Y curve", "Vertical acceleration curve. Optimized removes added acceleration."
		);
	}

	void MousePage::render_latency( )
	{
		Widgets::settings_row(
			"Mouse data queue size",
			"Buffered input events the driver keeps before reporting movement.",
			Widgets::preset_row_width( 2 ),
			ImGui::GetFrameHeight( ),
			[ & ]
			{
				if ( ImGui::Button( "Default", ImVec2( Widgets::preset_button_width, 0.f ) ) )
					set_status( mouse_->set<Modules::MouseFeature::DataQueueSize>(
						Modules::FeatureTraits<Modules::MouseFeature::DataQueueSize>::fallback( ) ) );

				ImGui::SameLine( );

				if ( ImGui::Button( "Optimized", ImVec2( Widgets::preset_button_width, 0.f ) ) )
					set_status( mouse_->set<Modules::MouseFeature::DataQueueSize>( optimized_queue_size ) );
			}
		);
	}

	void MousePage::render( )
	{
		if ( ImGui::CollapsingHeader( "Precision", ImGuiTreeNodeFlags_DefaultOpen ) )
			render_precision( );

		if ( ImGui::CollapsingHeader( "Latency", ImGuiTreeNodeFlags_DefaultOpen ) )
			render_latency( );

		if ( !status_.empty( ) )
			ImGui::TextColored( ImVec4( 1.f, 0.35f, 0.35f, 1.f ), "%s", status_.c_str( ) );
	}
}

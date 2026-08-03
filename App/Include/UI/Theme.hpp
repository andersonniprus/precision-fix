#pragma once

#pragma comment( lib, "dwmapi.lib" )

#ifndef DWMWA_WINDOW_CORNER_PREFERENCE
#define DWMWA_WINDOW_CORNER_PREFERENCE 33
#endif
#ifndef DWMWA_BORDER_COLOR
#define DWMWA_BORDER_COLOR 34
#endif
#ifndef DWMWA_CAPTION_COLOR
#define DWMWA_CAPTION_COLOR 35
#endif
#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
#endif
#ifndef DWMWCP_DEFAULT
#define DWMWCP_DEFAULT 0
#endif
#ifndef DWMWCP_ROUND
#define DWMWCP_ROUND 2
#endif
#ifndef DWMWA_COLOR_NONE
#define DWMWA_COLOR_NONE 0xFFFFFFFE
#endif

namespace UI::Theme
{
	enum class Mode : std::uint8_t
	{
		Dark,
		Light,
	};

	struct Palette
	{
		ImVec4 background;
		ImVec4 card;
		ImVec4 popover;
		ImVec4 elevated;
		ImVec4 secondary;
		ImVec4 foreground;
		ImVec4 secondary_text;
		ImVec4 muted_text;
		ImVec4 primary;
		ImVec4 success;
		ImVec4 warning;
		ImVec4 danger;
		ImVec4 border;
		ImVec4 surface_soft;
		ImVec4 surface_hover;
		COLORREF gdi_background;
		BOOL immersive_dark;
	};

	inline constexpr Palette kDark {
		.background = { 0.010f, 0.012f, 0.011f, 1.f },
		.card = { 0.016f, 0.020f, 0.017f, 1.f },
		.popover = { 0.028f, 0.034f, 0.030f, 1.f },
		.elevated = { 0.040f, 0.050f, 0.043f, 1.f },
		.secondary = { 0.070f, 0.085f, 0.074f, 1.f },
		.foreground = { 0.890f, 0.920f, 0.898f, 1.f },
		.secondary_text = { 0.620f, 0.660f, 0.628f, 1.f },
		.muted_text = { 0.430f, 0.475f, 0.443f, 1.f },
		.primary = { 0.235f, 0.780f, 0.455f, 1.f },
		.success = { 0.176f, 0.788f, 0.404f, 1.f },
		.warning = { 0.961f, 0.620f, 0.043f, 1.f },
		.danger = { 0.937f, 0.267f, 0.267f, 1.f },
		.border = { 1.f, 1.f, 1.f, 0.06f },
		.surface_soft = { 0.235f, 0.780f, 0.455f, 0.06f },
		.surface_hover = { 0.235f, 0.780f, 0.455f, 0.10f },
		.gdi_background = RGB( 0x03, 0x03, 0x03 ),
		.immersive_dark = TRUE,
	};

	inline constexpr Palette kLight {
		.background = { 0.965f, 0.970f, 0.966f, 1.f },
		.card = { 0.940f, 0.948f, 0.942f, 1.f },
		.popover = { 1.f, 1.f, 1.f, 1.f },
		.elevated = { 0.910f, 0.920f, 0.912f, 1.f },
		.secondary = { 0.840f, 0.860f, 0.845f, 1.f },
		.foreground = { 0.090f, 0.110f, 0.095f, 1.f },
		.secondary_text = { 0.320f, 0.360f, 0.330f, 1.f },
		.muted_text = { 0.450f, 0.490f, 0.460f, 1.f },
		.primary = { 0.120f, 0.620f, 0.340f, 1.f },
		.success = { 0.120f, 0.620f, 0.340f, 1.f },
		.warning = { 0.820f, 0.520f, 0.040f, 1.f },
		.danger = { 0.820f, 0.220f, 0.220f, 1.f },
		.border = { 0.f, 0.f, 0.f, 0.10f },
		.surface_soft = { 0.120f, 0.620f, 0.340f, 0.08f },
		.surface_hover = { 0.120f, 0.620f, 0.340f, 0.14f },
		.gdi_background = RGB( 0xF6, 0xF7, 0xF6 ),
		.immersive_dark = FALSE,
	};

	inline Mode CurrentMode = Mode::Dark;
	inline ImVec4 Background = kDark.background;
	inline ImVec4 Card = kDark.card;
	inline ImVec4 Popover = kDark.popover;
	inline ImVec4 Elevated = kDark.elevated;
	inline ImVec4 Secondary = kDark.secondary;
	inline ImVec4 Foreground = kDark.foreground;
	inline ImVec4 SecondaryText = kDark.secondary_text;
	inline ImVec4 MutedText = kDark.muted_text;
	inline ImVec4 Primary = kDark.primary;
	inline ImVec4 Success = kDark.success;
	inline ImVec4 Warning = kDark.warning;
	inline ImVec4 Danger = kDark.danger;
	inline ImVec4 Border = kDark.border;
	inline ImVec4 SurfaceSoft = kDark.surface_soft;
	inline ImVec4 SurfaceHover = kDark.surface_hover;

	inline constexpr float SeparatorAlpha = 0.18f;
	inline constexpr float SeparatorThickness = 1.5f;
	inline constexpr float RadiusBase = 8.f;
	inline constexpr float HeaderHBase = 44.f;

	inline float DpiScale = 1.f;
	inline float Radius = RadiusBase;
	inline float HeaderH = HeaderHBase;

	inline void set_dpi_scale( const float scale ) noexcept
	{
		DpiScale = ( scale > 0.f ) ? scale : 1.f;
		Radius = RadiusBase * DpiScale;
		HeaderH = HeaderHBase * DpiScale;
	}

	[[nodiscard]] inline float px( const float v ) noexcept
	{
		return v * DpiScale;
	}

	[[nodiscard]] inline ImU32 col( const ImVec4& c, const float a = -1.f ) noexcept
	{
		return IM_COL32(
			static_cast<int>( c.x * 255.f + 0.5f ),
			static_cast<int>( c.y * 255.f + 0.5f ),
			static_cast<int>( c.z * 255.f + 0.5f ),
			static_cast<int>( ( a < 0.f ? c.w : a ) * 255.f + 0.5f )
		);
	}

	[[nodiscard]] inline ImU32 separator_col( ) noexcept
	{
		return col( Border, SeparatorAlpha );
	}

	[[nodiscard]] inline Mode mode( ) noexcept
	{
		return CurrentMode;
	}

	[[nodiscard]] inline COLORREF background_gdi( ) noexcept
	{
		return CurrentMode == Mode::Light ? kLight.gdi_background : kDark.gdi_background;
	}

	inline void set_mode( const Mode mode ) noexcept
	{
		CurrentMode = mode;
		const Palette& palette = mode == Mode::Light ? kLight : kDark;

		Background = palette.background;
		Card = palette.card;
		Popover = palette.popover;
		Elevated = palette.elevated;
		Secondary = palette.secondary;
		Foreground = palette.foreground;
		SecondaryText = palette.secondary_text;
		MutedText = palette.muted_text;
		Primary = palette.primary;
		Success = palette.success;
		Warning = palette.warning;
		Danger = palette.danger;
		Border = palette.border;
		SurfaceSoft = palette.surface_soft;
		SurfaceHover = palette.surface_hover;
	}

	inline void apply( )
	{
		if ( CurrentMode == Mode::Light )
			ImGui::StyleColorsLight( );
		else
			ImGui::StyleColorsDark( );

		ImGuiStyle& s = ImGui::GetStyle( );

		s.WindowRounding = s.PopupRounding = Radius;
		s.ChildRounding = 0.f;
		s.FrameRounding = s.TabRounding = px( 6.f );
		s.ScrollbarRounding = s.GrabRounding = px( 4.f );
		s.WindowBorderSize = s.PopupBorderSize = 1.f;
		s.ChildBorderSize = 0.f;
		s.FrameBorderSize = 0.f;
		s.WindowPadding = { 0.f, 0.f };
		s.FramePadding = { px( 10.f ), px( 6.f ) };
		s.ItemSpacing = { px( 8.f ), px( 6.f ) };
		s.ScrollbarSize = px( 6.f );

		s.AntiAliasedLines = true;
		s.AntiAliasedLinesUseTex = true;
		s.AntiAliasedFill = true;
		s.CurveTessellationTol = 0.75f;
		s.CircleTessellationMaxError = 0.20f;

		auto* c = s.Colors;
		c[ ImGuiCol_Text ] = Foreground;
		c[ ImGuiCol_TextDisabled ] = MutedText;
		c[ ImGuiCol_WindowBg ] = Background;
		c[ ImGuiCol_ChildBg ] = Background;
		c[ ImGuiCol_PopupBg ] = Popover;
		c[ ImGuiCol_Border ] = Border;
		c[ ImGuiCol_BorderShadow ] = {};
		c[ ImGuiCol_FrameBg ] = SurfaceSoft;
		c[ ImGuiCol_FrameBgHovered ] = SurfaceHover;
		c[ ImGuiCol_FrameBgActive ] = Secondary;
		c[ ImGuiCol_TitleBg ] =
		c[ ImGuiCol_TitleBgActive ] =
		c[ ImGuiCol_TitleBgCollapsed ] =
		c[ ImGuiCol_MenuBarBg ] = Background;
		c[ ImGuiCol_ScrollbarBg ] = {};
		c[ ImGuiCol_ScrollbarGrab ] = CurrentMode == Mode::Light
			? ImVec4 { 0.f, 0.f, 0.f, 0.12f }
			: ImVec4 { 1.f, 1.f, 1.f, 0.08f };
		c[ ImGuiCol_ScrollbarGrabHovered ] = CurrentMode == Mode::Light
			? ImVec4 { 0.f, 0.f, 0.f, 0.20f }
			: ImVec4 { 1.f, 1.f, 1.f, 0.15f };
		c[ ImGuiCol_ScrollbarGrabActive ] = CurrentMode == Mode::Light
			? ImVec4 { 0.f, 0.f, 0.f, 0.28f }
			: ImVec4 { 1.f, 1.f, 1.f, 0.22f };
		c[ ImGuiCol_CheckMark ] =
		c[ ImGuiCol_SliderGrab ] =
		c[ ImGuiCol_SliderGrabActive ] = Primary;
		c[ ImGuiCol_Button ] = SurfaceSoft;
		c[ ImGuiCol_ButtonHovered ] = { Primary.x, Primary.y, Primary.z, 0.16f };
		c[ ImGuiCol_ButtonActive ] = { Primary.x, Primary.y, Primary.z, 0.28f };
		c[ ImGuiCol_Header ] = SurfaceSoft;
		c[ ImGuiCol_HeaderHovered ] = { Primary.x, Primary.y, Primary.z, 0.12f };
		c[ ImGuiCol_HeaderActive ] = { Primary.x, Primary.y, Primary.z, 0.22f };
		c[ ImGuiCol_TabHovered ] = SurfaceHover;
		c[ ImGuiCol_Separator ] = Border;
		c[ ImGuiCol_SeparatorHovered ] =
		c[ ImGuiCol_SeparatorActive ] = Primary;
		c[ ImGuiCol_NavHighlight ] = Primary;
		c[ ImGuiCol_Tab ] =
		c[ ImGuiCol_TabUnfocused ] = Elevated;
		c[ ImGuiCol_TabActive ] =
		c[ ImGuiCol_TabUnfocusedActive ] = Background;
		c[ ImGuiCol_TextSelectedBg ] = { Primary.x, Primary.y, Primary.z, 0.28f };
	}

	inline void clear_window_rgn( const HWND hwnd )
	{
		if ( hwnd )
			SetWindowRgn( hwnd, nullptr, TRUE );
	}

	inline void apply_chrome( const HWND hwnd )
	{
		if ( !hwnd )
			return;

		constexpr DWORD corner = DWMWCP_ROUND;
		const COLORREF bg = background_gdi( );
		const BOOL dark = CurrentMode == Mode::Dark ? TRUE : FALSE;
		constexpr COLORREF no_border = DWMWA_COLOR_NONE;

		DwmSetWindowAttribute( hwnd, DWMWA_WINDOW_CORNER_PREFERENCE, &corner, sizeof( corner ) );
		DwmSetWindowAttribute( hwnd, DWMWA_CAPTION_COLOR, &bg, sizeof( bg ) );
		DwmSetWindowAttribute( hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &dark, sizeof( dark ) );
		DwmSetWindowAttribute( hwnd, DWMWA_BORDER_COLOR, &no_border, sizeof( no_border ) );

		clear_window_rgn( hwnd );

		SetWindowPos(
			hwnd,
			nullptr,
			0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED
		);
	}

	inline void draw_header_ornament( ImDrawList* dl, const ImVec2 min, const ImVec2 max, const ImVec4 accent = Primary )
	{
		if ( !dl )
			return;

		const float width = max.x - min.x;
		const float height = max.y - min.y;

		if ( width <= 0.f || height <= 0.f )
			return;

		dl->PushClipRect( min, max, true );

		const float time = static_cast<float>( ImGui::GetTime( ) );
		const ImVec2 focus { max.x - height * 0.55f, min.y + height * 0.52f };

		for ( int i = 5; i >= 1; --i )
		{
			const float radius = height * ( 0.55f + static_cast<float>( i ) * 0.42f );
			dl->AddCircleFilled( focus, radius, col( accent, 0.010f * static_cast<float>( i ) ), 64 );
		}

		dl->AddRectFilledMultiColor(
			{ max.x - width * 0.38f, min.y },
			max,
			col( accent, 0.f ),
			col( accent, 0.07f ),
			col( accent, 0.07f ),
			col( accent, 0.f )
		);

		for ( int ring = 0; ring < 3; ++ring )
		{
			const float radius = height * ( 0.62f + static_cast<float>( ring ) * 0.48f );
			const float spin = time * ( 0.18f + static_cast<float>( ring ) * 0.07f ) + static_cast<float>( ring ) * 1.35f;
			const float span = 1.55f + static_cast<float>( ring ) * 0.35f;
			const float alpha = 0.22f - static_cast<float>( ring ) * 0.05f;
			const float thick = 1.1f + static_cast<float>( ring ) * 0.25f;

			dl->PathClear( );
			dl->PathArcTo( focus, radius, spin, spin + span, 40 );
			dl->PathStroke( col( accent, alpha ), ImDrawFlags_None, thick );

			dl->PathClear( );
			dl->PathArcTo( focus, radius, spin + 3.2f, spin + 3.2f + span * 0.45f, 24 );
			dl->PathStroke( col( accent, alpha * 0.55f ), ImDrawFlags_None, thick * 0.8f );
		}

		{
			const float radius = height * 1.55f;
			const float base = time * 0.12f;

			for ( int i = 0; i < 18; ++i )
			{
				const float a = base + static_cast<float>( i ) * ( 6.2831853f / 18.f );
				const float major = ( i % 3 == 0 );
				const float inner = radius - ( major ? height * 0.16f : height * 0.08f );
				const float ca = std::cos( a );
				const float sa = std::sin( a );

				dl->AddLine(
					{ focus.x + ca * inner, focus.y + sa * inner },
					{ focus.x + ca * radius, focus.y + sa * radius },
					col( accent, major ? 0.28f : 0.12f ),
					major ? 1.4f : 1.f
				);
			}
		}

		const float core = height * 0.10f;
		dl->AddCircle( focus, core, col( accent, 0.35f ), 24, 1.2f );
		dl->AddCircleFilled( focus, core * 0.28f, col( accent, 0.55f ), 16 );
		dl->AddLine( { focus.x - core * 1.7f, focus.y }, { focus.x - core * 0.55f, focus.y }, col( accent, 0.30f ), 1.f );
		dl->AddLine( { focus.x + core * 0.55f, focus.y }, { focus.x + core * 1.7f, focus.y }, col( accent, 0.30f ), 1.f );
		dl->AddLine( { focus.x, focus.y - core * 1.7f }, { focus.x, focus.y - core * 0.55f }, col( accent, 0.30f ), 1.f );
		dl->AddLine( { focus.x, focus.y + core * 0.55f }, { focus.x, focus.y + core * 1.7f }, col( accent, 0.30f ), 1.f );

		for ( int i = 0; i < 4; ++i )
		{
			const float orbit = height * ( 0.85f + static_cast<float>( i ) * 0.22f );
			const float a = time * ( 0.55f - static_cast<float>( i ) * 0.08f ) + static_cast<float>( i ) * 1.7f;
			const ImVec2 p {
				focus.x + std::cos( a ) * orbit,
				focus.y + std::sin( a ) * orbit * 0.72f
			};

			if ( p.x < min.x || p.x > max.x || p.y < min.y || p.y > max.y )
				continue;

			dl->AddCircleFilled( p, 2.2f + static_cast<float>( i % 2 ), col( accent, 0.45f ), 12 );
			dl->AddCircleFilled( p, 5.5f + static_cast<float>( i ), col( accent, 0.08f ), 16 );
		}

		{
			const float phase = 0.5f + 0.5f * std::sin( time * 0.9f );
			const float y = min.y + height * ( 0.22f + phase * 0.56f );
			dl->AddLine( { min.x + width * 0.45f, y }, { max.x, y }, col( accent, 0.10f + phase * 0.08f ), 1.f );
		}

		dl->PopClipRect( );
	}
}

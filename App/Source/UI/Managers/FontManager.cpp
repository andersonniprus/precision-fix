#include "Stdafx.hpp"
#include "UI/Managers/FontManager.hpp"
#include "UI/Fonts/Regular.hpp"
#include "UI/Fonts/Medium.hpp"
#include "UI/Fonts/LucideIcons.hpp"
#include "UI/Fonts/IconsLucide.h"

namespace UI::Managers
{
	FontManager::FontManager( )
	{
		add_font( FontType::Regular, { satoshi_regular_bin, sizeof( satoshi_regular_bin ) }, { 16.0f } );
		add_font( FontType::Medium, { satoshi_medium_bin, sizeof( satoshi_medium_bin ) }, { 16.0f } );

		add_font( FontType::Icons, { lucide_icons_bin, sizeof( lucide_icons_bin ) }, { 14.0f, 13.0f } );

		ImFontConfig primary_config;
		primary_config.OversampleH        = 2;
		primary_config.OversampleV        = 1;
		primary_config.PixelSnapH         = false;
		primary_config.RasterizerMultiply = 1.08f;
		set_font_config( FontType::Regular, primary_config );
		set_font_config( FontType::Medium, primary_config );

		ImFontConfig icons_config;
		icons_config.PixelSnapH         = true;
		icons_config.OversampleH        = 2;
		icons_config.OversampleV        = 1;
		icons_config.RasterizerMultiply = 1.05f;

		constexpr ImWchar icon_ranges[ ] = { ICON_MIN_LC, ICON_MAX_LC, 0 };

		set_font_config( FontType::Icons, icons_config );
		set_font_ranges( FontType::Icons, icon_ranges );
	}

	void FontManager::add_font( const FontType type, const std::span<const unsigned char> data, const std::vector<float>& initial_sizes )
	{
		fonts_[ type ] = FontData {
			.data = data,
			.initial_sizes = initial_sizes
		};
	}

	void FontManager::set_font_config( const FontType type, const ImFontConfig& config )
	{
		if ( const auto it = fonts_.find( type ); it != fonts_.end( ) )
		{
			auto& font                       = it->second;
			font.config                      = config;
			font.config.FontDataOwnedByAtlas = false;
			font.config_set                  = true;
		}
	}

	void FontManager::set_font_ranges( const FontType type, const ImWchar* ranges )
	{
		auto it = fonts_.find( type );
		if ( it == fonts_.end( ) )
			return;

		auto& font = it->second;
		font.ranges.clear( );

		std::ranges::copy(
			default_ranges | std::views::take_while( []( const ImWchar c )
			{
				return c != 0;
			} ),
			std::back_inserter( font.ranges )
		);

		if ( ranges )
		{
			for ( const ImWchar* p = ranges; *p != 0; ++p )
			{
				font.ranges.push_back( *p );
			}
		}

		font.ranges.push_back( 0 );
	}

	ImFont* FontManager::get( const FontType type, const float size )
	{
		const auto& io = ImGui::GetIO( ).Fonts;

		const auto it = fonts_.find( type );
		if ( it == fonts_.end( ) )
			return io->Fonts.front( );

		auto& font_data = it->second;

		if ( const auto size_it = font_data.loaded_fonts.find( size ); size_it != font_data.loaded_fonts.end( ) )
			return size_it->second;

		needs_rebuild_ = true;
		font_data.initial_sizes.push_back( size );

		return io->Fonts.front( );
	}

	void FontManager::load_font( FontData& font_data, const float size )
	{
		const auto& io = ImGui::GetIO( ).Fonts;

		const auto ranges = font_data.ranges.empty( ) ? io->GetGlyphRangesDefault( ) : font_data.ranges.data( );

		ImFontConfig config = font_data.config_set ? font_data.config : ImFontConfig( );

		config.FontDataOwnedByAtlas = false;

		if ( !font_data.config_set )
		{
			config.OversampleH        = 2;
			config.OversampleV        = 1;
			config.RasterizerMultiply = 1.08f;
		}

		font_data.loaded_fonts[ size ] = io->AddFontFromMemoryTTF(
			const_cast<unsigned char*>( font_data.data.data( ) ),
			static_cast<int>( font_data.data.size( ) ),
			size,
			&config,
			ranges
		);
	}

	void FontManager::build_font_atlas( )
	{
		const auto& io = ImGui::GetIO( ).Fonts;
		io->Clear( );

		const FontData* icon_data  = nullptr;
		const ImWchar* icon_ranges = nullptr;

		if ( const auto icon_it = fonts_.find( FontType::Icons ); icon_it != fonts_.end( ) )
		{
			icon_data = &icon_it->second;

			if ( !icon_data->ranges.empty( ) )
			{
				icon_ranges = icon_data->ranges.data( );
			}
		}

		auto add_icon_font = [&]( const float base_size )
		{
			if ( !icon_data || icon_data->data.empty( ) || !icon_ranges )
				return;

			ImFontConfig merge_config;
			merge_config.MergeMode            = true;
			merge_config.PixelSnapH           = true;
			merge_config.OversampleH          = 2;
			merge_config.OversampleV          = 1;
			merge_config.RasterizerMultiply   = 1.05f;
			merge_config.FontDataOwnedByAtlas = false;
			merge_config.GlyphOffset.y        = 1.f;

			io->AddFontFromMemoryTTF(
				const_cast<unsigned char*>( icon_data->data.data( ) ),
				static_cast<int>( icon_data->data.size( ) ),
				base_size - 2.f,
				&merge_config,
				icon_ranges
			);
		};

		for ( auto& [ type, font_data ] : fonts_ )
		{
			if ( type == FontType::Icons )
				continue;

			for ( const float size : font_data.initial_sizes )
			{
				load_font( font_data, size );
				add_icon_font( size );
			}
		}

		io->Build( );
	}

	void FontManager::rebuild_font_atlas_if_needed( )
	{
		if ( needs_rebuild_ )
		{
			build_font_atlas( );
			ImGui_ImplDX11_CreateDeviceObjects( );
			needs_rebuild_ = false;
		}
	}
}

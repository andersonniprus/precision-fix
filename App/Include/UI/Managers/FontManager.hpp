#pragma once

namespace UI::Managers
{
	enum class FontType : std::uint8_t
	{
		Regular,
		Medium,
		Icons,
		COUNT
	};

	class FontManager
	{
	public:
		FontManager( );
		~FontManager( ) = default;

		FontManager( const FontManager& )            = delete;
		FontManager& operator=( const FontManager& ) = delete;

		void add_font( FontType, std::span<const unsigned char>, const std::vector<float>& );

		void set_font_config( FontType, const ImFontConfig& );
		void set_font_ranges( FontType, const ImWchar* );

		[[nodiscard]] ImFont* get( FontType, float );

		void build_font_atlas( );
		void rebuild_font_atlas_if_needed( );

	private:
		struct FontData
		{
			std::span<const unsigned char> data;
			std::vector<float> initial_sizes;
			std::unordered_map<float, ImFont*> loaded_fonts;
			ImFontConfig config;
			std::vector<ImWchar> ranges;
			bool config_set = false;
		};

		static constexpr ImWchar default_ranges[ ] =
		{
			0x0020, 0x00FF, 0x0300, 0x036F, 0x2000, 0x206F, 0
		};

		std::unordered_map<FontType, FontData> fonts_;
		bool needs_rebuild_ = false;

		static void load_font( FontData&, float );
	};
}

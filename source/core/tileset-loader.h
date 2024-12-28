#ifndef TILESET_LOADER_NTRACER_HH
#define TILESET_LOADER_NTRACER_HH

#include <core/common-types.h>
#include <SFML/Graphics/Image.hpp>

namespace core {
	class TilesetLoader {
	public:
		bool LoadImage(std::string_view filepath, const uint16_t width, const uint16_t height);

		void SelectTile(sf::Sprite& sprite, const uint16_t x, const uint16_t y);

		void PrevTileset();
		void NextTileset();

		sf::Texture	r_texture;
		sf::Image	r_image;

		uint16_t m_state{};
		int16_t  m_index{};
		int16_t  m_count{};

		vector2f	dimension{};
		vector2f	tile_size{ 8.0f, 8.0f };
		vector2f	tile_present;

		vector2u	min, max;

		float		draw_scale{ 3.0f };

		TilesetLoader();

		TilesetLoader(const TilesetLoader&) = delete;
		TilesetLoader& operator=(const TilesetLoader&) = delete;

		TilesetLoader(TilesetLoader&&) = delete;
		TilesetLoader& operator=(TilesetLoader&&) = delete;

		~TilesetLoader();

	private:
		void update_tileset();

	};
}

#endif

#ifndef TILESET_LOADER_NTRACER_HH
#define TILESET_LOADER_NTRACER_HH

#include "custom_types.h"

struct upoints_t {
	vector2u min;
	vector2u max;
};

class TilesetLoader {
public:
	sf::Sprite		m_tile{};
	sf::Texture		m_tileset{};
	sf::Image		m_raw_tileset{};

	int16_t			m_index{};
	int16_t			m_count{};
	uint8_t			m_state{};

	upoints_t		m_points{};
	vector2u		m_dimension{};
	vector2f		m_tilesize{ 8.0f, 8.0f };

	vector2f		rect_min{};
	vector2f		rect_max{};

	float			scale_level{3.0};

	bool LoadImage(std::string_view filepath, const vector2u& tilesize);

	void PrevTileset();
	void NextTileset();

	void HoveredTile(const uint16_t x, const uint16_t y);
	void SelectTile(const uint16_t x, const uint16_t y);

private:
	void update_current_tileset();
};

inline TilesetLoader loader{};

#define TS_LOADER	loader

#endif

#ifndef TILESET_LOADER_NTRACER_HH
#define TILESET_LOADER_NTRACER_HH

#include "custom_types.h"

struct TilePoints {
	vector2u min{};
	vector2u max{};
};

class TilesetLoader {
public:
	sf::Texture target{};
	bool target_state{};

	TilePoints to_draw{};

	int index{};
	int count{};

	bool LoadImage(std::string_view filepath, const vector2u& tilesize);

	sf::Image& GetTilesetImage();

	void PrevTileset();
	void NextTileset();

	static TilesetLoader& Instance()
	{
		static TilesetLoader loader{};
		return loader;
	}

private:
	TilesetLoader();
	~TilesetLoader();

	void update_current_tileset();
};

inline struct live_properties {
	vector2u tilesize{ 8u, 8u };
	uint16_t u_id{};

	vector2f min{};
	vector2f max{};

	float scale = 3.0f;

	sf::Sprite live_part{};

	void HoveringTile(const uint16_t x, const uint16_t y);

	void PickingTile(const uint16_t x, const uint16_t y);

} current;

#define TS_LOADER		TilesetLoader::Instance()
#define TS_CURRENT		current

#endif

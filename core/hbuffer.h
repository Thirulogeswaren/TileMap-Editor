#ifndef TILESET_LOADER_NTRACER_HH
#define TILESET_LOADER_NTRACER_HH

#include "SFML/Graphics/Texture.hpp"

class TilesetLoader {
public:
	sf::Texture target;
	bool is_target_active;

	int index;
	int count;

	bool LoadImage(std::string_view filepath, const sf::Vector2i& tilesize);

	void MovePointerL();
	void MovePointerR();

	static TilesetLoader& Instance()
	{
		static TilesetLoader loader{};
		return loader;
	}

private:
	TilesetLoader();
	~TilesetLoader();
};

inline struct live_properties {
	sf::Vector2u tsize = { 8u, 8u };

	sf::Vector2f min = { 0.0f, 0.0f };
	sf::Vector2f max = { 0.0f, 0.0f };
	size_t u_id = 0;

	float scale = 2.0f;

	void setTile(const int x, const int y);

} current;

inline sf::Vector2i mouse_position;

#define TS_LOADER		TilesetLoader::Instance()
#define TS_CURRENT		current

#endif

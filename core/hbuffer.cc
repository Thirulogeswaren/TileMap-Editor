#include "hbuffer.h"
#include "../util/console.h"

#include <SFML/System/Vector3.hpp>

struct hbuffer
{
	sf::Image image;
	sf::Vector2u tilesz;
	float zoom_level;

	hbuffer() : image{ }, tilesz{ 8u, 8u }, zoom_level{ 1.5f } { }
};

struct tile_properties {
	std::vector<sf::Vector3i> unique_id;
	std::vector<sf::Vector2f> min;
	std::vector<sf::Vector2f> max;
};


#include <unordered_map>

static std::vector<hbuffer> c_tileset;
static std::unordered_map<size_t, tile_properties> tile_props;

TilesetLoader::TilesetLoader() : target{}, is_target_active{ false }, index { 0 }, count{ 0 }
{
	Console::LogMessage("TS_LOADER active");
	c_tileset.reserve(30);
}

TilesetLoader::~TilesetLoader()
{
	c_tileset.clear();
}

static void update_current_tileset()
{

	if (TS_LOADER.index >= c_tileset.size()) { TS_LOADER.index--; }

	if (TS_LOADER.index <= 0) { TS_LOADER.index = 0; }

	// set the current properties
	TS_LOADER.is_target_active = {
		TS_LOADER.target.loadFromImage(c_tileset[TS_LOADER.index].image)
	};

	TS_CURRENT.tsize.x = c_tileset[TS_LOADER.index].tilesz.x;
	TS_CURRENT.tsize.y = c_tileset[TS_LOADER.index].tilesz.y;
	TS_CURRENT.scale = c_tileset[TS_LOADER.index].zoom_level;
	TS_CURRENT.min = TS_CURRENT.max = { 0.0f, 0.0f };
	TS_CURRENT.u_id = 0;
}

bool TilesetLoader::LoadImage(std::string_view filepath, const sf::Vector2i& tilesize)
{
	if (hbuffer temp{}; temp.image.loadFromFile(filepath.data()))
	{
		temp.tilesz.x = tilesize.x;
		temp.tilesz.y = tilesize.y;

		uint32_t tiles_in_x = (temp.image.getSize().x / temp.tilesz.x) + 1;
		uint32_t tiles_in_y = (temp.image.getSize().y / temp.tilesz.y) + 1;

		uint32_t tiles_present = tiles_in_x * tiles_in_y;

		if (!c_tileset.empty())
			c_tileset[TS_LOADER.index].zoom_level = TS_CURRENT.scale;

		c_tileset.emplace_back(temp);

		TS_LOADER.index = (c_tileset.size() - 1);
		TS_LOADER.count++;

		// set active texture properties
		update_current_tileset();

		tile_props[index].unique_id.reserve(tiles_present);
		tile_props[index].min.reserve(tiles_present);
		tile_props[index].max.reserve(tiles_present);

		// calculate the tile 's min-max points
		auto&[tsize_x, tsize_y] = c_tileset[index].tilesz;

		sf::Vector3i unique_set = { 0, 0, 0 };
		for (sf::Vector2f tmin{ 0.0f, 0.0f }, tmax{ (float)tsize_x, (float)tsize_y };
			unique_set.z < tiles_present;
			unique_set.z++, unique_set.x++, tmin.x += tsize_x, tmax.x += tsize_y)
		{
			if (!(unique_set.x < tiles_in_x))
			{
				tmin.x = 0; tmin.y += tsize_y;
				tmax.x = tsize_x; tmax.y += tsize_y;

				unique_set.y++;
				unique_set.x = 0;
			}

			tile_props[index].unique_id.push_back(sf::Vector3i
				{
					unique_set.x, unique_set.y, unique_set.z
				}
			);

			tile_props[index].min.push_back(tmin);
			tile_props[index].max.push_back(tmax);

		}

		if (TS_CURRENT.tsize.x >= TS_LOADER.target.getSize().x ||
			TS_CURRENT.tsize.y >= TS_LOADER.target.getSize().y)
		{
			Console::LogMessage("Improper tile size", ERROR);
		}

		return true;
	}

	return false;
}

void TilesetLoader::MovePointerR()
{
	if (c_tileset.empty()) return;

	c_tileset[index++].zoom_level = TS_CURRENT.scale;
	update_current_tileset();
}

void TilesetLoader::MovePointerL()
{
	if (c_tileset.empty()) return;

	c_tileset[index--].zoom_level = TS_CURRENT.scale;
	update_current_tileset();
}

// unique_set -> index and u_id
void live_properties::setTile(const int x, const int y)
{
	if (tile_props[TS_LOADER.index].unique_id.empty()) { return; }

	for (const auto& unique_set : tile_props[TS_LOADER.index].unique_id)
	{
		if (unique_set.x == x && unique_set.y == y)
			TS_CURRENT.u_id = unique_set.z;
	}
		
	TS_CURRENT.min += (
		tile_props[TS_LOADER.index].min[TS_CURRENT.u_id] * TS_CURRENT.scale
	);

	TS_CURRENT.max += (
		tile_props[TS_LOADER.index].max[TS_CURRENT.u_id] * TS_CURRENT.scale
	);
}

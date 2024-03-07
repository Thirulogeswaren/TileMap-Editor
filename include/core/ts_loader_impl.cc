#include "ts_loader.h"
#include "util/console.h"

struct tileset
{
	vector2u	m_size{};
	float		m_scale{};

	sf::Image	m_image{};
};

struct tile_properties {
	std::vector<vector3u> unique_id;
	std::vector<vector2u> min;
	std::vector<vector2u> max;
};


#include <unordered_map>

static std::vector<tileset> c_tileset;
static std::unordered_map<size_t, tile_properties> c_tileprops;

TilesetLoader::TilesetLoader() : target{}, target_state{ false }, index { 0 }, count{ 0 }
{
	Console::LogMessage("TS_LOADER active");
	c_tileset.reserve(30);
}

TilesetLoader::~TilesetLoader()
{
	c_tileset.clear();
}

void TilesetLoader::update_current_tileset()
{
	if (index >= c_tileset.size()) { index--; }

	if (index <= 0) { index = 0; }

	// set the current properties
	target_state = { target.loadFromImage(c_tileset[index].m_image) };

	TS_CURRENT.tilesize.x = c_tileset[index].m_size.x;
	TS_CURRENT.tilesize.y = c_tileset[index].m_size.y;
	TS_CURRENT.scale = c_tileset[index].m_scale;
	TS_CURRENT.min = TS_CURRENT.max = { 0, 0 };
	TS_CURRENT.u_id = 0;
}

bool TilesetLoader::LoadImage(std::string_view filepath, const vector2u& tilesize)
{
	if (tileset temp{}; temp.m_image.loadFromFile(filepath.data()))
	{
		temp.m_size.x = tilesize.x;
		temp.m_size.y = tilesize.y;

		temp.m_scale = 3.0f;

		auto tiles_in_x = temp.m_image.getSize().x / temp.m_size.x;
		auto tiles_in_y = temp.m_image.getSize().y / temp.m_size.y;

		uint16_t tiles_present = tiles_in_x * tiles_in_y;

		if (!c_tileset.empty())
			c_tileset[TS_LOADER.index].m_scale = TS_CURRENT.scale;

		c_tileset.emplace_back(temp);

		TS_LOADER.index = (c_tileset.size() - 1);
		TS_LOADER.count++;

		// set active texture properties
		update_current_tileset();

		c_tileprops[index].unique_id.reserve(tiles_present);
		c_tileprops[index].min.reserve(tiles_present);
		c_tileprops[index].max.reserve(tiles_present);

		// calculate the tile 's min-max points
		auto& [tx, ty] = c_tileset[index].m_size;

		vector3u unique_set{}; // where z -> u_id
		for (vector2u tmin{ 0, 0 }, tmax{ tx, ty };
			unique_set.z < tiles_present;
			unique_set.z++, unique_set.x++, tmin.x += tx, tmax.x += tx)
		{
			if (!(unique_set.x < tiles_in_x))
			{
				tmin.x = 0; tmin.y += ty;
				tmax.x = tx; tmax.y += ty;

				unique_set.y++;
				unique_set.x = 0;
			}

			c_tileprops[index].unique_id.push_back(vector3u
				{
					unique_set.x, unique_set.y, unique_set.z
				}
			);

			c_tileprops[index].min.push_back(tmin);
			c_tileprops[index].max.push_back(tmax);

		}

		if (TS_CURRENT.tilesize.x >= TS_LOADER.target.getSize().x ||
			TS_CURRENT.tilesize.y >= TS_LOADER.target.getSize().y)
		{
			Console::LogMessage("Improper tile size", ERROR);
		}

		return true;
	}

	return false;
}

void TilesetLoader::NextTileset()
{
	if (c_tileset.empty()) return;

	c_tileset[index++].m_scale = TS_CURRENT.scale;
	update_current_tileset();
}

void TilesetLoader::PrevTileset()
{
	if (c_tileset.empty()) return;

	c_tileset[index--].m_scale = TS_CURRENT.scale;
	update_current_tileset();
}

// unique_set -> index and u_id
void live_properties::HoveringTile(const int x, const int y)
{
	if (c_tileprops[TS_LOADER.index].unique_id.empty()) { return; }

	for (const auto& unique_set : c_tileprops[TS_LOADER.index].unique_id)
	{
		if (unique_set.x == x && unique_set.y == y)
			TS_CURRENT.u_id = unique_set.z;
	}
		
	TS_CURRENT.min.x += c_tileprops[TS_LOADER.index].min[TS_CURRENT.u_id].x * TS_CURRENT.scale;
	TS_CURRENT.min.y += c_tileprops[TS_LOADER.index].min[TS_CURRENT.u_id].y * TS_CURRENT.scale;

	TS_CURRENT.max.x += c_tileprops[TS_LOADER.index].max[TS_CURRENT.u_id].x * TS_CURRENT.scale;
	TS_CURRENT.max.y += c_tileprops[TS_LOADER.index].max[TS_CURRENT.u_id].y * TS_CURRENT.scale;
}

void live_properties::PickingTile()
{
	live_part.setTexture(TS_LOADER.target, true);
	live_part.setTextureRect(sf::IntRect
		{
			c_tileprops[TS_LOADER.index].min[TS_CURRENT.u_id].x,
			c_tileprops[TS_LOADER.index].min[TS_CURRENT.u_id].y,
			TS_CURRENT.tilesize.x,
			TS_CURRENT.tilesize.y
		}
	);
}

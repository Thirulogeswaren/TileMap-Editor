#include "ts_loader.h"
#include "util/console.h"

#include <unordered_map>

struct tileset
{
	vector2u	m_size{};
	vector2u	m_tiles_in{};
	float		m_scale{};

	sf::Image	m_image{};

	std::unordered_map<uint16_t, TilePoints> m_points{};
};

static std::vector<tileset> c_tileset;

sf::Image& TilesetLoader::GetTilesetImage()
{
	return c_tileset[index].m_image;
}

TilesetLoader::TilesetLoader() : target{}, target_state{ false }, index{ 0 }, count{ 0 }
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

		temp.m_tiles_in.x = temp.m_image.getSize().x / temp.m_size.x;
		temp.m_tiles_in.y = temp.m_image.getSize().y / temp.m_size.y;

		uint16_t tiles_present = temp.m_tiles_in.x * temp.m_tiles_in.y;

		if (!c_tileset.empty())
			c_tileset[TS_LOADER.index].m_scale = TS_CURRENT.scale;

		c_tileset.emplace_back(temp);

		TS_LOADER.index = (c_tileset.size() - 1);
		TS_LOADER.count++;

		// set active texture properties
		update_current_tileset();

		// calculate the tile 's min-max points
		auto& [tx, ty] = c_tileset[index].m_size;

		vector3u unique_set{}; // where z -> u_id
		for (vector2u tmin{ 0, 0 }, tmax{ tx, ty };
			unique_set.z < tiles_present;
			unique_set.z++, unique_set.x++, tmin.x += tx, tmax.x += tx)
		{
			if (!(unique_set.x < temp.m_tiles_in.x))
			{
				tmin.x = 0; tmin.y += ty;
				tmax.x = tx; tmax.y += ty;

				unique_set.y++;
				unique_set.x = 0;
			}

			c_tileset[index].m_points[unique_set.z].min = tmin;
			c_tileset[index].m_points[unique_set.z].max = tmax;
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
void live_properties::HoveringTile(const uint16_t x, const uint16_t y)
{
	static uint16_t active_id{};

	const auto& [tiles_in_x, tiles_in_y] = c_tileset[TS_LOADER.index].m_tiles_in;

	if (x >= tiles_in_x || y >= tiles_in_y)
	{
		active_id = 0u;
	}
	else if (active_id = x; y > 0) {
		active_id = x + (tiles_in_x * y);
	}

	TS_CURRENT.min.x += c_tileset[TS_LOADER.index].m_points[active_id].min.x * TS_CURRENT.scale;
	TS_CURRENT.min.y += c_tileset[TS_LOADER.index].m_points[active_id].min.y * TS_CURRENT.scale;

	TS_CURRENT.max.x += c_tileset[TS_LOADER.index].m_points[active_id].max.x * TS_CURRENT.scale;
	TS_CURRENT.max.y += c_tileset[TS_LOADER.index].m_points[active_id].max.y * TS_CURRENT.scale;
}

void live_properties::PickingTile(const uint16_t x, const uint16_t y)
{
	const auto& [tiles_in_x, tiles_in_y] = c_tileset[TS_LOADER.index].m_tiles_in;

	// sets the TS_CURRENT->u_id 
	if (x >= tiles_in_x || y >= tiles_in_y) {
		TS_CURRENT.u_id = 0u;
	}
	else if (TS_CURRENT.u_id = x; y > 0) {
		TS_CURRENT.u_id = x + (tiles_in_x * y);
	}


	TS_LOADER.to_draw.min = c_tileset[TS_LOADER.index].m_points[TS_CURRENT.u_id].min;
	TS_LOADER.to_draw.max = c_tileset[TS_LOADER.index].m_points[TS_CURRENT.u_id].max;

	live_part.setTexture(TS_LOADER.target, true);
	live_part.setTextureRect(sf::IntRect
		{
			c_tileset[TS_LOADER.index].m_points[TS_CURRENT.u_id].min.x,
			c_tileset[TS_LOADER.index].m_points[TS_CURRENT.u_id].min.y,
			TS_CURRENT.tilesize.x,
			TS_CURRENT.tilesize.y
		}
	);
}

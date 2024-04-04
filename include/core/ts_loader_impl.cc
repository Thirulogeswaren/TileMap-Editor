#include "ts_loader.h"
#include "util/console.h"

#include <unordered_map>

struct tileset
{
	vector2f	m_size{};
	vector2f	m_tiles_in{};
	float		m_scale{}; 

	sf::Image	m_image{};

	// contains min-max points of all the tiles
	std::unordered_map<uint16_t, upoints_t> m_points{};
};

static std::vector<tileset> c_tileset;

void TilesetLoader::update_current_tileset()
{
	if (TS_LOADER.m_index >= c_tileset.size()) { TS_LOADER.m_index--; }

	if (TS_LOADER.m_index <= 0) { TS_LOADER.m_index = 0; }

	// set the current properties
	if (TS_LOADER.m_tileset.loadFromImage(c_tileset[TS_LOADER.m_index].m_image))
	{
		TS_LOADER.m_state = 1;
	}

	TS_LOADER.m_dimension.x = TS_LOADER.m_tileset.getSize().x;
	TS_LOADER.m_dimension.y = TS_LOADER.m_tileset.getSize().y;
	TS_LOADER.m_tilesize.x = c_tileset[TS_LOADER.m_index].m_size.x;
	TS_LOADER.m_tilesize.y = c_tileset[TS_LOADER.m_index].m_size.y;

	TS_LOADER.scale_level = c_tileset[TS_LOADER.m_index].m_scale;

	TS_LOADER.rect_min = { 0u, 0u };
	TS_LOADER.rect_max = { 0u, 0u };

	// set referring tileset
	TS_LOADER.m_raw_tileset = c_tileset[TS_LOADER.m_index].m_image;
}

bool TilesetLoader::LoadImage(std::string_view filepath, const vector2u& tilesize)
{
	if (tileset temp{}; temp.m_image.loadFromFile(filepath.data()))
	{
		temp.m_size.x = tilesize.x;
		temp.m_size.y = tilesize.y;

		temp.m_tiles_in.x = static_cast<float>(temp.m_image.getSize().x) / temp.m_size.x;
		temp.m_tiles_in.y = static_cast<float>(temp.m_image.getSize().y) / temp.m_size.y;

		temp.m_scale = 3.0f;

		temp.m_tiles_in.x = std::roundf(temp.m_tiles_in.x);
		temp.m_tiles_in.y = std::roundf(temp.m_tiles_in.y);

		uint16_t tiles_present = temp.m_tiles_in.x * temp.m_tiles_in.y;

		if (!c_tileset.empty())
		{
			c_tileset[TS_LOADER.m_index].m_scale = TS_LOADER.scale_level;
		}

		c_tileset.emplace_back(temp);

		TS_LOADER.m_index = (c_tileset.size() - 1);
		TS_LOADER.m_count++;

		// update TS_LOADER
		update_current_tileset();

		// calculate the tile 's min-max points
		uint16_t tx = static_cast<uint16_t>(c_tileset[TS_LOADER.m_index].m_size.x);
		uint16_t ty = static_cast<uint16_t>(c_tileset[TS_LOADER.m_index].m_size.y);

		uint16_t unique_id{}; // where z -> u_id
		for (vector2u tmin{ 0u, 0u }, tmax{ tx, ty }, unique_set{}; unique_id< tiles_present;
			unique_id++, unique_set.x++, tmin.x += tx, tmax.x += tx)
		{
			if (!(unique_set.x < temp.m_tiles_in.x))
			{
				tmin.x = 0u; tmin.y += ty;
				tmax.x = tx; tmax.y += ty;

				unique_set.y++;
				unique_set.x = 0u;
			}

			c_tileset[TS_LOADER.m_index].m_points[unique_id].min = tmin;
			c_tileset[TS_LOADER.m_index].m_points[unique_id].max = tmax;
		}

		if (TS_LOADER.m_tilesize.x > TS_LOADER.m_tileset.getSize().x ||
			TS_LOADER.m_tilesize.y > TS_LOADER.m_tileset.getSize().y)
		{
			Console::LogMessage("tile_size larger than the tileset", ERROR);
		}

		return true;
	}

	return false;
}

void TilesetLoader::NextTileset()
{
	if ((!m_state) || c_tileset.empty()) return;

	c_tileset[TS_LOADER.m_index++].m_scale = TS_LOADER.scale_level;
	update_current_tileset();
}

void TilesetLoader::PrevTileset()
{
	if ((!m_state) || c_tileset.empty()) return;

	c_tileset[TS_LOADER.m_index--].m_scale = TS_LOADER.scale_level;
	update_current_tileset();
}

// unique_set -> index and u_id
void TilesetLoader::HoveredTile(const uint16_t x, const uint16_t y)
{
	static uint16_t active_id{};

	const auto& [tiles_in_x, tiles_in_y] = c_tileset[TS_LOADER.m_index].m_tiles_in;

	if (x >= tiles_in_x || y >= tiles_in_y)
	{
		active_id = 0u;
	}
	else if (active_id = x; y > 0) {
		active_id = x + (tiles_in_x * y);
	}

	rect_min.x += c_tileset[TS_LOADER.m_index].m_points[active_id].min.x * scale_level;
	rect_min.y += c_tileset[TS_LOADER.m_index].m_points[active_id].min.y * scale_level;

	rect_max.x += c_tileset[TS_LOADER.m_index].m_points[active_id].max.x * scale_level;
	rect_max.y += c_tileset[TS_LOADER.m_index].m_points[active_id].max.y * scale_level;
}

static inline void setScaleFactor(float tile_size, float scale_factor)
{
	if (TS_LOADER.m_tilesize.x == TS_LOADER.m_tilesize.y)
	{
		if (TS_LOADER.m_tilesize.x == tile_size)
			TS_LOADER.m_tile.setScale(scale_factor, scale_factor);
	}
}

void TilesetLoader::SelectTile(const uint16_t x, const uint16_t y)
{

	static uint16_t tile_uid{};
	const auto& [tiles_in_x, tiles_in_y] = c_tileset[TS_LOADER.m_index].m_tiles_in;

	// sets the TS_LOADER->u_id 
	if (x >= tiles_in_x || y >= tiles_in_y) {
		tile_uid = 0u;
	}
	else if (tile_uid = x; y > 0) {
		tile_uid = x + (tiles_in_x * y);
	}

	// set the min-max point of the tile you choose
	auto& [active_min, active_max] = TS_LOADER.m_points;

	// used later by umap_handler
	active_min = c_tileset[TS_LOADER.m_index].m_points[tile_uid].min;
	active_max = c_tileset[TS_LOADER.m_index].m_points[tile_uid].max;

	TS_LOADER.m_tile.setTexture(TS_LOADER.m_tileset, true);
	TS_LOADER.m_tile.setTextureRect(sf::IntRect
		{
			c_tileset[TS_LOADER.m_index].m_points[tile_uid].min.x,
			c_tileset[TS_LOADER.m_index].m_points[tile_uid].min.y,
			static_cast<uint16_t>(TS_LOADER.m_tilesize.x),
			static_cast<uint16_t>(TS_LOADER.m_tilesize.y)
		}
	);
	
	setScaleFactor(8.0f, 11.0f);
	setScaleFactor(16.0f, 6.0f);
	setScaleFactor(32.0f, 3.0f);

	TS_LOADER.m_tile.setOrigin(TS_LOADER.m_tile.getScale() / 2.0f);
}

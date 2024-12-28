#include <core/tileset-loader.h>
#include <util/console.h>

#include <unordered_map>
#include <vector>
#include <cmath>

struct upoints_t
{
	vector2u min, max;
};

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

using namespace core;

TilesetLoader::TilesetLoader() : r_texture{}, r_image{}
{
	m_state = 0;
	m_index = 0;
	m_count = 0;

	dimension = { 0.0f, 0.0f };
	tile_size = { 8.0f, 8.0f };

	tile_present = { 0.0f, 0.0f };

	min = max = { 0u, 0u };

	draw_scale = 5.0f;

	c_tileset.reserve(30);
}

TilesetLoader::~TilesetLoader()
{
	c_tileset.clear();
}

void TilesetLoader::update_tileset()
{
	if (this->m_index >= c_tileset.size()) { this->m_index--; }

	if (this->m_index <= 0) { this->m_index = 0; }

	// set the current properties
	if (this->r_texture.loadFromImage(c_tileset[this->m_index].m_image))
	{
		this->m_state = 100;
	}
	this->r_image = c_tileset[this->m_index].m_image;

	// tileset: image width and height
	this->dimension.x = c_tileset[this->m_index].m_image.getSize().x;
	this->dimension.y = c_tileset[this->m_index].m_image.getSize().y;

	// tileset: tile size
	this->tile_size.x = c_tileset[this->m_index].m_size.x;
	this->tile_size.y = c_tileset[this->m_index].m_size.y;

	this->draw_scale = c_tileset[this->m_index].m_scale;

}

bool TilesetLoader::LoadImage(std::string_view filepath, const uint16_t width, const uint16_t height)
{
	if (tileset temp{}; temp.m_image.loadFromFile(filepath.data()))
	{
		temp.m_size.x = width;
		temp.m_size.y = height;

		temp.m_tiles_in.x = static_cast<float>(temp.m_image.getSize().x) / temp.m_size.x;
		temp.m_tiles_in.y = static_cast<float>(temp.m_image.getSize().y) / temp.m_size.y;

		temp.m_scale = 3.0f;

		temp.m_tiles_in.x = std::roundf(temp.m_tiles_in.x);
		temp.m_tiles_in.y = std::roundf(temp.m_tiles_in.y);

		uint16_t tiles_present = temp.m_tiles_in.x * temp.m_tiles_in.y;

		if (!c_tileset.empty())
		{
			c_tileset[this->m_index].m_scale = this->draw_scale;
		}

		c_tileset.emplace_back(temp);

		this->m_index = (c_tileset.size() - 1);
		this->m_count++;

		// update TS_LOADER
		this->update_tileset();

		// calculate the tile 's min-max points
		uint16_t tx = static_cast<uint16_t>(c_tileset[this->m_index].m_size.x);
		uint16_t ty = static_cast<uint16_t>(c_tileset[this->m_index].m_size.y);

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

			c_tileset[this->m_index].m_points[unique_id].min = tmin;
			c_tileset[this->m_index].m_points[unique_id].max = tmax;
		}

		if (this->tile_size.x > c_tileset[this->m_index].m_image.getSize().x ||
			this->tile_size.y > c_tileset[this->m_index].m_image.getSize().y)
		{
			// LOG_ERROR("tile_size larger than the tileset");
		}

		return true;
	}

	return false;
}

void TilesetLoader::NextTileset()
{
	if ((!m_state) || c_tileset.empty()) return;

	c_tileset[this->m_index++].m_scale = this->draw_scale;
	this->update_tileset();
}

void TilesetLoader::PrevTileset()
{
	if ((!m_state) || c_tileset.empty()) return;

	c_tileset[this->m_index--].m_scale = this->draw_scale;
	this->update_tileset();
}

// unique_set -> index and u_id
/*void TilesetLoader::HoveredTile(const uint16_t x, const uint16_t y)
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
*/

void TilesetLoader::SelectTile(sf::Sprite& sprite, const uint16_t x, const uint16_t y)
{

	static uint16_t tile_uid{};
	const auto& [tiles_in_x, tiles_in_y] = c_tileset[this->m_index].m_tiles_in;

	// sets the TS_LOADER->u_id 
	if (x >= tiles_in_x || y >= tiles_in_y) {
		tile_uid = 0u;
	}
	else if (tile_uid = x; y > 0) {
		tile_uid = x + (tiles_in_x * y);
	}

	// set the min-max point of the tile you choose
	this->min = c_tileset[this->m_index].m_points[tile_uid].min;
	this->max = c_tileset[this->m_index].m_points[tile_uid].max;

	sprite.setTexture(this->r_texture, true);
	// sprite.setTextureRect(sf::IntRect
	// {
	// 	c_tileset[this->m_index].m_points[tile_uid].min.x,
	// 	c_tileset[this->m_index].m_points[tile_uid].min.y,
	// 	static_cast<uint16_t>(this->tile_size.x),
	// 	static_cast<uint16_t>(this->tile_size.y)
	// }
	// );
	//
	//setScaleFactor(8.0f, 11.0f);
	//setScaleFactor(16.0f, 6.0f);
	//setScaleFactor(32.0f, 3.0f);
	//
	//sprite.setScale(3.0f, 3.0f);
	sprite.setOrigin(this->tile_size / 2.0f);
}

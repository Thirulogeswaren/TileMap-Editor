#include "hbuffer.h"

struct tileset
{
	sf::Image		image{};

	sf::Vector2f	tile_size{};
	sf::Vector2u	num_of_tiles_in{};
	float			num_of_tiles{};
	float			zoom_level{};
};

#include <vector>

static std::vector<tileset> c_tileset;

bool hbuffer::LoadImage(std::string_view filepath)
{
	if (tileset temp; temp.image.loadFromFile(filepath.data())) 
	{
		temp.tile_size.x = current.tile_size.x;
		temp.tile_size.y = current.tile_size.y;
		temp.zoom_level = 1.0f;

		temp.num_of_tiles_in.x = temp.image.getSize().x / temp.tile_size.x;
		temp.num_of_tiles_in.y = temp.image.getSize().y / temp.tile_size.y;
		
		temp.num_of_tiles = temp.num_of_tiles_in.x * temp.num_of_tiles_in.y;

		c_tileset.emplace_back(temp);

		if (!c_tileset.empty()) // save before loading
			c_tileset[index].zoom_level = current.zoom_level;

		index = (c_tileset.size() - 1);
		count++;

		// set active texture properties
		current.target.loadFromImage(temp.image);

		current.tile_size.x = temp.tile_size.x;
		current.tile_size.y = temp.tile_size.y;
		current.zoom_level  = temp.zoom_level;
				
		return true;
	}

	if (c_tileset.empty()) return false;

	current.tile_size.x = c_tileset.at(index).tile_size.x;
	current.tile_size.y = c_tileset.at(index).tile_size.y;

	return false;
}

static void update_current_tileset()
{
	using namespace hbuffer;

	if (index >= c_tileset.size()) { index--; }

	if (index <= 0) { index = 0; }

	// set the current properties
	current.target.loadFromImage(c_tileset[index].image);

	current.tile_size.x = c_tileset[index].tile_size.x;
	current.tile_size.y = c_tileset[index].tile_size.y;
	current.zoom_level  = c_tileset[index].zoom_level;
}

void hbuffer::MovePointerR()
{
	if (c_tileset.empty()) return;

	c_tileset[index++].zoom_level = current.zoom_level;
	update_current_tileset();
}

void hbuffer::MovePointerL()
{
	if (c_tileset.empty()) return;

	c_tileset[index--].zoom_level = current.zoom_level;
	update_current_tileset();
}

// working with tiles

namespace
{

}



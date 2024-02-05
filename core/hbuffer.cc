#include "hbuffer.h"
#include "../util/console.h"

#include "SFML/Graphics/Image.hpp"

struct tileset 
{
	sf::Image image;
	sf::Vector2f tile_size;
	float zoom_level;
};

#include <vector>

static std::vector<tileset> c_tileset;

bool hbuffer::LoadImage(std::string_view filepath)
{
	if (tileset temp; temp.image.loadFromFile(filepath.data())) 
	{
		temp.tile_size = current.tile_size;
		temp.zoom_level = 1.0f;

		c_tileset.push_back(temp);

		current.index = (c_tileset.size() - 1);
		
		const auto& [image, tilesize, scale] = c_tileset[current.index];

		current.target.loadFromImage(image);
		current.tile_size = tilesize;
		current.zoom_level = scale;
		current.count++;

		Console::LogMessage("loaded ", filepath.data(), INFO);
		
		return true;
	}

	current.tile_size = c_tileset[current.index].tile_size;
	
	Console::LogMessage("[error] loading ", filepath.data(), ERROR);

	return false;
}

void hbuffer::TilesetProperties::UpdateCurrent()
{
	if (index >= c_tileset.size()) { index--; }

	if (index <= 0) { index = 0; }

	const auto& [image, tilesize, scale] = c_tileset[index];

	// set the current properties
	this->target.loadFromImage(image);
	this->tile_size = tilesize;
	this->zoom_level = scale;
}

void hbuffer::TilesetProperties::MovePointerR()
{
	if (c_tileset.empty()) return;

	c_tileset[index++].zoom_level = this->zoom_level;
	this->UpdateCurrent();
}

void hbuffer::TilesetProperties::MovePointerL()
{
	if (c_tileset.empty()) return;

	c_tileset[index--].zoom_level = this->zoom_level;
	this->UpdateCurrent();
}

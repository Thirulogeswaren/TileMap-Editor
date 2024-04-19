#include "umap_handler.h"

#include "util/console.h"

#define SOL_ALL_SAFETIES_ON 1
#include "sol/sol.hpp"

#include <fstream>

namespace 
{
	sf::Texture		actual_map{};
	sf::Image		raw_image{};
}

using namespace core;

MapHandler::MapHandler() : map{ actual_map }
{
	map_size = { 1u, 1u };
	map_tile_size = { 8u, 8u };
	
	map_dimension = { 0u, 0u };
	map_index = { 0u,0u };
	
	state = 0;

}

MapHandler::~MapHandler()
{

}

void MapHandler::CreateMap(const vector2u& tiles_in, const vector2u& tile_size)
{
	this->map_size = tiles_in;
	this->map_tile_size = tile_size;

	this->map_dimension.x = tiles_in.x * tile_size.x;
	this->map_dimension.y = tiles_in.y * tile_size.y;

	raw_image.create(map_dimension.x, map_dimension.y, sf::Color(17, 17, 17, 255));

	this->state = {
		actual_map.create(map_dimension.x, map_dimension.y)
	};
	
	actual_map.update(raw_image);
}

void MapHandler::UpdateMap(const sf::Image& tileset, const vector2u& min) const
{
	raw_image.copy(tileset,
		this->map_index.x * this->map_tile_size.x,
		this->map_index.y * this->map_tile_size.y,
		sf::IntRect{
			min.x, min.y,
			this->map_tile_size.x, this->map_tile_size.y
		},
		true
	);

	actual_map.update(raw_image);
}

void MapHandler::SaveMap(std::string_view filename) const
{
	std::string tmp_filename = std::string{ filename };

	if (filename.rfind('.') == std::string_view::npos) {
		tmp_filename.append(".png", 4);
	}
	
	raw_image.saveToFile(tmp_filename);

	LOG_NORMAL("saved %s", tmp_filename.data());
}

void MapHandler::SaveMapLua(std::string_view filename)
{
	sol::state lua;

	std::ofstream lua_file{ "example.lua" };

	lua.open_libraries(sol::lib::base);

	lua.script_file("example.lua");
}

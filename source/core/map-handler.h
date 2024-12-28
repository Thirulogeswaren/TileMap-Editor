#ifndef MAP_RESOURCE_CREATOR_NHANDLER_HH
#define MAP_RESOURCE_CREATOR_NHANDLER_HH

#include <core/common-types.h>

namespace core 
{
	struct MapHandler final {

		void CreateMap(const vector2u& tiles_in, const vector2u& size);

		void UpdateMap(const sf::Image& tileset, const vector2u& min) const;
		
		void SaveMap(std::string_view filename) const;

		void SaveMapXML(std::string_view filename);

		sf::Texture& map;
		
		vector2u map_size;
		vector2u map_tile_size;

		vector2u map_dimension;
		vector2u map_index;

		uint16_t state;

		MapHandler();
		~MapHandler();

	};
}
#endif

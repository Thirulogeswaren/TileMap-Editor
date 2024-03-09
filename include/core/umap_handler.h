#ifndef MAP_RESOURCE_CREATOR_NHANDLER_HH
#define MAP_RESOURCE_CREATOR_NHANDLER_HH

#include "custom_types.h"

#include "SFML/Graphics/RenderTexture.hpp"

inline struct map_handler {

	vector2u map_tilepresent{ 1u, 1u };
	vector2u map_tilesize{ 8u, 8u };

	vector2u map_dimension{};
	vector2u index{};

	bool map_state{};

	sf::Texture& GetFinalMap();
	
	void PlaceTile();
	
	void CreateMap(const vector2u& tiles_in, const vector2u& size);

	void SaveMap();


} map_properties{};

#define CURRENT_MAP map_properties

#endif

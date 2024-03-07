#ifndef MAP_RESOURCE_CREATOR_NHANDLER_HH
#define MAP_RESOURCE_CREATOR_NHANDLER_HH

#include "custom_types.h"

#include "SFML/Graphics/RenderTexture.hpp"

inline sf::RenderTexture umap;

inline struct map_handler
{
	vector2u map_tilepresent{};
	vector2u map_tilesize{ 8u, 8u };

	vector2f map_dimension{};
	vector2f index;

	void CreateMap(const vector2u& tiles_in, const vector2u& size);

	void PlaceTile();

} map_properties;

#define CURRENT_MAP map_properties

#endif

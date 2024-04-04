#ifndef MAP_RESOURCE_CREATOR_NHANDLER_HH
#define MAP_RESOURCE_CREATOR_NHANDLER_HH

#include "custom_types.h"

inline struct map_handler {

	vector2u map_tilepresent{ 1u, 1u };
	vector2u map_tilesize{ 8u, 8u };

	vector2u map_dimension{};
	vector2u map_index{};

	uint16_t map_state{};

	sf::Texture&	GetFinalMap() const;

	void PlaceTile();
	
	void CreateMap(const vector2u& tiles_in, const vector2u& size);

	void SaveMap(std::string_view filename);

} current_map_properties{};

#define CURRENT_MAP current_map_properties

#endif

#ifndef HIDDEN_TILESET_BUFFER_H
#define HIDDEN_TILESET_BUFFER_H

#include "SFML/Graphics/Texture.hpp"

namespace hbuffer
{
	bool LoadImage(std::string_view filepath);

	inline int index{ 0 }, count{ 0 };
	
	inline struct {
		sf::Texture target{ };
		sf::Vector2f tile_size{ 8.0f, 8.0f };
		float zoom_level{ 3.0f };
	} current;

	void MovePointerL();
	void MovePointerR();
};

#endif

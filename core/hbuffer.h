#ifndef HIDDEN_TILESET_BUFFER_H
#define HIDDEN_TILESET_BUFFER_H

#include "SFML/Graphics.hpp"

namespace hbuffer
{
	inline struct /* live contents */ {
		int index = 0;
		int capacity = 0;
		float scale = 1.0f;
		sf::Vector2f size = { 8.0f, 8.0f };
	} current;

	void loadImage(std::string_view filepath);

	void lTarget();
	void rTarget();
	sf::Texture& getTarget();

};

#endif

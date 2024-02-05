#ifndef HIDDEN_TILESET_BUFFER_H
#define HIDDEN_TILESET_BUFFER_H

#include "SFML/Graphics/Texture.hpp"
#include <string_view>

namespace hbuffer
{
	bool LoadImage(std::string_view filepath);

	class TilesetProperties {
	public:
		inline static int index = 0;
		inline static int count = 0;

		sf::Texture target;
		sf::Vector2f tile_size{ 8.0f, 8.0f };
		float zoom_level{ 3.0f };

		void MovePointerR();
		void MovePointerL();

	private:
		void UpdateCurrent();
	};

	inline TilesetProperties current;

};

#endif

#ifndef EDITOR_CORE_WINDOW_UI_H
#define EDITOR_CORE_WINDOW_UI_H

#include <core/ts_loader.h>
#include <core/umap_handler.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace ns_editor
{
	enum class EditorFlags : uint16_t
	{
		IN_ACTIVE = 0U,
		MAP_ENABLED,
		LOADER_ENABLED
	};

	class Editor final {
	public:
		void MenuBar();

		void Inspector();

		void Viewport();

		void Render(sf::RenderWindow& window);

		Editor(unsigned int width, unsigned int height);
		~Editor();

		// sf::Sprite tile;

	private:
		core::TilesetLoader loader;
		core::MapHandler map_handler;

	private:
		EditorFlags overlay;

		uint16_t isRunning{};
		uint16_t load_state{};
		uint16_t viewport_hovered{};
		
		//nfdchar_t* nfd_filepath;
	};

}

#endif

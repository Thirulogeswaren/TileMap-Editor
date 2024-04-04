#ifndef EDITOR_CORE_WINDOW_UI_H
#define EDITOR_CORE_WINDOW_UI_H

#include "nfd.h"

#include "SFML/Graphics/RenderWindow.hpp"

enum class EditorOverlay : uint8_t 
{
	IN_ACTIVE = 0U,
	MAP_ENABLED,
	LOADER_ENABLED
};

class Editor
{
public:
	void MenuBar();

	void Inspector();

	void Viewport();

	void Render();

	Editor();
	~Editor();

private:
	sf::RenderWindow* window_ref;
	EditorOverlay overlay;

	nfdchar_t* nfd_filepath;
	nfdresult_t nfd_result;

	uint8_t isRunning{};

	uint8_t viewport_hovered{};
};

#endif

#ifndef EDITORS_COMPONENT_UI__HH
#define EDITORS_COMPONENT_UI__HH

#include "imgui.h"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/View.hpp"

namespace window_ui
{
	namespace tileset
	{
		void Inspector();

		void TilePicker();
	
	}

	namespace world
	{
		void MenuBar();

		inline sf::View viewport{};

		inline sf::Sprite tile{};
	}

	namespace settings
	{
		inline bool import_enabled = false;

		inline bool map_enabled = false;

		constexpr ImGuiWindowFlags flags = {
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoCollapse
		};
	}
}

#endif

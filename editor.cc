#include "editor.h"

#include "imgui.h"
#include "imgui-SFML.h"

#include "SFML/Graphics.hpp"

struct Editor
{
	void GuiUpdates();

	Editor();
	~Editor();

	sf::RenderWindow window;
	sf::Clock delta_time;
};

void Editor::GuiUpdates()
{
	ImGui::Begin("Hello");

	ImGui::End();
}

Editor::Editor() : delta_time{} {

	window.create({ 1280, 720 }, "Editor", sf::Style::Default);
	window.setVerticalSyncEnabled(true);

	ImGui::SFML::Init(window);
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

}

Editor::~Editor()
{
	ImGui::SFML::Shutdown();
}

int main()
{
	Editor core; sf::Event evnt{};

	auto& [window, dt] = core;

	while (window.isOpen())
	{
		while (window.pollEvent(evnt)) 
		{
			ImGui::SFML::ProcessEvent(window, evnt);

			if (evnt.type == sf::Event::Closed)
				window.close();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				window.close();
		}

		ImGui::SFML::Update(window, dt.restart());

		core.GuiUpdates();

		
		// Render States
		window.clear(sf::Color::Black);

		ImGui::SFML::Render(window);
		window.display();
	}
}

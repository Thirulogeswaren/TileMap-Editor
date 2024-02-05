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
	using namespace settings;

	Console::Render();

	ImGui::Begin("Properties");
	ImGui::Text("Framerate: %.0f", ImGui::GetIO().Framerate);
	
	const auto& [x, y] = ImGui::GetIO().DisplaySize;
	ImGui::Text("Display Size: %.0f x %.0f", x, y);
	
	tileset::RenderUI();

	ImGui::End();
}

Editor::Editor() : delta_time{} {

	window.create({ 1600, 900 }, "Editor", sf::Style::Default);
	window.setVerticalSyncEnabled(true);

	ImGui::SFML::Init(window, false);
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui::GetIO().FontGlobalScale = 1.0f;

	ImGui::GetIO().Fonts->AddFontFromFileTTF("cousine.ttf", 20.0f);
	ImGui::SFML::UpdateFontTexture();
}

Editor::~Editor()
{
	ImGui::SFML::Shutdown();
}

#ifdef _WIN32_ENTRY__CC
int WinMain()
#else
int main()
#endif
{
	Editor core; sf::Event event{};

	auto& [window, dt] = core;

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(window, event);

			if (event.type == sf::Event::Closed)
				window.close();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				window.close();
		}

		ImGui::SFML::Update(window, dt.restart());

		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
		core.GuiUpdates();


		// Render States
		window.clear(sf::Color::Black);

		ImGui::SFML::Render(window);
		window.display();
	}
}

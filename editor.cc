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

using namespace window_ui;
sf::RectangleShape rshape{ sf::Vector2f{200.f,200.f } };

void Editor::GuiUpdates()
{
	Console::Render();

	ImGui::Begin("Inspector");

	tileset::Inspector();

	ImGui::End();

	world::MenuBar();
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

			if (event.type == sf::Event::Resized)
			{
				Console::LogMessage("Window Resized");
				sf::FloatRect nRect = {
					0.f, 0.f,
					ImGui::GetIO().DisplaySize.x,
					ImGui::GetIO().DisplaySize.y
				};
				window.setView(sf::View{ nRect });
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
				world::viewport.move(sf::Vector2f{ -200.0f, 0.0f });
				window.setView(world::viewport);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
				world::viewport.move(sf::Vector2f{ 200.0f, 0.0f });
				window.setView(world::viewport);
			}
		}

		ImGui::SFML::Update(window, dt.restart());

		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
		core.GuiUpdates();

		world::tile.setPosition(
			window.mapPixelToCoords(sf::Mouse::getPosition(window))
		);


		// Render States
		window.clear(sf::Color::Black);

		window.draw(world::tile);
		window.draw(rshape);

		ImGui::SFML::Render(window);
		window.display();
	}
}

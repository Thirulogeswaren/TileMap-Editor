#include "include/editor.h"

#include "imgui.h"
#include "imgui-SFML.h"

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Event.hpp"

#if defined(_WIN32) && (_MSC_VER)
#define entry_point WinMain
#else
#define entry_point main
#endif

struct Editor
{
	Editor();
	~Editor();

	sf::RenderWindow window;
	sf::Clock delta_time;
};

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

int entry_point()
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
		}

		ImGui::SFML::Update(window, dt.restart());

		ImGui::DockSpaceOverViewport(
			ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode
		);

		// ImGui Begin() && End() calls

		Console::Render();

		window::MenuBar();

		window::Inspector();
		
		window::Viewport();

		// Render States
		window.clear(sf::Color::Black);


		ImGui::SFML::Render(window);
		window.display();
	}

	return EXIT_SUCCESS;
}

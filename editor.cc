#include "include/editor.h"
#include "include/util/console.h"

#include "imgui.h"
#include "imgui-SFML.h"

#include "SFML/Window/Event.hpp"

#if defined(_WIN32) && (_MSC_VER)
#define entry_point main
#else
#define entry_point main
#endif

namespace {
	sf::RenderWindow window;

	ImVec4 Grey = ImVec4(0.24f, 0.24f, 0.24f, 1.0f);
	ImVec4 DarkGrey = ImVec4(0.07f, 0.07f, 0.07f, 1.0f);
	ImVec4 RedLike = ImVec4(0.86f, 0.47f, 0.47f, 1.0f);
}

Editor::Editor() :
	window_ref{ nullptr },
	overlay{ EditorOverlay::IN_ACTIVE },
	nfd_filepath{ nullptr },
	nfd_result{ NFD_ERROR }
{

	window.create({ 1600, 900 }, "Editor", sf::Style::Default);
	window.setVerticalSyncEnabled(true);

	ImGui::SFML::Init(window, false);
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	ImGui::GetIO().FontGlobalScale = 1.0f;

	ImGui::GetIO().Fonts->AddFontFromFileTTF("cousine.ttf", 20.0f);
	ImGui::SFML::UpdateFontTexture();

	window_ref = &window;

	auto& color = ImGui::GetStyle().Colors;

	color[ImGuiCol_WindowBg] = DarkGrey;

	color[ImGuiCol_TitleBg] = Grey;
	color[ImGuiCol_TitleBgActive] = Grey;

	color[ImGuiCol_TabActive] = DarkGrey;
	color[ImGuiCol_TabUnfocusedActive] = DarkGrey;

	color[ImGuiCol_TabHovered] = RedLike;

	color[ImGuiCol_FrameBg] = Grey;
	color[ImGuiCol_FrameBgHovered] = RedLike;
	color[ImGuiCol_FrameBgActive] = Grey;

	color[ImGuiCol_Button] = Grey;
	color[ImGuiCol_ButtonHovered] = RedLike;
	color[ImGuiCol_ButtonActive] = Grey;

	color[ImGuiCol_SliderGrab] = Grey;
	color[ImGuiCol_SliderGrabActive] = RedLike;

	color[ImGuiCol_ResizeGrip] = Grey;
	color[ImGuiCol_ResizeGripActive] = Grey;
	color[ImGuiCol_ResizeGripHovered] = RedLike;

	color[ImGuiCol_Header] = Grey;
	color[ImGuiCol_HeaderActive] = Grey;
	color[ImGuiCol_HeaderHovered] = RedLike;

	color[ImGuiCol_CheckMark] = RedLike;
	color[ImGuiCol_DockingPreview] = Grey;

}

Editor::~Editor()
{
	nfd_result = NFD_ERROR;
	nfd_filepath = nullptr;
	window_ref = nullptr;
	ImGui::SFML::Shutdown();
}

int entry_point()
{
	Editor core; sf::Event event{};

	sf::Clock delta_time{};

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
				sf::FloatRect new_viewport = {
					0.f, 0.f,
					ImGui::GetIO().DisplaySize.x,
					ImGui::GetIO().DisplaySize.y
				};
				window.setView(sf::View{ new_viewport });
			}
		}

		ImGui::SFML::Update(window, delta_time.restart());

		ImGui::DockSpaceOverViewport(
			ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode
		);

		// ImGui Begin() && End() calls
		
		Console::Render();

		core.MenuBar();

		core.Inspector();
		
		core.Viewport();

		// Render States
		window.clear(sf::Color::Black);

		ImGui::SFML::Render(window);
		core.Render();
		window.display();
	}

	return EXIT_SUCCESS;
}

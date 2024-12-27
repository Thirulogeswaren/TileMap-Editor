#include <editor/editor.h>
#include <util/console.h>

#include "imgui.h"
#include "imgui-SFML.h"

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Event.hpp"

#if defined(_WIN32) && (_MSC_VER)
#define entry_point main
#else
#define entry_point main
#endif

static sf::RenderWindow window;

static void setEditorStyle();

using namespace ns_editor;

Editor::Editor(unsigned int width, unsigned int height) :
	//tile			{ },
	loader			{ },
	map_handler		{ },
	overlay			{ EditorFlags::IN_ACTIVE }
	//nfd_filepath	{ nullptr }
{

	window.create(sf::VideoMode({ width, height }), "Editor", sf::Style::Close | sf::Style::Titlebar);
	window.setVerticalSyncEnabled(true);

	auto stat = ImGui::SFML::Init(window, false);

	// ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	// ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	ImGui::GetIO().FontGlobalScale = 1.0f;

	ImGui::GetIO().Fonts->AddFontFromFileTTF("cousine.ttf", 20.0f);
	stat = ImGui::SFML::UpdateFontTexture();

	setEditorStyle();
}

Editor::~Editor()
{
	//nfd_filepath = nullptr;
	ImGui::SFML::Shutdown();
}

int entry_point()
{
	Editor core{ 1600, 900 };

	sf::Clock delta_time{};

	while (window.isOpen())
	{
		while (std::optional event = window.pollEvent())
		{
			ImGui::SFML::ProcessEvent(window, event.value());

			if (event->getIf<sf::Event::Closed>() || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Escape))
				window.close();
			
			if (event->getIf<sf::Event::Resized>())
			{
				// LOG_NORMAL("Window Resized");
				// sf::FloatRect new_viewport = {
				// 	0.f, 0.f,
				// 	ImGui::GetIO().DisplaySize.x,
				// 	ImGui::GetIO().DisplaySize.y
				// };
				// window.setView(sf::View{ new_viewport });
			}
		}

		ImGui::SFML::Update(window, delta_time.restart());

		//ImGui::DockSpaceOverViewport(
		//	ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode
		//);

		// ImGui Begin() && End() calls
		
		Console::Render();

		core.MenuBar();

		core.Inspector();
		
		core.Viewport();

		// core.tile.setPosition(ImGui::GetMousePos());

		// Render States
		window.clear(sf::Color::Black);

		ImGui::SFML::Render(window);
		core.Render(window);
		window.display();
	}

	return EXIT_SUCCESS;
}

void setEditorStyle()
{
	constexpr ImVec4 Grey = ImVec4(0.24f, 0.24f, 0.24f, 1.0f);
	constexpr ImVec4 DarkGrey = ImVec4(0.07f, 0.07f, 0.07f, 1.0f);
	constexpr ImVec4 RedLike = ImVec4(0.86f, 0.47f, 0.47f, 1.0f);

	LOG_NORMAL("%.1f, %.1f, %.1f", 212.0f, 199.0f, 199.0f);

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
	// color[ImGuiCol_DockingPreview] = Grey;

}

#include "window_ui.h"
#include "imgui-SFML.h"

using namespace window_ui;

namespace
{
	sf::Vector2i map_size{ 10, 10 };

}

void world::MenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File")) {

			if (ImGui::MenuItem("Create Map"))
				settings::map_enabled = true;

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	if (settings::map_enabled)
	{
		ImGui::SetNextWindowFocus();
		
		ImGui::SetNextWindowPos(ImGui::GetWindowViewport()->GetCenter(), ImGuiCond_None, { 0.5,0.5 });
		
		ImGui::SetNextWindowSize({ 260.0f, 190.0f }, ImGuiCond_FirstUseEver);
		
		ImGui::Begin("Map Settings", nullptr, settings::flags);
		
		ImGui::AlignTextToFramePadding();
		ImGui::Text("map width  "); ImGui::SameLine();

		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		ImGui::InputInt("##1", &map_size.x, 0);

		ImGui::Spacing();

		ImGui::AlignTextToFramePadding();
		ImGui::Text("map height "); ImGui::SameLine();

		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		ImGui::InputInt("##2", &map_size.y, 0);

		ImGui::Spacing();
		ImGui::Spacing();

		if (ImGui::Button("Create Map")) {

			settings::map_enabled = false;
		}

		ImGui::End();
	}
}
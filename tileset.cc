#include "tileset.h"

#include "imgui.h"
#include "imgui-SFML.h"
#include "nfd.h"

#include "util/console.h"
#include "SFML/Graphics.hpp"

namespace {

	bool is_settings_open = false;
	nfdresult_t settings_result = NFD_ERROR;
	constexpr ImGuiWindowFlags settings_flags{
		ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoNavInputs |
		ImGuiWindowFlags_NoCollapse
	};

	// 8 pixels by default
	float tile_size[2]{ 8.0f, 8.0f };

	float scale{ 1.0f }; // zoom level

	std::string filename;

	ImVec2 canvas_min{ 1.0f, 1.0f }; // top-left 0 x 0
	ImVec2 canvas_max{ 1.0f, 1.0f }; // bottom-right 256 x 256
	ImVec2 canvas_size{ 1.0f, 1.0f }; // scale * 256
	ImVec2 grid_size{ 8.0f, 8.0f };

	ImVec2 mouse_position{ 0.0f, 0.0f };

	sf::Texture target;
	sf::Image image;
}

void Tileset::BeginEndUI()
{
	ImGui::SeparatorText("Tileset Properties");

	if (ImGui::Button("load texture"))
		is_settings_open = true;

	// active only when loading texture
	if (nfdchar_t* filepath = nullptr; is_settings_open)
	{
		ImGui::SetNextWindowFocus();
		ImGui::SetNextWindowPos(ImGui::GetWindowViewport()->GetCenter(), ImGuiCond_None, { 0.5,0.5 });
		ImGui::SetNextWindowSize({ 250.0f, 150.0f },ImGuiCond_FirstUseEver);
		ImGui::Begin("Import Settings", nullptr, settings_flags);
		ImGui::Text("tile width  "); ImGui::SameLine();
		ImGui::InputFloat("##1", &tile_size[0], 0.0F, 0.0F, "%.0f");
		ImGui::Text("tile height "); ImGui::SameLine();
		ImGui::InputFloat("##2", &tile_size[1], 0.0F, 0.0F, "%.0f");
		ImGui::Spacing();
		if (ImGui::Button("browse")) {
			if (NFD_OpenDialog("png", nullptr, &filepath) == NFD_OKAY)
			{
				filename = { filepath };
				settings_result = NFD_OKAY;
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("ok")) {
			is_settings_open = false;
			if (settings_result) {
				if (image.loadFromFile(filename)) {
					Console::LogMessage("loaded ", filename.c_str(), INFO);
				}
				else {
					Console::LogMessage("[error] ", filename.c_str(), ERROR);
				}
				target.loadFromImage(image);
				canvas_size.x = target.getSize().x * scale;
				canvas_size.y = target.getSize().y * scale;

				settings_result = NFD_ERROR;
			}
		}

		ImGui::End();
	} 


	ImGui::Text("tile width:  %.0f", tile_size[0]);
	ImGui::Text("tile height: %.0f", tile_size[1]);
	ImGui::Text("texture width:  %d", target.getSize().x);
	ImGui::Text("texture height: %d", target.getSize().y);

	ImGui::Spacing();

	if (ImGui::SliderFloat("##", &scale, 1.0f, 9.0f, "%.0f"))
	{
		canvas_size.x = target.getSize().x * scale;
		canvas_size.y = target.getSize().y * scale;
	}

	ImGui::Text("x-position: %.0f", mouse_position.x);
	ImGui::Text("y-position: %.0f", mouse_position.y);

	canvas_min = ImGui::GetCursorScreenPos();
	mouse_position.x = (ImGui::GetMousePos().x - canvas_min.x);
	mouse_position.y = (ImGui::GetMousePos().y - canvas_min.y);

	// irrespective of the zoom scale
	mouse_position.x /= scale;
	mouse_position.y /= scale;

	canvas_max = canvas_min;

	canvas_max.x += canvas_size.x;
	canvas_max.y += canvas_size.y;

	// grid size <lines>
	grid_size.x = tile_size[0] * scale;
	grid_size.y = tile_size[1] * scale;
	
	ImDrawList* dl = ImGui::GetWindowDrawList();

	dl->AddRectFilled(canvas_min, canvas_max, IM_COL32(60, 60, 60, 255));

	ImGui::Image(target, canvas_size);
	
	// define the point and draw them
	for (ImVec2 pt = canvas_min, pt2 = { canvas_max.x, canvas_min.y };
		pt.x <= canvas_max.x && pt.y <= canvas_max.y;
		pt.y += grid_size.y, pt2.y += grid_size.y)
	{
		dl->AddLine(pt, pt2, IM_COL32(0, 0, 0, 255), 3.0F);
	}
	for (ImVec2 pt = canvas_min, pt2 = { canvas_min.x, canvas_max.y };
		pt.x <= canvas_max.x && pt.y <= canvas_max.y;
		pt.x += grid_size.x, pt2.x += grid_size.x)
	{
		dl->AddLine(pt, pt2, IM_COL32(0, 0, 0, 255), 3.0F);
	}
}

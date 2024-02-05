#include "settings.h"

#include "hbuffer.h"

#include "imgui.h"
#include "imgui-SFML.h"

#include "nfd.h"

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

	std::string filename;

	ImVec2 canvas_min, canvas_max;
	ImVec2 canvas_size, grid_size;

	ImVec2 mouse_position;

	sf::Vector2f initial_size { 8.0f, 8.0f };
}

using namespace settings;

void tileset::RenderUI()
{

	auto& [target, tile_size, scale] = hbuffer::current;

	ImGui::Spacing();
	ImGui::SeparatorText("TileSet Properties");

	// active only when loading texture
	if (nfdchar_t* filepath = nullptr; is_settings_open)
	{
		ImGui::SetNextWindowFocus();
		ImGui::SetNextWindowPos(ImGui::GetWindowViewport()->GetCenter(), ImGuiCond_None, { 0.5,0.5 });
		ImGui::SetNextWindowSize({ 250.0f, 150.0f },ImGuiCond_FirstUseEver);
		ImGui::Begin("Import Settings", nullptr, settings_flags);
		ImGui::Text("tile width  "); ImGui::SameLine();
		ImGui::InputFloat("##1", &initial_size.x, 0.0F, 0.0F, "%.0f");
		ImGui::Text("tile height "); ImGui::SameLine();
		ImGui::InputFloat("##2", &initial_size.y, 0.0F, 0.0F, "%.0f");
		ImGui::Spacing();
		if (ImGui::Button("browse"))
		{
			if (NFD_OpenDialog(nullptr, nullptr, &filepath) == NFD_OKAY)
			{
				settings_result = NFD_OKAY;
				filename = { filepath };
			}
		}

		ImGui::SameLine();
		
		if (ImGui::Button("ok")) {
			if (settings_result == NFD_OKAY) {
				tile_size = initial_size;
				if (hbuffer::LoadImage(filename)) {
					canvas_size.x = target.getSize().x * scale;
					canvas_size.y = target.getSize().y * scale;
				}
			}
			is_settings_open = false;
			settings_result = NFD_ERROR;
		}

		ImGui::End();
	} 


	ImGui::Text("tile size:  %.0f x %0.f", tile_size.x, tile_size.y);
	ImGui::Text("dimensions: %d x %d", target.getSize().x, target.getSize().y);

	ImGui::Spacing();
	
	if (ImGui::Button("load texture"))
		is_settings_open = true;

	ImGui::Spacing();

	if (ImGui::SliderFloat("##", &scale, 1.0f, 9.0f, "%.1f"))
	{
		canvas_size.x = target.getSize().x * scale;
		canvas_size.y = target.getSize().y * scale;
	}

	ImGui::Spacing();

	if (ImGui::ArrowButton("##1", ImGuiDir_Left))
	{
		hbuffer::current.MovePointerL();
		canvas_size.x = target.getSize().x * scale;
		canvas_size.y = target.getSize().y * scale;
	} 
	ImGui::SameLine(); 
	ImGui::Text("Loaded TileSets");
	ImGui::SameLine();
	if (ImGui::ArrowButton("##2", ImGuiDir_Right))
	{
		hbuffer::current.MovePointerR();
		canvas_size.x = target.getSize().x * scale;
		canvas_size.y = target.getSize().y * scale;
	}
	ImGui::SameLine();
	ImGui::Text("loaded: %d [%d]", hbuffer::current.count, hbuffer::current.index);

	ImGui::Spacing();
	ImGui::BeginChild("Overview", ImVec2{}, ImGuiChildFlags_Border, ImGuiWindowFlags_HorizontalScrollbar);

	canvas_min = ImGui::GetCursorScreenPos();
	mouse_position.x = (ImGui::GetMousePos().x - canvas_min.x);
	mouse_position.y = (ImGui::GetMousePos().y - canvas_min.y);

	// irrespective of the zoom level
	// later used by hidden buffer
	mouse_position.x /= scale;
	mouse_position.y /= scale;

	canvas_max = canvas_min;

	canvas_max.x += canvas_size.x;
	canvas_max.y += canvas_size.y;

	// grid size <lines>
	grid_size.x = tile_size.x * scale;
	grid_size.y = tile_size.y * scale;
	
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

	ImGui::EndChild();
}

#include "settings.h"

#include "hbuffer.h"

#include "imgui.h"
#include "imgui-SFML.h"

#include "nfd.h"

namespace {

	bool is_settings_open = false;
	bool settings_toggle = true;
	nfdresult_t settings_result = NFD_ERROR;
	constexpr ImGuiWindowFlags settings_flags{
		ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoNavInputs |
		ImGuiWindowFlags_NoCollapse
	};

	std::string filename;

	sf::Vector2f initial_size{ 8.0f, 8.0f };

	// tileset canvas
	ImVec2 canvas_min, canvas_max;
	ImVec2 canvas_size, grid_size;

	ImVec2 mouse_position;

}

using namespace settings;

void tileset::RenderUI()
{
	auto& [target, tile_size, scale] = hbuffer::current;

	// active only when loading texture
	if (nfdchar_t* filepath = nullptr; is_settings_open)
	{
		ImGui::SetNextWindowFocus();
		ImGui::SetNextWindowPos(ImGui::GetWindowViewport()->GetCenter(), ImGuiCond_None, { 0.5,0.5 });
		ImGui::SetNextWindowSize({ 250.0f, 150.0f },ImGuiCond_FirstUseEver);
		ImGui::Begin("Import Settings", nullptr, settings_flags);
		ImGui::Text("tile width  "); ImGui::SameLine();

		if (ImGui::InputFloat("##1", &initial_size.x, 0.0F, 0.0F, "%.0f"))
			if (settings_toggle) { initial_size.y = initial_size.x; }
		
		ImGui::Text("tile height "); ImGui::SameLine();

		if (ImGui::InputFloat("##2", &initial_size.y, 0.0F, 0.0F, "%.0f"))
			if (settings_toggle) { initial_size.x = initial_size.y; }

		ImGui::Spacing();
		
		ImGui::Checkbox("##3", &settings_toggle);

		ImGui::SameLine();

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

	ImGui::Text("dimension: %d x %d", target.getSize().x, target.getSize().y);
	
	ImGui::Text("tile size: %.0f x %.0f", tile_size.x, tile_size.y);

	ImGui::Spacing();
	ImGui::Spacing();

	if (ImGui::Button("load texture"))
		is_settings_open = true;

	ImGui::SameLine();

	ImGui::Text("count: %d [inx: %d]", hbuffer::count, hbuffer::index);

	ImGui::Spacing();
	ImGui::Spacing();

	if (ImGui::ArrowButton("##1", ImGuiDir_Left))
	{
		hbuffer::MovePointerL();
		canvas_size.x = target.getSize().x * scale;
		canvas_size.y = target.getSize().y * scale;
	}
	ImGui::SameLine();
	if (ImGui::SliderFloat("##0", &scale, 1.0f, 9.0f, "%.1f"))
	{
		canvas_size.x = target.getSize().x * scale;
		canvas_size.y = target.getSize().y * scale;
	}
	ImGui::SameLine();
	if (ImGui::ArrowButton("##2", ImGuiDir_Right))
	{
		hbuffer::MovePointerR();
		canvas_size.x = target.getSize().x * scale;
		canvas_size.y = target.getSize().y * scale;
	}

	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::BeginChild("##", ImGui::GetContentRegionAvail(), ImGuiChildFlags_Border, ImGuiWindowFlags_HorizontalScrollbar);

	canvas_min = ImGui::GetCursorScreenPos();

	mouse_position.x = (ImGui::GetMousePos().x - canvas_min.x);
	mouse_position.y = (ImGui::GetMousePos().y - canvas_min.y);

	// irrespective of the zoom level later used by hidden buffer
	if (mouse_position.x > 0 && mouse_position.y > 0) {
		mouse_position.x /= scale;
		mouse_position.y /= scale;
	}
	else { mouse_position = { 0.0f, 0.0f }; }

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

	// above the image -> grids -> hover animation

	ImGui::EndChild();
}

static void HoverAnim()
{
	// calculate min and max points of individual tiles
	// see if mouse_position lies between any min and max points
	// MAP THOSE min/max points to IDs <given individually to every tile>
	// then again MAP THEM to the individual pixels itself

	// such that min/max <=> IDs <=> pixels(tiles)

}
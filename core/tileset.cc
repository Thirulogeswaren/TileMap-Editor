#include "window_ui.h"

#include "hbuffer.h"

#include "imgui.h"
#include "imgui-SFML.h"

#include "nfd.h"

namespace /* import settings */ {
	std::string load_filename = {};
	nfdresult_t load_state = NFD_ERROR;

	bool is_locked = true;
	bool is_active = false;
}

namespace {

	// tileset canvas
	ImVec2 canvas_min, canvas_max;
	ImVec2 canvas_size, grid_size;

	sf::Vector2i active_tile;
	
}

using namespace window_ui;

void tileset::Inspector()
{
	// active only when loading texture
	if (nfdchar_t* filepath = nullptr; is_active)
	{
		static sf::Vector2i initial_size = { 8, 8 };
		ImGui::SetNextWindowFocus();
		ImGui::SetNextWindowPos(ImGui::GetWindowViewport()->GetCenter(), ImGuiCond_None, { 0.5,0.5 });
		ImGui::SetNextWindowSize({ 250.0f, 150.0f }, ImGuiCond_FirstUseEver);
		ImGui::Begin("Import Settings", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
		ImGui::AlignTextToFramePadding();
		ImGui::Text("tile width "); ImGui::SameLine();

		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		if (ImGui::InputInt("##1", &initial_size.x, 0))
			if (is_locked) { initial_size.y = initial_size.x; }
		
		ImGui::Spacing();

		ImGui::AlignTextToFramePadding();
		ImGui::Text("tile height"); ImGui::SameLine();

		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		if (ImGui::InputInt("##2", &initial_size.y, 0))
			if (is_locked) { initial_size.x = initial_size.y; }

		ImGui::Spacing();
		
		ImGui::Checkbox("##3", &is_locked);

		ImGui::SameLine();

		if (ImGui::Button("browse"))
		{
			if (NFD_OpenDialog(nullptr, nullptr, &filepath) == NFD_OKAY)
			{
				load_state = NFD_OKAY;
				load_filename = { filepath };
			}
		}

		ImGui::SameLine();
		
		if (ImGui::Button("ok")) {
			if (load_state == NFD_OKAY) {
				if (TS_LOADER.LoadImage(load_filename, initial_size)) {
					canvas_size.x = TS_LOADER.target.getSize().x * TS_CURRENT.scale;
					canvas_size.y = TS_LOADER.target.getSize().y * TS_CURRENT.scale;
				}
			}
			is_active = false;
			load_state = NFD_ERROR;
		}

		ImGui::End();
	}

	ImGui::Spacing();
	ImGui::Spacing();

	// Property Begin()

	ImGui::Text("Dimension");
	ImGui::SameLine();

	ImGui::SameLine(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("x ####").x);
	ImGui::Text("x");
	ImGui::SameLine();
	ImGui::TextDisabled("%u", TS_LOADER.target.getSize().x);
		
	ImGui::SameLine(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("y ####").x);
	ImGui::Text("x");
	ImGui::SameLine();
	ImGui::TextDisabled("%u", TS_LOADER.target.getSize().y);

	//ImGui::Separator();
	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::Text("Tile Size");
	ImGui::SameLine();

	ImGui::SameLine(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("x ####").x);
	ImGui::Text("x");
	ImGui::SameLine();
	ImGui::TextDisabled("%u", TS_CURRENT.tsize.x);

	ImGui::SameLine(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("y ####").x);
	ImGui::Text("y"); ImGui::SameLine();
	ImGui::TextDisabled("%u", TS_CURRENT.tsize.y);

	ImGui::Spacing();
	ImGui::Spacing();

	if (ImGui::Button("load tileset"))
		is_active = true;

	ImGui::Spacing();
	ImGui::Spacing();

	if (ImGui::ArrowButton("##l", ImGuiDir_Left))
	{
		TS_LOADER.MovePointerL();
		canvas_size.x = TS_LOADER.target.getSize().x * TS_CURRENT.scale;
		canvas_size.y = TS_LOADER.target.getSize().y * TS_CURRENT.scale;
	}
	ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - 35.0f);
	if (ImGui::SliderFloat("##f", &TS_CURRENT.scale, 1.0f, 9.0f, "%.1f"))
	{
		canvas_size.x = TS_LOADER.target.getSize().x * TS_CURRENT.scale;
		canvas_size.y = TS_LOADER.target.getSize().y * TS_CURRENT.scale;
	}
	ImGui::SameLine();
	if (ImGui::ArrowButton("##r", ImGuiDir_Right))
	{
		TS_LOADER.MovePointerR();
		canvas_size.x = TS_LOADER.target.getSize().x * TS_CURRENT.scale;
		canvas_size.y = TS_LOADER.target.getSize().y * TS_CURRENT.scale;
	}

	ImGui::Spacing();
	ImGui::Spacing();

	// Tileset Canvas BeginChild()

	ImGui::BeginChild("##v", ImGui::GetContentRegionAvail(), ImGuiChildFlags_Border, ImGuiWindowFlags_HorizontalScrollbar);
	canvas_min = ImGui::GetCursorScreenPos();

	mouse_position.x = (ImGui::GetMousePos().x - canvas_min.x);
	mouse_position.y = (ImGui::GetMousePos().y - canvas_min.y);

	// irrespective of the zoom level later used by hidden buffer
	if (TS_LOADER.is_target_active && mouse_position.x > 0 && mouse_position.y > 0)
	{
		if (TS_CURRENT.scale >= 1.0f) {
			mouse_position.x /= TS_CURRENT.scale;
			mouse_position.y /= TS_CURRENT.scale;
		}

		// gives the index
		active_tile.x = mouse_position.x / TS_CURRENT.tsize.x;
		active_tile.y = mouse_position.y / TS_CURRENT.tsize.y;

	}
	else {
		TS_CURRENT.min = TS_CURRENT.max = { 0.0f, 0.0f };
		mouse_position = { 0, 0 };
		active_tile = { 0, 0 };
	}

	canvas_max = canvas_min;

	canvas_max.x += canvas_size.x;
	canvas_max.y += canvas_size.y;

	// grid size <lines>
	grid_size.x = TS_CURRENT.tsize.x * TS_CURRENT.scale;
	grid_size.y = TS_CURRENT.tsize.y * TS_CURRENT.scale;
	
	ImDrawList* dl = ImGui::GetWindowDrawList();

	dl->AddRectFilled(canvas_min, canvas_max, IM_COL32(60, 60, 60, 255));

	ImGui::Image(TS_LOADER.target, canvas_size);
	
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

	if (TS_LOADER.is_target_active && mouse_position.x > 0 && mouse_position.y > 0) 
	{
		TS_CURRENT.min = canvas_min; TS_CURRENT.max = canvas_min;
		TS_CURRENT.setTile(active_tile.x, active_tile.y);
		
		dl->AddRect(TS_CURRENT.min, TS_CURRENT.max, IM_COL32(255, 0, 0, 255), 0.0F, 0, 3.0F);
	}
	
	ImGui::EndChild();

}

#include "editor.h"

#include "imgui.h"
#include "imgui-SFML.h"

namespace 
{
	bool lock_enabled{ true };
	vector2u tilesize{};

	// tileset viewer
	vector2f canvas_min{}, canvas_max{};
	vector2f canvas_size{}, grid_size{};

	vector2f active_min{}, active_max{};

	vector2f mouse_position{};
	vector2u tile_index{};
}

using namespace ns_editor;

void Editor::Inspector()
{
	ImGui::Begin("Inspector", nullptr);

	// active only when loading texture
	if (overlay == EditorFlags::LOADER_ENABLED) {

		ImGui::SetNextWindowFocus();
		//ImGui::SetNextWindowPos(ImGui::GetWindowViewport()->GetCenter(), ImGuiCond_None, { 0.5,0.5 });
		ImGui::SetNextWindowSize({ 250.0f, 150.0f }, ImGuiCond_FirstUseEver);
		ImGui::Begin("Import Settings", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
		ImGui::AlignTextToFramePadding();
		ImGui::Text("tile width "); ImGui::SameLine();

		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		if (ImGui::InputScalar("##0", ImGuiDataType_U16, &tilesize.x))
			if (lock_enabled) { tilesize.y = tilesize.x; }

		ImGui::Spacing();

		ImGui::AlignTextToFramePadding();
		ImGui::Text("tile height"); ImGui::SameLine();

		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		if (ImGui::InputScalar("##1", ImGuiDataType_U16, &tilesize.y))
			if (lock_enabled) { tilesize.x = tilesize.y; }

		ImGui::Spacing();

		ImGui::Checkbox("##3", &lock_enabled);

		ImGui::SameLine();

		if (ImGui::Button("browse")) {
			//if (NFD_OpenDialog(nullptr, nullptr, &nfd_filepath) == NFD_OKAY)
			//	this->load_state = NFD_OKAY;
		}

		ImGui::SameLine();

		if (ImGui::Button("ok")) { 
			// if (this->load_state == NFD_OKAY)
			// {
			// 	if (loader.LoadImage(nfd_filepath, tilesize.x, tilesize.y)) {
			// 		canvas_size.x = this->loader.r_texture.getSize().x * this->loader.draw_scale;
			// 		canvas_size.y = this->loader.r_texture.getSize().y * this->loader.draw_scale;
			// 		loader.SelectTile(tile, 0u, 0u);
			// 	}
			// }
			overlay = EditorFlags::IN_ACTIVE;
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
	ImGui::TextDisabled("%.0f", loader.dimension.x);

	ImGui::SameLine(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("y ####").x);
	ImGui::Text("y");
	ImGui::SameLine();
	ImGui::TextDisabled("%.0f", loader.dimension.y);

	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::Text("Tile Size");
	ImGui::SameLine();

	ImGui::SameLine(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("x ####").x);
	ImGui::Text("x");
	ImGui::SameLine();
	ImGui::TextDisabled("%0.f", loader.tile_size.x);

	ImGui::SameLine(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("y ####").x);
	ImGui::Text("y"); ImGui::SameLine();
	ImGui::TextDisabled("%0.f", loader.tile_size.y);

	ImGui::Spacing();
	ImGui::Spacing();

	if (ImGui::Button("load tileset"))
		overlay = EditorFlags::LOADER_ENABLED;

	ImGui::Spacing();
	ImGui::Spacing();

	if (ImGui::ArrowButton("##l", ImGuiDir_Left))
	{
		loader.PrevTileset();
		canvas_size.x = loader.dimension.x * loader.draw_scale;
		canvas_size.y = loader.dimension.y * loader.draw_scale;
	}
	ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - 35.0f);
	if (ImGui::SliderFloat("##f", &loader.draw_scale, 1.0f, 9.0f, "%.1f"))
	{
		canvas_size.x = loader.dimension.x * loader.draw_scale;
		canvas_size.y = loader.dimension.y * loader.draw_scale;
	}
	ImGui::SameLine();
	if (ImGui::ArrowButton("##r", ImGuiDir_Right))
	{
		loader.NextTileset();
		canvas_size.x = loader.dimension.x * loader.draw_scale;
		canvas_size.y = loader.dimension.y * loader.draw_scale;
	}

	ImGui::Spacing();
	ImGui::Spacing();

	// Tileset Canvas BeginChild()

	ImGui::BeginChild("##v", ImGui::GetContentRegionAvail(), ImGuiChildFlags_Border, ImGuiWindowFlags_HorizontalScrollbar);

	canvas_min = { ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y };

	mouse_position.x = (ImGui::GetMousePos().x - canvas_min.x);
	mouse_position.y = (ImGui::GetMousePos().y - canvas_min.y);

	// irrespective of the zoom level later used by hidden buffer
	if (loader.m_state && ImGui::IsWindowHovered())
	{
		if (loader.draw_scale >= 1.0f) {
			mouse_position.x /= loader.draw_scale;
			mouse_position.y /= loader.draw_scale;
		}

		// gives the index
		tile_index.x = mouse_position.x / loader.tile_size.x;
		tile_index.y = mouse_position.y / loader.tile_size.y;

		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		{
			// loader.SelectTile(tile, tile_index.x, tile_index.y);
		}
	}
	else {
		mouse_position = { 0.0f, 0.0f };
		tile_index = { 0, 0 };
	}

	canvas_max = canvas_min;
	active_min = active_max = canvas_min;

	canvas_max.x += canvas_size.x;
	canvas_max.y += canvas_size.y;

	// grid size <lines>
	grid_size.x = loader.tile_size.x * loader.draw_scale;
	grid_size.y = loader.tile_size.y * loader.draw_scale;

	if (loader.m_state) // draw loaded tileset
	{
		ImGui::Image(loader.r_texture, canvas_size);
	}

	static ImDrawList* dl = ImGui::GetWindowDrawList();

	// define the point and draw them
	for (vector2f pt{ canvas_min }, pt2{ canvas_max.x, canvas_min.y };
		pt.y <= canvas_max.y && pt2.y <= canvas_max.y;
		pt.y += grid_size.y, pt2.y += grid_size.y)
	{
		dl->AddLine(pt, pt2, IM_COL32(0, 0, 0, 255), 3.0f);
	}

	for (vector2f pt{ canvas_min }, pt2{ canvas_min.x, canvas_max.y };
		pt.x <= canvas_max.x && pt2.x <= canvas_max.x;
		pt.x += grid_size.x, pt2.x += grid_size.x)
	{
		dl->AddLine(pt, pt2, IM_COL32(0, 0, 0, 255), 3.0f);
	}

	active_min.x += this->loader.min.x * this->loader.draw_scale;
	active_min.y += this->loader.min.y * this->loader.draw_scale;

	active_max.x += this->loader.max.x * this->loader.draw_scale;
	active_max.y += this->loader.max.y * this->loader.draw_scale;

	if (loader.m_state)
		dl->AddRect(active_min, active_max, IM_COL32(212, 199, 199, 255), 1.0F, 0, 3.0F);

	ImGui::EndChild();

	ImGui::End();
}

#include "editor.h"

#include "core/ts_loader.h"

#include "imgui.h"
#include "imgui-SFML.h"

#include "nfd.h"

namespace 
{
	// import settings
	nfdchar_t* nfd_filepath = nullptr;
	nfdresult_t nfd_result = NFD_ERROR;

	bool import_enabled = false;
	bool lock_enabled = true;


	// tileset viewer
	vector2f canvas_min{}, canvas_max{};
	vector2f canvas_size{}, grid_size{};

	vector2f mouse_position{};
	vector2u tile_index{};
}

void window::Inspector()
{
	ImGui::Begin("Inspector", nullptr);

	// active only when loading texture
	if (static vector2u tilesize{}; import_enabled) {
		
		ImGui::SetNextWindowFocus();
		ImGui::SetNextWindowPos(ImGui::GetWindowViewport()->GetCenter(), ImGuiCond_None, { 0.5,0.5 });
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
			if (NFD_OpenDialog(nullptr, nullptr, &nfd_filepath) == NFD_OKAY)
				nfd_result = NFD_OKAY;
		}

		ImGui::SameLine();
		
		if (ImGui::Button("ok")) {
			if (nfd_result == NFD_OKAY) {
				if (TS_LOADER.LoadImage(nfd_filepath, tilesize)) {
					canvas_size.x = TS_LOADER.target.getSize().x * TS_CURRENT.scale;
					canvas_size.y = TS_LOADER.target.getSize().y * TS_CURRENT.scale;
				}
			}
			import_enabled = false;
			nfd_result = NFD_ERROR;
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
	ImGui::TextDisabled("%u", TS_CURRENT.tilesize.x);

	ImGui::SameLine(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("y ####").x);
	ImGui::Text("y"); ImGui::SameLine();
	ImGui::TextDisabled("%u", TS_CURRENT.tilesize.y);

	ImGui::Spacing();
	ImGui::Spacing();

	if (ImGui::Button("load tileset"))
		import_enabled = true;

	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::Text("Tile Index");
	ImGui::SameLine();

	ImGui::SameLine(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("x ####").x);
	ImGui::Text("x");
	ImGui::SameLine();
	ImGui::TextDisabled("%u", tile_index.x);

	ImGui::SameLine(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("y ####").x);
	ImGui::Text("y"); ImGui::SameLine();
	ImGui::TextDisabled("%u", tile_index.y);

	ImGui::Spacing();
	ImGui::Spacing();

	if (ImGui::ArrowButton("##l", ImGuiDir_Left))
	{
		TS_LOADER.PrevTileset();
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
		TS_LOADER.NextTileset();
		canvas_size.x = TS_LOADER.target.getSize().x * TS_CURRENT.scale;
		canvas_size.y = TS_LOADER.target.getSize().y * TS_CURRENT.scale;
	}

	ImGui::Spacing();
	ImGui::Spacing();

	// Tileset Canvas BeginChild()

	ImGui::BeginChild("##v", ImGui::GetContentRegionAvail(), ImGuiChildFlags_Border, ImGuiWindowFlags_HorizontalScrollbar);
	
	canvas_min = { ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y };

	mouse_position.x = (ImGui::GetMousePos().x - canvas_min.x);
	mouse_position.y = (ImGui::GetMousePos().y - canvas_min.y);

	// irrespective of the zoom level later used by hidden buffer
	if (TS_LOADER.target_state && mouse_position.x > 0 && mouse_position.y > 0)
	{
		if (TS_CURRENT.scale >= 1.0f) {
			mouse_position.x /= TS_CURRENT.scale;
			mouse_position.y /= TS_CURRENT.scale;
		}

		// gives the index
		tile_index.x = mouse_position.x / TS_CURRENT.tilesize.x;
		tile_index.y = mouse_position.y / TS_CURRENT.tilesize.y;

		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
			TS_CURRENT.PickingTile(tile_index.x, tile_index.y);
		}
	}
	else {
		TS_CURRENT.min = TS_CURRENT.max = { 0u, 0u };
		mouse_position = { 0.0f, 0.0f };
		tile_index = { 0, 0 };
	}

	canvas_max = canvas_min;

	canvas_max.x += canvas_size.x;
	canvas_max.y += canvas_size.y;

	// grid size <lines>
	grid_size.x = TS_CURRENT.tilesize.x * TS_CURRENT.scale;
	grid_size.y = TS_CURRENT.tilesize.y * TS_CURRENT.scale;
	
	ImGui::Image(TS_LOADER.target, canvas_size);
	
	// define the point and draw them
	for (vector2f pt{ canvas_min }, pt2{ canvas_max.x, canvas_min.y };
		pt.y <= canvas_max.y && pt2.y <= canvas_max.y;
		pt.y += grid_size.y, pt2.y += grid_size.y)
	{
		ImGui::GetWindowDrawList()->AddLine(
			{ pt.x, pt.y }, { pt2.x,pt2.y }, IM_COL32(0, 0, 0, 255), 3.0f
		);
	}

	for (vector2f pt{ canvas_min }, pt2{ canvas_min.x, canvas_max.y };
		pt.x <= canvas_max.x && pt2.x <= canvas_max.x;
		pt.x += grid_size.x, pt2.x += grid_size.x)
	{
		ImGui::GetWindowDrawList()->AddLine(
			{ pt.x, pt.y }, { pt2.x,pt2.y }, IM_COL32(0, 0, 0, 255), 3.0f
		);
	}
	

	if (TS_LOADER.target_state && mouse_position.x > 0 && mouse_position.y > 0) 
	{
		TS_CURRENT.min = canvas_min; TS_CURRENT.max = canvas_min;
		TS_CURRENT.HoveringTile(tile_index.x, tile_index.y);

		ImGui::GetWindowDrawList()->AddRect(
			{ TS_CURRENT.min.x, TS_CURRENT.min.y }, { TS_CURRENT.max.x,  TS_CURRENT.max.y },
			IM_COL32(255, 0, 0, 255), 0.0F, 0, 3.0F
		);
	}
	
	ImGui::EndChild();

	ImGui::End();
}

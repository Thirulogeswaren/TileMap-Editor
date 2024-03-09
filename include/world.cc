#include "editor.h"

#include "core/ts_loader.h"
#include "core/umap_handler.h"

#include "imgui.h"
#include "imgui-SFML.h"

namespace
{
	constexpr float drag_speed{ 9.0f };
	constexpr float scale_strength{ 2.0f };

	float camera_zoom = { 4.0f };
	bool map_enabled{};

	vector2f cursor_max{};
	vector2f grid_size{};

	vector2f mouse_position{};
}

void window::MenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File")) {

			if (ImGui::MenuItem("Create Map"))
				map_enabled = true;

			if (ImGui::MenuItem("Save as Image"))
				CURRENT_MAP.SaveMap();

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	if (static vector2u tiles_in{ 3u, 3u }, tile_size{ TS_CURRENT.tilesize }; map_enabled)
	{
		ImGui::SetNextWindowFocus();

		ImGui::SetNextWindowPos(ImGui::GetWindowViewport()->GetCenter(), ImGuiCond_None, { 0.5, 0.5 });

		ImGui::SetNextWindowSize({ 390.0f, 190.0f });

		ImGui::Begin("Map Settings", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
		
		ImGui::Spacing();

		if (ImGui::BeginTable("##properties", 3, ImGuiTableFlags_None))
		{
			ImGui::TableNextColumn(); // Row 1
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Tiles in");

			ImGui::TableNextColumn();
			ImGui::Text("x "); ImGui::SameLine();
			ImGui::InputScalar("##0", ImGuiDataType_U16, &tiles_in.x);

			ImGui::TableNextColumn();
			ImGui::Text("y "); ImGui::SameLine();
			ImGui::InputScalar("##1", ImGuiDataType_U16, &tiles_in.y);

			ImGui::TableNextRow(); // Row 2

			ImGui::TableNextColumn();
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Tile size");

			ImGui::TableNextColumn();
			ImGui::Text("x "); ImGui::SameLine();
			ImGui::InputScalar("##2", ImGuiDataType_U16, &tile_size.x);

			ImGui::TableNextColumn();
			ImGui::Text("y "); ImGui::SameLine();
			ImGui::InputScalar("##3", ImGuiDataType_U16, &tile_size.y);

			ImGui::EndTable();
		}

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Text("Dimensions: %u x %u", tiles_in.x * tile_size.x, tiles_in.y * tile_size.y);

		ImGui::Spacing();
		ImGui::Spacing();
		
		if (ImGui::Button("Create Map")) {
			CURRENT_MAP.CreateMap(tiles_in, tile_size);
			map_enabled = false;
		}

		ImGui::End();

	}
}

void window::Viewport()
{
	ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);
	static uint16_t uid{};
	ImGui::TextDisabled(" Use W, A, S, D to move");
	ImGui::TextDisabled("Position: %.1f x %.1f", mouse_position.x, mouse_position.y);

	static vector2f cursor_min = {
		ImGui::GetContentRegionAvail().x / 2.0f,
		ImGui::GetContentRegionAvail().y / 2.0f
	};

	ImGui::SetCursorScreenPos({ cursor_min.x, cursor_min.y });

	mouse_position = {
		(ImGui::GetMousePos().x - cursor_min.x),
		(ImGui::GetMousePos().y - cursor_min.y)
	};
	
	cursor_max = {
		cursor_min.x + (CURRENT_MAP.map_dimension.x * camera_zoom),
		cursor_min.y + (CURRENT_MAP.map_dimension.y * camera_zoom)
	};

	grid_size = {
		CURRENT_MAP.map_tilesize.x * camera_zoom,
		CURRENT_MAP.map_tilesize.y * camera_zoom
	};

	if (CURRENT_MAP.map_state) {
		ImGui::Image(CURRENT_MAP.GetFinalMap(), {
				CURRENT_MAP.map_dimension.x * camera_zoom,
				CURRENT_MAP.map_dimension.y * camera_zoom
			}
		);
	}

	// map grid
	for (vector2f pt{ cursor_min }, pt2{ cursor_max.x, cursor_min.y };
		pt.y <= cursor_max.y && pt2.y <= cursor_max.y;
		pt.y += grid_size.y, pt2.y += grid_size.y)
	{
		ImGui::GetWindowDrawList()->AddLine(
			{ pt.x, pt.y }, { pt2.x, pt2.y }, IM_COL32(255, 255, 255, 255)
		);
	}

	for (vector2f pt{ cursor_min }, pt2{ cursor_min.x, cursor_max.y };
		pt.x <= cursor_max.x && pt2.x <= cursor_max.x;
		pt.x += grid_size.x, pt2.x += grid_size.x)
	{
		ImGui::GetWindowDrawList()->AddLine(
			{ pt.x, pt.y }, { pt2.x, pt2.y }, IM_COL32(255, 255, 255, 255)
		);
	}

	if (ImGui::IsKeyDown(ImGuiKey_W)) { cursor_min.y += drag_speed; }
	if (ImGui::IsKeyDown(ImGuiKey_A)) { cursor_min.x += drag_speed; }
	if (ImGui::IsKeyDown(ImGuiKey_S)) { cursor_min.y -= drag_speed; }
	if (ImGui::IsKeyDown(ImGuiKey_D)) { cursor_min.x -= drag_speed; }

	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left, true)) {
		if (ImGui::IsMouseHoveringRect
		(
			{ cursor_min.x, cursor_min.y },
			{ cursor_max.x, cursor_max.y })
		)
		{
			CURRENT_MAP.PlaceTile();
		}
	}

	if (ImGui::IsWindowHovered())
	{
		if (ImGui::GetIO().MouseWheel > 0.0f)
		{
			camera_zoom += scale_strength;
		}
		if (ImGui::GetIO().MouseWheel < 0.0f && camera_zoom > scale_strength)
		{
			camera_zoom -= scale_strength;
		}

		if (mouse_position.x >= 1.0f && mouse_position.y >= 1.0f)
		{
			mouse_position.x /= camera_zoom;
			mouse_position.y /= camera_zoom;

			CURRENT_MAP.index.x = mouse_position.x / CURRENT_MAP.map_tilesize.x;
			CURRENT_MAP.index.y = mouse_position.y / CURRENT_MAP.map_tilesize.y;
		}

		ImGui::SetCursorPosX(ImGui::GetMousePos().x);
		ImGui::SetCursorPosY(ImGui::GetMousePos().y);

		ImGui::Image(TS_CURRENT.live_part, sf::Vector2f{ TS_CURRENT.tilesize.x * camera_zoom, TS_CURRENT.tilesize.y * camera_zoom });
	}
	
	ImGui::End();
}

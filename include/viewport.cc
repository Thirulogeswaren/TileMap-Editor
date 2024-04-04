#include "editor.h"

#include "core/ts_loader.h"
#include "core/umap_handler.h"

#include "imgui.h"
#include "imgui-SFML.h"

#include "SFML/Window/Mouse.hpp"

namespace
{
	constexpr float drag_speed{ 9.0f };
	constexpr float scale_strength{ 1.0f };

	float camera_zoom = { 4.0f };

	vector2f cursor_max{};
	vector2f grid_size{};

	vector2f mouse_position{};

}

void Editor::Viewport()
{
	ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);
	
	ImGui::TextDisabled("index: %u - %u", CURRENT_MAP.map_index.x, CURRENT_MAP.map_index.y);

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

	if (ImGui::IsWindowHovered())
	{
		viewport_hovered = 1;
		if (ImGui::GetIO().MouseWheel > 0.0f)
		{
			camera_zoom += scale_strength;
			cursor_min.x -= (CURRENT_MAP.map_dimension.x / 2u);
			cursor_min.y -= (CURRENT_MAP.map_dimension.y / 2u);
		}
		if (ImGui::GetIO().MouseWheel < 0.0f && camera_zoom > scale_strength)
		{
			camera_zoom -= scale_strength;
			cursor_min.x += (CURRENT_MAP.map_dimension.x / 2u);
			cursor_min.y += (CURRENT_MAP.map_dimension.y / 2u);
		}

		if (CURRENT_MAP.map_state && TS_LOADER.m_state && mouse_position.x >= 1.0f && mouse_position.y >= 1.0f)
		{
			mouse_position.x /= camera_zoom;
			mouse_position.y /= camera_zoom;

			CURRENT_MAP.map_index.x = mouse_position.x / CURRENT_MAP.map_tilesize.x;
			CURRENT_MAP.map_index.y = mouse_position.y / CURRENT_MAP.map_tilesize.y;

			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left, true)) {
				if (CURRENT_MAP.map_index.x < CURRENT_MAP.map_tilepresent.x &&
					CURRENT_MAP.map_index.y < CURRENT_MAP.map_tilepresent.y)
				{
					CURRENT_MAP.PlaceTile();
				}
			}
		}
	}
	else
	{
		viewport_hovered = 0u;
	}
	
	ImGui::End();
}

void Editor::Render()
{
	if (viewport_hovered)
	{
		// draw the selected tile
		TS_LOADER.m_tile.setPosition(ImGui::GetMousePos());
		window_ref->draw(TS_LOADER.m_tile);
	}
}
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

using namespace ns_editor;

#include "util/console.h"

void Editor::Viewport()
{
	ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);

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
		cursor_min.x + (this->map_handler.map_dimension.x * camera_zoom),
		cursor_min.y + (this->map_handler.map_dimension.y * camera_zoom)
	};

	grid_size = {
		this->map_handler.map_tile_size.x * camera_zoom,
		this->map_handler.map_tile_size.y * camera_zoom
	};

	if (map_handler.state) {
		ImGui::Image(this->map_handler.map, {
				this->map_handler.map_dimension.x * camera_zoom,
				this->map_handler.map_dimension.y * camera_zoom
			}
		);
	}

	// map grid
	for (vector2f pt{ cursor_min }, pt2{ cursor_max.x, cursor_min.y };
		pt.y <= cursor_max.y && pt2.y <= cursor_max.y;
		pt.y += grid_size.y, pt2.y += grid_size.y)
	{
		ImGui::GetWindowDrawList()->AddLine(pt, pt2, IM_COL32(255, 255, 255, 255));
	}

	for (vector2f pt{ cursor_min }, pt2{ cursor_min.x, cursor_max.y };
		pt.x <= cursor_max.x && pt2.x <= cursor_max.x;
		pt.x += grid_size.x, pt2.x += grid_size.x)
	{
		ImGui::GetWindowDrawList()->AddLine(pt, pt2, IM_COL32(255, 255, 255, 255));
	}

	if (ImGui::IsKeyDown(ImGuiKey_W)) { cursor_min.y += drag_speed; }
	if (ImGui::IsKeyDown(ImGuiKey_A)) { cursor_min.x += drag_speed; }
	if (ImGui::IsKeyDown(ImGuiKey_S)) { cursor_min.y -= drag_speed; }
	if (ImGui::IsKeyDown(ImGuiKey_D)) { cursor_min.x -= drag_speed; }

	if (ImGui::IsWindowHovered())
	{
		viewport_hovered = 100u;
		if (ImGui::GetIO().MouseWheel > 0.0f)
		{
			camera_zoom += scale_strength;
			cursor_min.x -= (this->map_handler.map_dimension.x / 2u);
			cursor_min.y -= (this->map_handler.map_dimension.y / 2u);
		}
		if (ImGui::GetIO().MouseWheel < 0.0f && camera_zoom > scale_strength)
		{
			camera_zoom -= scale_strength;
			cursor_min.x += (this->map_handler.map_dimension.x / 2u);
			cursor_min.y += (this->map_handler.map_dimension.y / 2u);
		}

		if (mouse_position.x >= 1.0f && mouse_position.y >= 1.0f)
		{
			mouse_position.x /= camera_zoom;
			mouse_position.y /= camera_zoom;

			this->map_handler.map_index.x = mouse_position.x / this->map_handler.map_tile_size.x;
			this->map_handler.map_index.y = mouse_position.y / this->map_handler.map_tile_size.y;

			if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && 
				map_handler.map_index.x < map_handler.map_size.x &&
				map_handler.map_index.y < map_handler.map_size.y)
			{
				this->map_handler.UpdateMap(loader.r_image, loader.min);
			}
		}
	}
	else
	{
		viewport_hovered = 0u;
	}
	
	ImGui::End();
}

void Editor::Render(sf::RenderWindow& window)
{
	if (viewport_hovered && overlay == EditorFlags::IN_ACTIVE) {
		// tile.setPosition(ImGui::GetMousePos());
		// window.draw(tile);
	}
}

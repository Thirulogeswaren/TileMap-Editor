#include "editor.h"

#include "core/ts_loader.h"
#include "core/umap_handler.h"

#include "util/console.h"

#include "imgui.h"

static vector2u tiles_in{ 3u, 3u }, tile_sz{ 16u, 16u };

void Editor::MenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File")) {

			if (ImGui::MenuItem("Create Map"))
				this->overlay = EditorOverlay::MAP_ENABLED;

			if (ImGui::MenuItem("Save as Image"))
			{
				if (NFD_SaveDialog(nullptr, nullptr, &nfd_filepath) == NFD_OKAY)
				{
					CURRENT_MAP.SaveMap(nfd_filepath);
					Console::LogMessage("saved to ", nfd_filepath);
				}
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Exit"))
				exit(0);
			
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	if (this->overlay == EditorOverlay::MAP_ENABLED)
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
			ImGui::InputScalar("##t1", ImGuiDataType_U16, &tile_sz.x);

			ImGui::TableNextColumn();
			ImGui::Text("y "); ImGui::SameLine();
			ImGui::InputScalar("##t2", ImGuiDataType_U16, &tile_sz.y);

			ImGui::EndTable();
		}

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Text("Dimensions: %u x %u", tiles_in.x * tile_sz.x, tiles_in.y * tile_sz.y);

		ImGui::Spacing();
		ImGui::Spacing();

		if (ImGui::Button("Create Map")) {
			CURRENT_MAP.CreateMap(tiles_in, tile_sz);
			this->overlay = EditorOverlay::IN_ACTIVE;
		}

		ImGui::End();

	}
}

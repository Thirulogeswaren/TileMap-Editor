#include "umap_handler.h"
#include "ts_loader.h"

#include "util/console.h"

#include "imgui.h"

namespace {

	struct umap_handler_impl
	{
		sf::Texture		hmap_tex_buffer{};
		sf::Image		hmap_raw_buffer{};

	} umap_instance;

	sf::Color transparent{  };
}

sf::Texture& map_handler::GetFinalMap() const
{
	return umap_instance.hmap_tex_buffer;
}

void map_handler::CreateMap(const vector2u& tiles_in, const vector2u& size)
{
	CURRENT_MAP.map_tilepresent = tiles_in;
	CURRENT_MAP.map_tilesize = size;

	CURRENT_MAP.map_dimension.x = tiles_in.x * size.x;
	CURRENT_MAP.map_dimension.y = tiles_in.y * size.y;

	auto& [m_texture, m_image] = umap_instance;

	m_image.create(map_dimension.x, map_dimension.y, ImGui::GetStyle().Colors[ImGuiCol_WindowBg]);

	CURRENT_MAP.map_state = {
		m_texture.create(map_dimension.x, map_dimension.y)
	};
	
	m_texture.update(m_image);
}

void map_handler::PlaceTile()
{
	auto& [m_texture, m_image] = umap_instance;
	
	m_image.copy(TS_LOADER.m_raw_tileset,
		CURRENT_MAP.map_index.x * CURRENT_MAP.map_tilesize.x,
		CURRENT_MAP.map_index.y * CURRENT_MAP.map_tilesize.y,
		sf::IntRect{
			TS_LOADER.m_points.min.x, TS_LOADER.m_points.min.y,
			CURRENT_MAP.map_tilesize.x, CURRENT_MAP.map_tilesize.y
		},
		true
	);

	m_texture.update(m_image);
}

void map_handler::SaveMap(std::string_view filename)
{
	umap_instance.hmap_raw_buffer.saveToFile(filename.data());
} 


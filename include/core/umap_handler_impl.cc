#include "umap_handler.h"
#include "util/console.h"

// to get the tileset
#include "ts_loader.h"

namespace {

	sf::Image raw_map{}, refering_tileset{};

	class umap_handler_impl
	{
	public:
		static umap_handler_impl& GetInstance()
		{
			static umap_handler_impl instance{};
			return instance;
		}

		sf::Texture hmap_buffer;
	private:
		umap_handler_impl() {
			refering_tileset = TS_LOADER.target.copyToImage();
		};
		~umap_handler_impl() = default;
	};

}

sf::Texture& map_handler::GetFinalMap()
{
	return umap_handler_impl::GetInstance().hmap_buffer;
}

void map_handler::CreateMap(const vector2u& tiles_in, const vector2u& size)
{
	CURRENT_MAP.map_tilepresent = tiles_in;
	CURRENT_MAP.map_tilesize = size;

	CURRENT_MAP.map_dimension.x = tiles_in.x * size.x;
	CURRENT_MAP.map_dimension.y = tiles_in.y * size.y;

	raw_map.create(map_dimension.x, map_dimension.y);

	map_state = {
		umap_handler_impl::GetInstance().hmap_buffer.create(map_dimension.x, map_dimension.y)
	};

}

void map_handler::PlaceTile()
{
	raw_map.copy(refering_tileset,
		CURRENT_MAP.index.x * CURRENT_MAP.map_tilesize.x,
		CURRENT_MAP.index.y * CURRENT_MAP.map_tilesize.y,
		sf::IntRect{
			TS_LOADER.to_draw.min.x, TS_LOADER.to_draw.min.y,
			CURRENT_MAP.map_tilesize.x, CURRENT_MAP.map_tilesize.y
		},
		true
	);

	umap_handler_impl::GetInstance().hmap_buffer.update(raw_map);
}

void map_handler::SaveMap()
{
	raw_map.saveToFile("example.png");
}


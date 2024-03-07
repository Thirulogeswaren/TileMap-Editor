#include "umap_handler.h"
#include "util/console.h"

void map_handler::CreateMap(const vector2u& tiles_in, const vector2u& size)
{
	CURRENT_MAP.map_tilepresent = tiles_in;
	CURRENT_MAP.map_tilesize = size;

	CURRENT_MAP.map_dimension = {
		static_cast<float>(tiles_in.x * size.x),
		static_cast<float>(tiles_in.y * size.y)
	};

	if (umap.create(CURRENT_MAP.map_dimension.x, CURRENT_MAP.map_dimension.y))
	{
		umap.setActive();
		Console::LogMessage("RenderTexture Created");
	}
}

#include "ts_loader.h"

void map_handler::PlaceTile()
{
	umap.draw(TS_CURRENT.live_part);

	umap.display();
}



#include "hbuffer.h"
#include "../util/console.h"

static constexpr size_t buffer_size{ 128ull };
static char filename[buffer_size];

#include "imgui.h"

struct tileset_prop
{
	sf::Image image;
	sf::Vector2f size;
	float zscale;
};

#include <vector>

static std::vector<tileset_prop> vTileset;
static sf::Texture target{};

sf::Texture& hbuffer::getTarget() { return target; }

void hbuffer::loadImage(std::string_view filepath)
{
	memset(filename, 0, buffer_size);
	filepath.copy(filename, buffer_size);

	tileset_prop tileset;
	tileset.size = current.size;
	tileset.zscale = 1.0f;
	if (tileset.image.loadFromFile(filename)) {
		vTileset.push_back(tileset);
		current.capacity = (vTileset.size() - 1);
		Console::LogMessage("loaded ", filename, INFO);
	}
	else {
		Console::LogMessage("[error] loading ", filename, ERROR);
	}
	
	rTarget();
}

// called after saving ...
static void update_target()
{
	using namespace hbuffer;

	if (current.index > current.capacity) {
		current.index = current.capacity;
	}
	if (current.index <= 0) { current.index = 0; }

	const auto& [image, tsize, zscale] = vTileset[current.index];

	target.loadFromImage(image);
	current.size = tsize;
	current.scale = zscale;
}

void hbuffer::lTarget()
{
	if (vTileset.empty()) return;
	vTileset[current.index--].zscale = current.scale;
	update_target();
}

void hbuffer::rTarget()
{
	if (vTileset.empty()) return;
	vTileset[current.index++].zscale = current.scale;
	update_target();
}

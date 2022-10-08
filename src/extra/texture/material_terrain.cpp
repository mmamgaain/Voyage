#include "material_terrain.hpp"

namespace Voyage {
	MaterialTerrain::MaterialTerrain(Loader& loader, const char* const diffuse, const char* const red, const char* const green, const char* const blue, const char* const blend_map, float shineDamper, float reflectivity): diffuse(loader.loadTexture(diffuse)), red(loader.loadTexture(red)), green(loader.loadTexture(green)), blue(loader.loadTexture(blue)), blend_map(loader.loadTexture(blend_map, false)), shineDamper(shineDamper), reflectivity(reflectivity) {}
}

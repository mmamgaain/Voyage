#pragma once
#include <voyage.hpp>
#include "texture.hpp"
#include "extra/loader.hpp"

namespace Voyage {
	typedef struct MaterialTerrain {
		std::shared_ptr<Texture> diffuse = 0, red = 0, green = 0, blue = 0, blend_map = 0;
		float shineDamper = 0, reflectivity = 0;

		MaterialTerrain(Loader& loader, const char* const diffuse = 0, const char* const red = 0, const char* const green = 0, const char* const blue = 0, const char* const blend_map = 0, float shineDamper = 0, float reflectivity = 0);

		~MaterialTerrain() noexcept = default;
	} MaterialTerrain;
}


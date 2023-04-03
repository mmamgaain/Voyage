#pragma once
#include <voyage.hpp>
#include "Voyage/texture.hpp"
#include "Voyage/loader.hpp"

namespace Voyage {
	typedef struct MaterialTerrain {
		std::shared_ptr<Texture> diffuse, red, green, blue, blend_map;
		float shineDamper, reflectivity;

		MaterialTerrain(Loader& loader, const char* const diffuse = 0, const char* const red = 0, const char* const green = 0, const char* const blue = 0, const char* const blend_map = 0, float shineDamper = 0, float reflectivity = 0);

		MaterialTerrain(const MaterialTerrain& mat) noexcept;

		MaterialTerrain(MaterialTerrain&& mat) noexcept;

		MaterialTerrain& operator=(const MaterialTerrain& other) noexcept;

		MaterialTerrain& operator=(MaterialTerrain&& other) noexcept;

		uint32_t getDiffuseID() const noexcept;

		uint32_t getRedID() const noexcept;

		uint32_t getGreenID() const noexcept;

		uint32_t getBlueID() const noexcept;

		uint32_t getBlendMapID() const noexcept;

		bool isMultitextured() const noexcept;

		~MaterialTerrain() noexcept = default;
	} MaterialTerrain;
}


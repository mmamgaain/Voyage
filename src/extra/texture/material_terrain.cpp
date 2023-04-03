#include "Voyage/material_terrain.hpp"

namespace Voyage {
	MaterialTerrain::MaterialTerrain(Loader& loader, const char* const diffuse, const char* const red, const char* const green, const char* const blue, const char* const blend_map, float shineDamper, float reflectivity): diffuse(diffuse ? loader.loadTexture(diffuse) : nullptr), red(red ? loader.loadTexture(red) : nullptr), green(green ? loader.loadTexture(green) : nullptr), blue(blue ? loader.loadTexture(blue) : nullptr), blend_map(blend_map ? loader.loadTexture(blend_map, false) : nullptr), shineDamper(shineDamper), reflectivity(reflectivity) {}

	MaterialTerrain::MaterialTerrain(const MaterialTerrain& mat) noexcept: diffuse(mat.diffuse), red(mat.red), green(mat.green), blue(mat.blue), blend_map(mat.blend_map), shineDamper(mat.shineDamper), reflectivity(mat.reflectivity) {}

	MaterialTerrain::MaterialTerrain(MaterialTerrain&& mat) noexcept: diffuse(std::move(mat.diffuse)), red(std::move(mat.red)), green(std::move(mat.green)), blue(std::move(mat.blue)), blend_map(std::move(mat.blend_map)), shineDamper(mat.shineDamper), reflectivity(mat.reflectivity) {}

	MaterialTerrain& MaterialTerrain::operator=(const MaterialTerrain& other) noexcept {
		if(this == &other) return *this;
		diffuse = other.diffuse;
		red = other.red;
		green = other.green;
		blue = other.blue;
		blend_map = other.blend_map;
		shineDamper = other.shineDamper;
		reflectivity = other.reflectivity;
		return *this;
	}

	MaterialTerrain& MaterialTerrain::operator=(MaterialTerrain&& other) noexcept {
		if(this == &other) return *this;
		diffuse = std::move(other.diffuse);
		red = std::move(other.red);
		green = std::move(other.green);
		blue = std::move(other.blue);
		blend_map = std::move(other.blend_map);
		shineDamper = other.shineDamper;
		reflectivity = other.reflectivity;
		return *this;
	}

	uint32_t MaterialTerrain::getDiffuseID() const noexcept { return diffuse ? diffuse->getID() : 0; }

	uint32_t MaterialTerrain::getRedID() const noexcept { return red ? red->getID() : 0; }

	uint32_t MaterialTerrain::getGreenID() const noexcept { return green ? green->getID() : 0; }

	uint32_t MaterialTerrain::getBlueID() const noexcept { return blue ? blue->getID() : 0; }

	uint32_t MaterialTerrain::getBlendMapID() const noexcept { return blend_map ? blend_map->getID() : 0; }

	bool MaterialTerrain::isMultitextured() const noexcept { return red != 0 && green != 0 && blue != 0; }
}

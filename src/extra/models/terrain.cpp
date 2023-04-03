#include "Voyage/terrain.hpp"
#include "Voyage/material_terrain.hpp"
#include "Voyage/raw_model.hpp"

namespace Voyage {
	/* Terrain Properties starts */
	Terrain::TerrainProps::TerrainProps(uint32_t length, uint32_t width, float x, float z, int height, uint32_t grid_size, float fog_density, float fog_gradient): length(length), width(width), x(x), z(z), height(height), grid_size(grid_size), fog_density(fog_density), fog_gradient(fog_gradient) {}

	Terrain::TerrainProps::TerrainProps(const TerrainProps& properties): length(properties.length), width(properties.width), x(properties.x), z(properties.z), height(properties.height), grid_size(properties.grid_size), fog_density(properties.fog_density), fog_gradient(properties.fog_gradient) {}

	Terrain::TerrainProps& Terrain::TerrainProps::operator=(const Terrain::TerrainProps& other) noexcept {
		if(this == &other) return *this;
		length = other.length;
		width = other.width;
		grid_size = other.grid_size;
		height = other.height;
		x = other.x;
		z = other.z;
		fog_density = other.fog_density;
		fog_gradient = other.fog_gradient;

		return *this;
	}
	/* Terrain Properties ends */

	Terrain::Terrain(Loader& loader, const TerrainProps& properties, const MaterialTerrain& material): model(nullptr), properties(properties), material(material) { init_terrain(loader); }

	Terrain::Terrain(Loader& loader, const TerrainProps& properties, MaterialTerrain&& material): model(nullptr), properties(properties), material(std::move(material)) { init_terrain(loader); }

	Terrain::Terrain(const Terrain& terrain) noexcept: model(terrain.model), properties(terrain.properties), material(terrain.material) {}

	Terrain::Terrain(Terrain&& terrain) noexcept: model(std::move(terrain.model)), properties(terrain.properties), material(std::move(terrain.material)) {}

	Terrain::~Terrain() noexcept { model->dispose(); }

	Terrain& Terrain::operator=(const Terrain& other) noexcept {
		if(this == &other) return *this;
		model = other.model;
		properties = other.properties;
		material = other.material;

		return *this;
	}

	Terrain& Terrain::operator=(Terrain&& other) noexcept {
		if(this == &other) return *this;
		model = std::move(other.model);
		properties = std::move(other.properties);
		material = std::move(other.material);

		return *this;
	}

	RawModel& Terrain::get() const { return *(model.get()); }

	RawModel* Terrain::getptr() const { return model.get(); }

	const RawModel& Terrain::get_const() const { return *(model.get()); }

	const RawModel* const Terrain::getptr_const() const { return model.get(); }

	const float& Terrain::getX() const { return properties.x; }

	const float& Terrain::getZ() const { return properties.z; }

	const int& Terrain::getHeight() const { return properties.height; }

	const uint32_t& Terrain::getLength() const { return properties.length; }

	const uint32_t& Terrain::getWidth() const { return properties.width; }

	void Terrain::init_terrain(Loader& loader) {
		std::vector<float> VERTICES, TEXTURE_COORDS, NORMALS;
		uint32_t grid_size_2d = properties.grid_size * properties.grid_size;
		VERTICES.reserve(3 * grid_size_2d);
		TEXTURE_COORDS.reserve(2 * grid_size_2d);
		NORMALS.reserve(3 * grid_size_2d);
		uint32_t index, block_length = properties.length / properties.grid_size, block_width = properties.width / properties.grid_size;
		for(uint32_t j = 0; j < properties.grid_size; j++) {
			for(uint32_t i = 0; i < properties.grid_size; i++) {
				index = j * properties.grid_size + i;
				VERTICES.insert(VERTICES.begin() + (index * 3) + 0, properties.x - (properties.width / 2.0) + block_width * i);
				VERTICES.insert(VERTICES.begin() + (index * 3) + 0, properties.height);
				VERTICES.insert(VERTICES.begin() + (index * 3) + 0, properties.z - (properties.length / 2.0) + block_length * j);
				TEXTURE_COORDS.insert(TEXTURE_COORDS.begin() + (index * 2) + 0, (float)i / (properties.grid_size - 1));
				TEXTURE_COORDS.insert(TEXTURE_COORDS.begin() + (index * 2) + 1, (float)j / (properties.grid_size - 1));
				NORMALS.insert(NORMALS.begin() + (index * 3) + 0, 0);
				NORMALS.insert(NORMALS.begin() + (index * 3) + 1, 1);
				NORMALS.insert(NORMALS.begin() + (index * 3) + 2, 0);
			}
		}

		std::vector<uint32_t> INDICES;
		uint32_t bottom_left, top_right;
		INDICES.reserve(6 * (properties.grid_size - 1) * (properties.grid_size - 1));
		for(uint32_t j = 0; j < properties.grid_size - 1; j++) {
			for(uint32_t i = 0; i < properties.grid_size - 1; i++) {
				index = j * (properties.grid_size - 1) + i;
				bottom_left = index + properties.grid_size;
				top_right = index + 1;
				INDICES.insert(INDICES.begin() + (index * 6) + 0, index);
				INDICES.insert(INDICES.begin() + (index * 6) + 1, bottom_left);
				INDICES.insert(INDICES.begin() + (index * 6) + 2, top_right);
				INDICES.insert(INDICES.begin() + (index * 6) + 3, top_right);
				INDICES.insert(INDICES.begin() + (index * 6) + 4, bottom_left);
				INDICES.insert(INDICES.begin() + (index * 6) + 5, bottom_left + 1);
			}
		}

		model = loader.loadToVAO(VERTICES, 3, INDICES, TEXTURE_COORDS, NORMALS);
	}

	const Terrain::TerrainProps& Terrain::getProperties() const { return properties; }

	const MaterialTerrain& Terrain::getMaterial() const { return material; }
}

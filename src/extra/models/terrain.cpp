#include "terrain.hpp"

namespace Voyage {
	/* Terrain Properties starts */
	Terrain::TerrainProps::TerrainProps(unsigned int length, unsigned int width, float x, float z, int height, unsigned int grid_size, float fog_density, float fog_gradient): length(length), width(width), x(x), z(z), height(height), grid_size(grid_size), fog_density(fog_density), fog_gradient(fog_gradient) {}

	Terrain::TerrainProps::TerrainProps(const TerrainProps& properties): length(properties.length), width(properties.width), x(properties.x), z(properties.z), height(properties.height), grid_size(properties.grid_size), fog_density(0), fog_gradient(1) {}

	Terrain::TerrainProps::TerrainProps(TerrainProps&& properties): length(std::move(properties.length)), width(std::move(properties.width)), x(std::move(properties.x)), z(std::move(properties.z)), height(std::move(properties.height)), grid_size(std::move(properties.grid_size)), fog_density(0), fog_gradient(1) {}
	/* Terrain Properties ends */

	Terrain::Terrain(Loader& loader, const TerrainProps& properties, const MaterialTerrain& material): model(nullptr), properties(properties), material(material) { init_terrain(loader); }

	Terrain::Terrain(Loader& loader, TerrainProps&& properties, MaterialTerrain&& material): model(nullptr), properties(std::move(properties)), material(material) { init_terrain(loader); }

	Terrain::~Terrain() { model->dispose(); }

	RawModel& Terrain::get() const { return *model; }

	const RawModel& Terrain::get_const() const { return *model; }

	const float& Terrain::getX() const { return properties.x; }

	const float& Terrain::getZ() const { return properties.z; }

	const int& Terrain::getHeight() const { return properties.height; }

	const unsigned int& Terrain::getLength() const { return properties.length; }

	const unsigned int& Terrain::getWidth() const { return properties.width; }

	void Terrain::init_terrain(Loader& loader) {
		std::vector<float> VERTICES, TEXTURE_COORDS, NORMALS;
		unsigned int grid_size_2d = properties.grid_size * properties.grid_size;
		VERTICES.reserve(3 * grid_size_2d);
		TEXTURE_COORDS.reserve(2 * grid_size_2d);
		NORMALS.reserve(3 * grid_size_2d);
		uint32_t index, block_length = properties.length / properties.grid_size, block_width = properties.width / properties.grid_size;
		for(unsigned int j = 0; j < properties.grid_size; j++) {
			for(unsigned int i = 0; i < properties.grid_size; i++) {
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

		std::vector<unsigned int> INDICES;
		uint32_t bottom_left, top_right;
		INDICES.reserve(6 * (properties.grid_size - 1) * (properties.grid_size - 1));
		for(unsigned int j = 0; j < properties.grid_size - 1; j++) {
			for(unsigned int i = 0; i < properties.grid_size - 1; i++) {
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

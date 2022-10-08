#pragma once
#include "voyage.hpp"
#include "raw_model.hpp"
#include "extra/loader.hpp"
#include "extra/texture/material_terrain.hpp"

namespace Voyage {
	typedef struct Terrain {
		public:
			typedef struct TerrainProps {
				unsigned int length, width, grid_size;
				int height;
				float x, z;
				float fog_density = 0, fog_gradient = 1;

				TerrainProps(unsigned int length, unsigned int width, float x, float z, int height = 0, unsigned int grid_size = 50, float fog_density = 0, float fog_gradient = 1);

				TerrainProps(const TerrainProps& properties);

				TerrainProps(TerrainProps&& properties);

				~TerrainProps() noexcept = default;
			} TerrainProps;

			Terrain(Loader& loader, const TerrainProps& properties, const MaterialTerrain& material);

			Terrain(Loader& loader, TerrainProps&& properties, MaterialTerrain&& material);

			~Terrain();

			const RawModel& get_const() const;

			RawModel& get() const;

			const float& getX() const;

			const float& getZ() const;

			const int& getHeight() const;

			const unsigned int& getLength() const;

			const unsigned int& getWidth() const;

			const TerrainProps& getProperties() const;

			const MaterialTerrain& getMaterial() const;
		private:
			std::shared_ptr<RawModel> model;
			TerrainProps properties;
			MaterialTerrain material;

			void init_terrain(Loader& loader);

	} Terrain;
}

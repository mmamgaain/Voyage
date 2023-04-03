#pragma once
#include <voyage.hpp>
#include "Voyage/raw_model.hpp"
#include "Voyage/loader.hpp"
#include "Voyage/material_terrain.hpp"

namespace Voyage {
	typedef struct Terrain {
		public:
			typedef struct TerrainProps {
				uint32_t length, width, grid_size;
				int height;
				float x, z, fog_density = 0, fog_gradient = 1;

				TerrainProps(uint32_t length, uint32_t width, float x, float z, int height = 0, uint32_t grid_size = 50, float fog_density = 0, float fog_gradient = 1);

				TerrainProps(const TerrainProps& properties);

				TerrainProps& operator=(const TerrainProps& other) noexcept;

				~TerrainProps() noexcept = default;
			} TerrainProps;

			Terrain(Loader& loader, const TerrainProps& properties, const MaterialTerrain& material);

			Terrain(Loader& loader, const TerrainProps& properties, MaterialTerrain&& material);

			Terrain(const Terrain& terrain) noexcept;

			Terrain(Terrain&& terrain) noexcept;

			~Terrain() noexcept;

			Terrain& operator=(const Terrain& other) noexcept;

			Terrain& operator=(Terrain&& other) noexcept;

			const RawModel& get_const() const;

			const RawModel* const getptr_const() const;

			RawModel& get() const;

			RawModel* getptr() const;

			const float& getX() const;

			const float& getZ() const;

			const int& getHeight() const;

			const uint32_t& getLength() const;

			const uint32_t& getWidth() const;

			const TerrainProps& getProperties() const;

			const MaterialTerrain& getMaterial() const;
		private:
			std::shared_ptr<RawModel> model;
			TerrainProps properties;
			MaterialTerrain material;

			void init_terrain(Loader& loader);

	} Terrain;
}

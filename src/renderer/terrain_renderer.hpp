#pragma once
#include <voyage.hpp>
#include "extra/models/terrain.hpp"
#include "shaders/shader_program.hpp"
#include "extra/light.hpp"
#include "renderer.hpp"

namespace Voyage {

	class TerrainRenderer {
		public:
			TerrainRenderer(const char* const vertex_file, const char* const fragment_file, const glm::mat4& projection);

			~TerrainRenderer();

			void prepareRender(const Terrain& model) const;

			void render(const std::vector<Terrain>& model, const glm::mat4& view, const DirectionalLight& light) const;

			void dispose();
		private:
			ShaderProgram* shader;
			Renderer renderer;
			mutable glm::mat4 modelMatrix;

	void loadMaterial(const MaterialTerrain& material) const;

	};

}

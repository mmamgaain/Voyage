#pragma once
#include <voyage.hpp>
#include "Voyage/light.hpp"
#include "Voyage/model.hpp"
#include "Voyage/renderer.hpp"
#include "Voyage/shader_program.hpp"

namespace Voyage {
	class ModelRenderer {
		public:
			ModelRenderer(const char* const vertex_file, const char* const fragment_file, const glm::mat4& projection);

			~ModelRenderer();

			void render(const Model& model, const glm::mat4& view, DirectionalLight& light);

			void dispose();
		private:
			Renderer renderer;
			ShaderProgram *shader;
			glm::mat4 transform;

			void loadMaterial(const Material& material);
	};
}

#pragma once
#include <voyage.hpp>
#include "extra/light.hpp"
#include "extra/models/model.hpp"
#include "renderer.hpp"
#include "shaders/shader_program.hpp"

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

	};
}

#pragma once
#include <voyage.hpp>
#include "renderer.hpp"
#include "shaders/shader_program.hpp"
#include "extra/models/cards.hpp"

namespace Voyage {
	class CardsRenderer {
		public:
			CardsRenderer(const char* vertex_file, const char* fragment_file);

			~CardsRenderer();

			void render(const Cards& card);
		private:
			Renderer renderer;
			ShaderProgram* shader;
			glm::mat4 transform;
	};
}

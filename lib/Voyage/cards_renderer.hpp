#pragma once
#include <voyage.hpp>
#include "Voyage/renderer.hpp"
#include "Voyage/shader_program.hpp"
#include "Voyage/cards.hpp"

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

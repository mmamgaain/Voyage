#include <voyage.hpp>
#include "Voyage/material.hpp"
#include "renderer.hpp"
#include "raw_model.hpp"
#include "shader_program.hpp"
#include "loader.hpp"
#include "particle.hpp"

namespace Voyage {
	class ParticleRenderer {
		public:
			ParticleRenderer(const char* const vertex_file, const char* const fragment_file, Loader& loader, const glm::mat4& projection);

			~ParticleRenderer() noexcept;

			void render(const std::vector<Particle> particles, const glm::mat4& view);

			void dispose();
		private:
			std::shared_ptr<RawModel> model;
			ShaderProgram* shader;
			Renderer renderer;
			glm::mat4 dest;

			void prepare();

			glm::mat4& getModelViewMatrix(const glm::vec3& position, const glm::vec3& scale, const float& rotation, const glm::mat4& view);

			void loadMaterial(const Material& material) const;

			void finish();
	};
}

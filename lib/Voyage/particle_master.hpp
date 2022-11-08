#include <voyage.hpp>
#include "Voyage/particle_renderer.hpp"

namespace Voyage {
	class ParticleMaster {
		public:
			ParticleMaster() = delete;

			~ParticleMaster() = delete;

			static void init(const char* const vertex_file, const char* const fragment_file, Loader& loader, const glm::mat4& projection);

			static void addParticle(const glm::vec3& position, const glm::vec3& velocity = {}, const glm::vec3& scale = {0.2, 0.2, 0.2}, const float& gravityEffect = 1.0, const float& lifeLength = 1.0, const float& rotation = 0.0);

			static void update();

			static const int getNumParticles();

			static void render(const glm::mat4& view);

			static void dispose();
		private:
			static std::vector<Particle> particles;
			static ParticleRenderer* renderer;
	};
}

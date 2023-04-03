#pragma once
#include <vector>
#include <voyage.hpp>
#include "Voyage/particle.hpp"
#include "Voyage/particle_renderer.hpp"
#include "Voyage/particle_texture.hpp"

namespace Voyage {
	class ParticleMaster {
		public:
			ParticleMaster() = delete;

			~ParticleMaster() = delete;

			static void init(const char* const vertex_file, const char* const fragment_file, Loader& loader, const glm::mat4& projection);

			static void addParticle(const ParticleTexture* texture, const glm::vec3& position, const glm::vec3& velocity = {}, const glm::vec3& scale = {1.0, 1.0, 1.0}, const float& gravityEffect = 1.0, const float& lifeLength = 1.0, const float& rotation = 0.0);

			static const int getNumParticles();

			static void render(const glm::mat4& view);

			static void dispose();
			static size_t pool_size;
			static std::unordered_map<const ParticleTexture*, std::vector<ParticleInstanced>> particles;
		private:
			static ParticleRenderer* renderer;
	};
}

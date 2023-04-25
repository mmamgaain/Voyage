#include "Voyage/particle_master.hpp"
#include "Voyage/particle.hpp"
#include "Voyage/particle_texture.hpp"
#include "Voyage/types.hpp"
#include <algorithm>
#include <cstdint>

namespace Voyage {

	ParticleRenderer* ParticleMaster::renderer;
	size_t ParticleMaster::pool_size = 50;
	std::unordered_map<const ParticleTexture*, std::vector<ParticleInstanced>> ParticleMaster::particles;

	void ParticleMaster::init(const char* const vertex_file, const char* const fragment_file, Loader& loader, const glm::mat4& projection) { renderer = new ParticleRenderer(vertex_file, fragment_file, loader, projection); }

	void ParticleMaster::addParticle(const ParticleTexture* texture, const glm::vec3& position, const glm::vec3& velocity, const glm::vec3& scale, const float& gravityEffect, const float& lifeLength, const float& rotation) {
		const auto& it = particles.find(texture);
		if(it != particles.end()) {
			auto& particlesPerTex = it->second;
			const auto& it = std::find_if_not(particlesPerTex.begin(), particlesPerTex.end(), [](const ParticleInstanced& particle) { return particle.is_alive; });
			if(it != particlesPerTex.end()) it->set(position, velocity, scale, gravityEffect, lifeLength, rotation, texture->getNumberOfRows(), texture->getNumberOfCols());
		}
		else {
			std::vector<ParticleInstanced> newPartList(pool_size);
			newPartList[0].set(position, velocity, scale, gravityEffect, lifeLength, rotation, texture->getNumberOfRows(), texture->getNumberOfCols());
			particles[texture] = std::move(newPartList);
		}
	}

	uint32_t ParticleMaster::getNumParticles(const ParticleTexture* texture) {
		if(!texture) {
			uint32_t finalCount = 0;
			auto it = particles.begin();
			while(it != particles.end()) {
				const auto& parts = it->second;
				finalCount += std::count_if(parts.begin(), parts.end(), [](const ParticleInstanced& particle) { return particle.is_alive; });
				++it;
			}
			return finalCount;
		}
		const auto& parts = particles.find(texture);
		if(parts != particles.end()) return std::count_if(parts->second.begin(), parts->second.end(), [](const ParticleInstanced& particle) { return particle.is_alive; });
		return 0;
	}

	void ParticleMaster::render(const glm::mat4& view) { renderer->render(particles, view); }

	void ParticleMaster::dispose() { SAFE_DELETE(renderer); particles.clear(); }

}

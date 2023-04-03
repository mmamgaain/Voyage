#include "Voyage/particle_master.hpp"
#include "Voyage/particle.hpp"
#include "Voyage/particle_texture.hpp"
#include "Voyage/types.hpp"
#include <cstdint>
#include <vector>

namespace Voyage {

	ParticleRenderer* ParticleMaster::renderer;
	size_t ParticleMaster::pool_size = 25;
	std::unordered_map<const ParticleTexture*, std::vector<ParticleInstanced>> ParticleMaster::particles;

	void ParticleMaster::init(const char* const vertex_file, const char* const fragment_file, Loader& loader, const glm::mat4& projection) { renderer = new ParticleRenderer(vertex_file, fragment_file, loader, projection); }

	void ParticleMaster::addParticle(const ParticleTexture* texture, const glm::vec3& position, const glm::vec3& velocity, const glm::vec3& scale, const float& gravityEffect, const float& lifeLength, const float& rotation) {
		const auto& it = particles.find(texture);
		if(it == particles.end()) {
			std::vector<ParticleInstanced> newPartList(pool_size);
			newPartList[0].set(position, velocity, scale, gravityEffect, lifeLength, rotation, texture->getNumberOfRows(), texture->getNumberOfCols());
			particles[texture] = std::move(newPartList);
		}
		else {
			auto& particlesPerTex = it->second;
			for(uint32_t i = 0; i < pool_size; ++i) {
				if(particlesPerTex[i].is_alive) continue;
				particlesPerTex[i].set(position, velocity, scale, gravityEffect, lifeLength, rotation, texture->getNumberOfRows(), texture->getNumberOfCols());
				break;
			}
		}
			/* auto particlesPerTex = it->second.begin();
			   while(particlesPerTex != it->second.end()) {
			   if(particlesPerTex->is_alive) { ++particlesPerTex; continue; }
			   particlesPerTex->set(position, velocity, scale, gravityEffect, lifeLength, rotation, texture->getNumberOfRows(), texture->getNumberOfCols());
			   break;
			   } */
	}

	const int ParticleMaster::getNumParticles() { return particles.size(); }

	void ParticleMaster::render(const glm::mat4& view) { renderer->render(particles, view); }

	void ParticleMaster::dispose() { SAFE_DELETE(renderer); particles.clear(); }

}

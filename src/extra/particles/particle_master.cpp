#include "Voyage/particle_master.hpp"

namespace Voyage {
	std::unordered_map<const ParticleTexture*, std::vector<ParticleInstanced>> ParticleMaster::particles;
	ParticleRenderer* ParticleMaster::renderer;

	void ParticleMaster::init(const char* const vertex_file, const char* const fragment_file, Loader& loader, const glm::mat4& projection) { renderer = new ParticleRenderer(vertex_file, fragment_file, loader, projection); }

	void ParticleMaster::addParticle(const ParticleTexture* texture, const glm::vec3& position, const glm::vec3& velocity, const glm::vec3& scale, const float& gravityEffect, const float& lifeLength, const float& rotation) {
		const auto& it = particles.find(texture);
		if(it == particles.end()) {
			std::vector<ParticleInstanced> newPartList = std::vector<ParticleInstanced>();
			newPartList.emplace_back(position, velocity, scale, gravityEffect, lifeLength, rotation);
			particles[texture] = newPartList;
		}
		else it->second.emplace_back(position, velocity, scale, gravityEffect, lifeLength, rotation, texture->getNumberOfRows(), texture->getNumberOfCols());
	}

	const int ParticleMaster::getNumParticles() { return particles.size(); }

	void ParticleMaster::render(const glm::mat4& view) { renderer->render(particles, view); }

	void ParticleMaster::dispose() { if(renderer) delete renderer; particles.clear(); }

}

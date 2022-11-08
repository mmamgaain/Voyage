#include "Voyage/particle_master.hpp"

namespace Voyage {
	std::vector<Particle> ParticleMaster::particles;
	ParticleRenderer* ParticleMaster::renderer;

	void ParticleMaster::init(const char* const vertex_file, const char* const fragment_file, Loader& loader, const glm::mat4& projection) { renderer = new ParticleRenderer(vertex_file, fragment_file, loader, projection); }

	void ParticleMaster::addParticle(const glm::vec3& position, const glm::vec3& velocity, const glm::vec3& scale, const float& gravityEffect, const float& lifeLength, const float& rotation) { particles.emplace_back(position, velocity, scale, gravityEffect, lifeLength); }

	void ParticleMaster::update() {
		// Vector Iterator
		auto it = particles.begin();
		while(it != particles.end()) {
			if(!it->update())
				// 'erase()' invalidates the iterator, use returned iterator
				it = particles.erase(it);
			// Only increment when not erasing...
			else ++it;
		}
	}

	const int ParticleMaster::getNumParticles() { return particles.size(); }

	void ParticleMaster::render(const glm::mat4& view) {
		renderer->render(particles, view);
	}

	void ParticleMaster::dispose() { if(renderer) delete renderer; particles.clear(); }

}

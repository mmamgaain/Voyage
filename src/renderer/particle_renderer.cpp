#include "Voyage/particle_renderer.hpp"
#include "Voyage/particle_master.hpp"
#include "Voyage/particle_texture.hpp"

namespace Voyage {
	ParticleRenderer::ParticleRenderer(const char* const vertex_file, const char* const fragment_file, Loader& loader, const glm::mat4& projection): model(loader.loadToVAO({-1.0F, 1.0F, -1.0F, -1.0F, 1.0F, 1.0F, 1.0F, -1.0F}, 2)), shader(new ShaderProgram(vertex_file, fragment_file, 1)), dest(glm::identity<glm::mat4>()), vboID(loader.loadEmptyVBO(1000)) {
		shader->start();
		shader->loadUniform("project", projection);
		shader->stop();
		// TODO: Start adding instanced data
	}

	ParticleRenderer::~ParticleRenderer() noexcept { dispose(); }

	void ParticleRenderer::dispose() noexcept { if(shader) delete shader; glDeleteBuffers(1, &vboID); model->dispose(); }

	void ParticleRenderer::prepare() {
		renderer.prepareRender(model.get());
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);
	}

	glm::mat4& ParticleRenderer::getModelViewMatrix(const glm::vec3& position, const glm::vec3& scale, const float& rotation, const glm::mat4& view) {
		dest = glm::identity<glm::mat4>();
		dest = glm::translate(dest, position);

		dest[0][0] = view[0][0];
		dest[0][1] = view[1][0];
		dest[0][2] = view[2][0];
		dest[1][0] = view[0][1];
		dest[1][1] = view[1][1];
		dest[1][2] = view[2][1];
		dest[2][0] = view[0][2];
		dest[2][1] = view[1][2];
		dest[2][2] = view[2][2];

		dest = glm::rotate(dest, rotation, {1, 1, 1});
		dest = glm::scale(dest, scale);

		dest = view * dest;

		return dest;
	}

	void ParticleRenderer::render(const Particle& particle, const glm::mat4& view) {
		shader->start();
		prepare();
		shader->loadUniform("modelView", getModelViewMatrix(particle.getPosition(), particle.getScale(), particle.getRotation(), view));
		renderer.drawTriangleCall(model.get());
		finish();
		shader->stop();
	}

	void ParticleRenderer::render(std::unordered_map<const ParticleTexture*, std::vector<ParticleInstanced>>& it, const glm::mat4& view) {
		shader->start();
		auto partI = it.begin();
		while(partI != it.end()) {
			std::vector<ParticleInstanced>& particlesPerTex = partI->second;
			if(particlesPerTex.empty()) {
				partI = it.erase(partI);
				continue;
			}
			prepare();
			const ParticleTexture* const partTex = partI->first;
			renderer.loadTexture2D(0, partTex->getTexture()->getID());
			shader->loadUniform("texMapDimensions", partTex->getNumberOfRows(), partTex->getNumberOfCols());
			auto particle = particlesPerTex.begin();
			while(particle != particlesPerTex.end()) {
				if(!particle->update()) {
					particle = particlesPerTex.erase(particle);
					continue;
				}
				shader->loadUniform("texOffsetCurr", particle->texOffsetCurr);
				shader->loadUniform("texOffsetNext", particle->texOffsetNext);
				shader->loadUniform("blend", particle->blend);
				shader->loadUniform("modelView", getModelViewMatrix(particle->position, particle->scale, particle->rotation, view));
				renderer.drawTriangleCall(model.get());
				particle++;
			}
			partI++;
		}
		finish();
		shader->stop();
	}

	void ParticleRenderer::finish() {
		renderer.finishRender(model.get());
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
	}
}

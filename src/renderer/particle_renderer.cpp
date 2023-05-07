#include "Voyage/particle_renderer.hpp"
#include "Voyage/loader.hpp"
#include "Voyage/particle.hpp"
#include "Voyage/particle_master.hpp"
#include "Voyage/particle_texture.hpp"

namespace Voyage {

	uint32_t ParticleRenderer::MAX_INSTANCES = 100,
			 ParticleRenderer::INSTANCE_DATA_LENGTH = 21;
	float* ParticleRenderer::data = new float[MAX_INSTANCES * INSTANCE_DATA_LENGTH];

	ParticleRenderer::ParticleRenderer(const char* const vertex_file, const char* const fragment_file, Loader& loader, const glm::mat4& projection): model(loader.loadToVAO({-1.0F, 1.0F, -1.0F, -1.0F, 1.0F, 1.0F, 1.0F, -1.0F}, 2)), shader(new ShaderProgram(vertex_file, fragment_file, 1)), dest(glm::mat4(1.0)), vboID(loader.loadEmptyVBO(MAX_INSTANCES * INSTANCE_DATA_LENGTH)), loader(&loader) {
		shader->start();
		shader->loadUniform("project", projection);
		shader->stop();

		model->addInstancedAttribute(vboID, 1, 4, INSTANCE_DATA_LENGTH, (const void*)(0 * sizeof(float)));
		model->addInstancedAttribute(vboID, 2, 4, INSTANCE_DATA_LENGTH, (const void*)(4 * sizeof(float)));
		model->addInstancedAttribute(vboID, 3, 4, INSTANCE_DATA_LENGTH, (const void*)(8 * sizeof(float)));
		model->addInstancedAttribute(vboID, 4, 4, INSTANCE_DATA_LENGTH, (const void*)(12 * sizeof(float)));
		model->addInstancedAttribute(vboID, 5, 4, INSTANCE_DATA_LENGTH, (const void*)(16 * sizeof(float)));
		model->addInstancedAttribute(vboID, 6, 1, INSTANCE_DATA_LENGTH, (const void*)(20 * sizeof(float)));
	}

	ParticleRenderer::~ParticleRenderer() noexcept { dispose(); }

	void ParticleRenderer::dispose() noexcept { if(shader) delete shader; glDeleteBuffers(1, &vboID); model->dispose(); delete[] data; data = nullptr; }

	void ParticleRenderer::prepare() {
		renderer.prepareRender(model.get());
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);
		glEnableVertexAttribArray(5);
		glEnableVertexAttribArray(6);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);
	}

	glm::mat4& ParticleRenderer::getModelViewMatrix(const glm::vec3& position, const glm::vec3& scale, const float& rotation, const glm::mat4& view) {
		dest = glm::mat4(1.0);
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

	void ParticleRenderer::getModelViewMatrix(const glm::vec3& position, const glm::vec3& scale, const float& rotation, const glm::mat4& view, float* const data) { memcpy(data, &getModelViewMatrix(position, scale, rotation, view), 16 * sizeof(float)); }

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
		for(auto partI = it.begin(); partI != it.end(); ++partI) {
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
			uint32_t live_particles_count = 0;
			while(particle != particlesPerTex.end()) {
				if(particle->update()) {
					memcpy(&data[live_particles_count * INSTANCE_DATA_LENGTH], &getModelViewMatrix(particle->position, particle->scale, particle->rotation, view), 16 * sizeof(float));
					memcpy(&data[live_particles_count * INSTANCE_DATA_LENGTH + 16], &particle->texOffsetCurr, 2 * sizeof(float));
					memcpy(&data[live_particles_count * INSTANCE_DATA_LENGTH + 18], &particle->texOffsetNext, 2 * sizeof(float));
					data[live_particles_count * INSTANCE_DATA_LENGTH + 20] = particle->blend;
					++live_particles_count;
				}
				++particle;
			}
			loader->updateVBO(vboID, data, live_particles_count * sizeof(ParticleInstanced) / sizeof(float));
			glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, model->getVertexCount(), live_particles_count);
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

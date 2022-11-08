#include "Voyage/particle_renderer.hpp"
#include "glm/ext/matrix_transform.hpp"

namespace Voyage {
	ParticleRenderer::ParticleRenderer(const char* const vertex_file, const char* const fragment_file, Loader& loader, const glm::mat4& projection): model(loader.loadToVAO({-0.5F, 0.5F, -0.5F, -0.5F, 0.5F, 0.5F, 0.5F, -0.5F}, 2)), shader(new ShaderProgram(vertex_file, fragment_file)) {
		shader->start();
		shader->loadUniform("project", projection);
		shader->stop();
	}

	ParticleRenderer::~ParticleRenderer() noexcept { dispose(); }

	void ParticleRenderer::dispose() { if(shader) delete shader; }

	void ParticleRenderer::prepare() {
		renderer.prepareRender(*model);
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

		dest = glm::rotate(dest, rotation, {0, 0, 1});
		dest = glm::scale(dest, scale);

		dest = view * dest;
		return dest;
	}

	void ParticleRenderer::loadMaterial(const Material& material) const {
		shader->loadUniform("material.hasDiffuseTexture", material.hasTexture());
		shader->loadUniform("material.hasNormalMap", material.hasNormalMap());
		shader->loadUniform("hasNormalMap", material.hasNormalMap());
		shader->loadUniform("material.hasSpecularMap", material.hasSpecularMap());
		shader->loadUniform("isTransparent", material.transparency == 0);
		shader->loadUniform("material.isTransparent", material.transparency == 0);
		shader->loadUniform("material.shineDamper", material.shineDamper);
		shader->loadUniform("material.reflectivity", material.specularReflectivity);
		// TODO: Make the actual environment reflectivity
		shader->loadUniform("enviroRefractivity", 0.0F);
		shader->loadUniform("material.transparency", material.transparency);
		shader->loadUniform("material.diffuse", material.getDiffuseColor());
		shader->loadUniform("material.ambient", material.getAmbientColor());
		shader->loadUniform("material.specular", material.getSpecularColor());
		shader->loadUniform("numberOfRows", material.getRows());
		shader->loadUniform("material.hasFresnel", material.hasFresnel);
		shader->loadUniform("material.fresnelPower", material.fresnelPower);
	}

	void ParticleRenderer::render(const std::vector<Particle> particles, const glm::mat4& view) {
		shader->start();
		prepare();
		for(const Particle& particle : particles) {
			shader->loadUniform("modelView", getModelViewMatrix(particle.getPosition(), particle.getScale(), particle.getRotation(), view));
			loadMaterial(particle.getMaterial());
			renderer.drawTriangleCall(*model);
		}
		finish();
		shader->stop();
	}

	void ParticleRenderer::finish() {
		renderer.finishRender(*model);
		glDepthMask(GL_TRUE);
	}
}

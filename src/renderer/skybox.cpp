#include "Voyage/skybox.hpp"
#include "Voyage/core.hpp"

namespace Voyage {

	std::shared_ptr<RawModel> SkyboxRenderer::skybox = nullptr;
	std::shared_ptr<Texture> SkyboxRenderer::texture = nullptr;
	float SkyboxRenderer::SIZE = 500.0F, SkyboxRenderer::speed = 0.0F, SkyboxRenderer::rotation = 0.0F;
	std::vector<float> SkyboxRenderer::VERTICES = {
			-SIZE,  SIZE, -SIZE,
		    -SIZE, -SIZE, -SIZE,
		     SIZE, -SIZE, -SIZE,
		     SIZE, -SIZE, -SIZE,
		     SIZE,  SIZE, -SIZE,
		    -SIZE,  SIZE, -SIZE,

		    -SIZE, -SIZE,  SIZE,
		    -SIZE, -SIZE, -SIZE,
		    -SIZE,  SIZE, -SIZE,
		    -SIZE,  SIZE, -SIZE,
		    -SIZE,  SIZE,  SIZE,
		    -SIZE, -SIZE,  SIZE,

		     SIZE, -SIZE, -SIZE,
		     SIZE, -SIZE,  SIZE,
		     SIZE,  SIZE,  SIZE,
		     SIZE,  SIZE,  SIZE,
		     SIZE,  SIZE, -SIZE,
		     SIZE, -SIZE, -SIZE,

		    -SIZE, -SIZE,  SIZE,
		    -SIZE,  SIZE,  SIZE,
		     SIZE,  SIZE,  SIZE,
		     SIZE,  SIZE,  SIZE,
		     SIZE, -SIZE,  SIZE,
		    -SIZE, -SIZE,  SIZE,

		    -SIZE,  SIZE, -SIZE,
		     SIZE,  SIZE, -SIZE,
		     SIZE,  SIZE,  SIZE,
		     SIZE,  SIZE,  SIZE,
		    -SIZE,  SIZE,  SIZE,
		    -SIZE,  SIZE, -SIZE,

		    -SIZE, -SIZE, -SIZE,
		    -SIZE, -SIZE,  SIZE,
		     SIZE, -SIZE, -SIZE,
		     SIZE, -SIZE, -SIZE,
		    -SIZE, -SIZE,  SIZE,
		     SIZE, -SIZE,  SIZE
	};
	ShaderProgram* SkyboxRenderer::shader = nullptr;
	Renderer* SkyboxRenderer::renderer= nullptr;
	SKYBOX_ROTATION_DIRECTION SkyboxRenderer::direction = SKYBOX_ROTATION_DIRECTION::NONE;

	void SkyboxRenderer::init(const char* vertex_file, const char* fragment_file, const glm::mat4& projection, Loader& loader, const std::vector<const char*>& skybox_textures) {
		skybox = loader.loadToVAO(VERTICES, 3);
		texture = loader.loadTexture(skybox_textures);
		shader = new ShaderProgram(vertex_file, fragment_file, 1);
		renderer = new Renderer();
		shader->start();
		shader->loadUniform("project", projection);
		shader->stop();
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	}

	void SkyboxRenderer::setSkyTextures(Loader& loader, const std::vector<const char*>& skybox_textures) {
		std::shared_ptr<Texture> newTex = std::move(loader.loadTexture(skybox_textures));
		if(newTex) texture = std::move(newTex);
	}

	void SkyboxRenderer::setSkyRotation(const SKYBOX_ROTATION_DIRECTION& direction, const float& speed) {
		SkyboxRenderer::speed = speed;
		SkyboxRenderer::direction = direction;
	}

	void SkyboxRenderer::prepareRender() {
		shader->start();
		renderer->prepareRender(skybox.get());
		glDepthMask(GL_FALSE);
		glDepthRange(0.9999999, 0.9999999);
		renderer->loadTextureCubeMap(0, texture->getID());
	}

	void SkyboxRenderer::finishRender() {
		glDepthRange(0, 1);
		glDepthMask(GL_TRUE);
		renderer->finishRender(skybox.get());
		shader->stop();
	}

	void SkyboxRenderer::render(const glm::mat4& view) {
		prepareRender();
		loadViewMatrix(view);
		glDrawArrays(GL_TRIANGLES, 0, skybox->getVertexCount());
		finishRender();
	}

	void SkyboxRenderer::loadViewMatrix(glm::mat4 view) {
		view[3][0] = 0;
		view[3][1] = 0;
		view[3][2] = 0;
		if(direction == SKYBOX_ROTATION_DIRECTION::ANTICLOCKWISE) rotation += speed * std::max(Core::deltaTime, 0.016);
		else if(direction == SKYBOX_ROTATION_DIRECTION::CLOCKWISE) rotation -= speed * std::max(Core::deltaTime, 0.016);
		view = glm::rotate(view, glm::radians(rotation), {0, 1, 0});
		shader->loadUniform("view", view);
	}

	void SkyboxRenderer::dispose() { if(skybox) { skybox->dispose(); skybox = nullptr; } if(texture) { texture->dispose(); texture = nullptr; } if(shader) { delete shader; shader = nullptr; } if(renderer) { delete renderer; renderer = nullptr; } }

}

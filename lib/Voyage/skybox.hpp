#pragma once
#include "voyage.hpp"
#include "Voyage/renderer.hpp"
#include "Voyage/loader.hpp"
#include "Voyage/shader_program.hpp"

namespace Voyage {

	typedef enum SKYBOX_ROTATION_DIRECTION { NONE, CLOCKWISE, ANTICLOCKWISE } SKYBOX_ROTATION_DIRECTION;

	class SkyboxRenderer {
		public:
			SkyboxRenderer() = delete;

			SkyboxRenderer(const SkyboxRenderer& renderer) = delete;

			SkyboxRenderer(SkyboxRenderer&& renderer) = delete;

			SkyboxRenderer& operator=(const SkyboxRenderer& other) = delete;

			SkyboxRenderer& operator=(SkyboxRenderer&& other) = delete;

			~SkyboxRenderer() = delete;

			static void init(const char* vertex_file, const char* fragment_file, const glm::mat4& projection, Loader& loader, const std::vector<const char*>& skybox_textures);

			static void setSkyTextures(Loader& loader, const std::vector<const char*>& skybox_textures);

			static void setSkyRotation(const SKYBOX_ROTATION_DIRECTION& direction, const float& speed = 0.05F);

			static void dispose();

			static void render(const glm::mat4& view);
		private:
			static void prepareRender();

			static void finishRender();

			static std::shared_ptr<RawModel> skybox;
			static std::shared_ptr<Texture> texture;
			static float SIZE, speed, rotation;
			static std::vector<float> VERTICES;
			static ShaderProgram* shader;
			static Renderer* renderer;
			static SKYBOX_ROTATION_DIRECTION direction;

			static void loadViewMatrix(glm::mat4 view);
	};

}

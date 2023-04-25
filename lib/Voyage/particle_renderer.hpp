#pragma once
#include <voyage.hpp>
#include "renderer.hpp"
#include "shader_program.hpp"
#include "loader.hpp"
#include "particle.hpp"

namespace Voyage {
	class ParticleRenderer {
		public:
			ParticleRenderer(const char* const vertex_file, const char* const fragment_file, Loader& loader, const glm::mat4& projection);

			~ParticleRenderer() noexcept;

			void render(const Particle& particle, const glm::mat4& view);

			void render(std::unordered_map<const ParticleTexture*, std::vector<ParticleInstanced>>& it, const glm::mat4& view);

			void dispose() noexcept;
		private:
			std::shared_ptr<RawModel> model;
			ShaderProgram* shader;
			Renderer renderer;
			glm::mat4 dest;
			uint32_t vboID;
			static uint32_t MAX_INSTANCES, INSTANCE_DATA_LENGTH;
			static float* data;
			Loader* loader;

			void prepare();

			glm::mat4& getModelViewMatrix(const glm::vec3& position, const glm::vec3& scale, const float& rotation, const glm::mat4& view);

			void getModelViewMatrix(const glm::vec3& position, const glm::vec3& scale, const float& rotation, const glm::mat4& view, float* const data);

			void finish();
	};
}

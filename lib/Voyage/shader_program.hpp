#pragma once
#include <voyage.hpp>

namespace Voyage {
	class ShaderProgram {
		public:
			ShaderProgram(const char* const vertex_file, const char* const fragment_file, uint32_t num_textures = DEFAULT_NUM_TEXTURES) noexcept;

			ShaderProgram(const ShaderProgram& shader);

			ShaderProgram(ShaderProgram&& shader);

			~ShaderProgram();

			ShaderProgram& operator=(const ShaderProgram& shader);

			ShaderProgram& operator=(ShaderProgram&& shader);

			void start() const;

			void stop() const;

			const int getUniformLocation(const char* const name, const bool& store = true) const;

			void loadUniform(const char* const name, const float& value);

			void loadUniform(const char* const name, const int& value);

			void loadUniform(const char* const name, const uint32_t& value);

			void loadUniform(const char* const name, const bool& value);

			void loadUniform(const char* const name, const glm::vec2& value);

			void loadUniform(const char* const name, const glm::vec3& value);

			void loadUniform(const char* const name, const glm::vec4& value);

			void loadUniform(const char* const name, const float& x, const float& y);

			void loadUniform(const char* const name, const float& x, const float& y, const float& z);

			void loadUniform(const char* const name, const float& x, const float& y, const float& z, const float& w);

			void loadUniform(const char* const name, const glm::mat3& value, const bool& to_be_transposed = false);

			void loadUniform(const char* const name, const glm::mat4& value, const bool& to_be_transposed = false);

			bool remapTextureSampleName(const uint32_t& location, const char* const name) const;

			void dispose();
		private:
			uint32_t id, num_textures;
			mutable std::unordered_map<const char*, int> uniformLocations;
			static uint32_t DEFAULT_NUM_TEXTURES, MAX_TEXTURE_UNITS;

			static uint32_t loadShader(const char* const source, uint32_t type);

			void getAllTextureLocations();
		protected:
			mutable std::vector<int> location_textureUnits;
	};
}


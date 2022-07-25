#pragma once
#include <voyage.hpp>
#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>

namespace Cyclone {
	class ShaderProgram {
		public:
			ShaderProgram(const char* vertex_file, const char* fragment_file, unsigned int num_textures = 5) noexcept;

			ShaderProgram(const ShaderProgram& shader);

			ShaderProgram(ShaderProgram&& shader);

			ShaderProgram& operator=(const ShaderProgram& shader);

			ShaderProgram& operator=(ShaderProgram&& shader);

			void start() const;

			void stop() const;

			const int getUniformLocation(const char* name, const bool& store = true);

			void loadUniform(const char* name, const float& value);

			void loadUniform(const char* name, const int& value);

			void loadUniform(const char* name, const bool& value);

			void loadUniform(const char* name, const glm::vec3& value);

			void loadUniform(const char* name, const glm::vec4& value);

			void loadUniform(const char* name, const float& x, const float& y, const float& z);

			void loadUniform(const char* name, const float& x, const float& y, const float& z, const float& w);

			void loadUniform(const char* name, const glm::mat3& value);

			void loadUniform(const char* name, const glm::mat4& value);

			void dispose();
		private:
			unsigned int id, num_textures;
			std::unordered_map<const char*, int> uniformLocations;
			static unsigned int DEFAULT_NUM_TEXTURES;
			static int MAX_TEXTURE_UNITS;

			static unsigned int loadShader(const char* source, unsigned int type);

			void getAllTextureLocations();
		protected:
			std::vector<int> location_textureUnits;
	};
}


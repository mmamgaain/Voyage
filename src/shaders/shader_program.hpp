#pragma once

namespace Cyclone {
	class ShaderProgram {
		private:
			unsigned int id, num_textures;
			static unsigned int DEFAULT_NUM_TEXTURES;
			static int MAX_TEXTURE_UNITS;

			static unsigned int loadShader(const char* source, unsigned int type);
		public:
			ShaderProgram(const char* vertex_file, const char* fragment_file, unsigned int num_textures = 5) noexcept;

			ShaderProgram(const ShaderProgram& shader);

			ShaderProgram(ShaderProgram&& shader);

			ShaderProgram& operator=(const ShaderProgram& shader);

			ShaderProgram& operator=(ShaderProgram&& shader);

			void start();

			void stop();

			void dispose();
	};
}


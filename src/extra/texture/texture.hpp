#pragma once
#include <voyage.hpp>

namespace Voyage {
	typedef struct Texture {
		public:
			Texture(const char* filepath, const bool& flip_vertically = true, const float& anisotropic = 4.0, const int& type = GL_TEXTURE_2D);

			Texture(const std::vector<const char*>& filepaths, const std::vector<bool>& flip_vertically = {false, false, false, false, false, false}, const float& levelOfDetail = 0.0F, const float& anisotropic = 4.0);

			Texture(const Texture& texture);

			Texture(Texture&& texture);

			~Texture();

			const unsigned char* getBuffer() const;

			const unsigned int& getID() const;

			const int& getWidth() const;

			const int& getHeight() const;

			const int& getBPP() const;

			const char* getFilepath() const;

			const bool isFilepath(const char* filepath) const;

			const Texture& operator=(Texture&& texture);

			const Texture& operator=(const Texture& texture);

			void dispose();
		private:
			unsigned char* localBuffer;
			const char* filepath;
			int width, height, bpp, type;
			unsigned int id;
			static float MAX_ANISOTROPY_LEVEL;

	} Texture;

}


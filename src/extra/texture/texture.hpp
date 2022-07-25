#pragma once
#include <voyage.hpp>

namespace Cyclone {
	typedef struct Texture {
		public:
			Texture(const char* filepath, const bool& flip_vertically = true);

			Texture(const Texture& texture);

			Texture(Texture&& texture);

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
			int width, height, bpp;
			unsigned int id;

	} Texture;

}


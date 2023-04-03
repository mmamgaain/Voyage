#pragma once
#include <voyage.hpp>

namespace Voyage {
	typedef struct Texture {
		public:
			Texture(const char* const filepath, const bool& flip_vertically = true, const float& anisotropic = 4.0F, const int& type = GL_TEXTURE_2D) noexcept;

			Texture(const std::vector<const char*>& filepaths, const std::vector<bool>& flip_vertically = {false, false, false, false, false, false}, const float& levelOfDetail = 0.0F, const float& anisotropic = 4.0F) noexcept;

			Texture(const Texture& texture) noexcept;

			Texture(Texture&& texture) noexcept;

			~Texture() noexcept;

			const unsigned char* getBuffer() const;

			const uint32_t& getID() const;

			const int& getWidth() const;

			const int& getHeight() const;

			const int& getBPP() const;

			const char* getFilepath() const;

			const bool isFilepath(const char* filepath) const;

			const Texture& operator=(const Texture& lhs);

			const Texture& operator=(Texture&& rhs);

			void dispose();
		private:
			unsigned char* localBuffer;
			const char* filepath;
			int width, height, bpp, type;
			uint32_t id;
			static float MAX_ANISOTROPY_LEVEL;

	} Texture;

}


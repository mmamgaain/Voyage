#include "texture.hpp"

namespace Cyclone {
	Texture::Texture(const char* filepath, const bool& flip_vertically): localBuffer(nullptr), filepath(filepath), width(0), height(0), bpp(0) {
		stbi_set_flip_vertically_on_load(flip_vertically);
		localBuffer = stbi_load(filepath, &width, &height, &bpp, 4);
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	Texture::Texture(const Texture& texture) {
		this->localBuffer = texture.localBuffer;
		this->filepath = texture.filepath;
		this->width = texture.width;
		this->height = texture.height;
		this->bpp = texture.bpp;
		this->id = texture.id;
	}

	Texture::Texture(Texture&& texture) {
		this->localBuffer = texture.localBuffer;
		this->filepath = texture.filepath;
		this->width = texture.width;
		this->height = texture.height;
		this->bpp = texture.bpp;
		this->id = texture.id;
	}

	const unsigned char* Texture::getBuffer() const { return localBuffer; }

	const unsigned int& Texture::getID() const { return id; }

	const int& Texture::getWidth() const { return width; }

	const int& Texture::getHeight() const { return height; }

	const int& Texture::getBPP() const { return bpp; }

	const char* Texture::getFilepath() const { return filepath; }

	const bool Texture::isFilepath(const char* filepath) const { return strcmp(this->filepath, filepath); }

	const Texture& Texture::operator=(Texture&& texture) {
		if(this != &texture) {
			// delete this->localBuffer;
			stbi_image_free(localBuffer);
			delete this->filepath;

			this->localBuffer = texture.localBuffer;
			this->filepath = texture.filepath;
			this->width = texture.width;
			this->height = texture.height;
			this->bpp = texture.bpp;
			this->id = texture.id;

			texture.localBuffer = nullptr;
			texture.filepath = nullptr;
		}
		return *this;
	}

	const Texture& Texture::operator=(const Texture& texture) {
		if(this != &texture) {
			// delete this->localBuffer;
			stbi_image_free(localBuffer);
			delete this->filepath;

			this->localBuffer = texture.localBuffer;
			this->filepath = texture.filepath;
			this->width = texture.width;
			this->height = texture.height;
			this->bpp = texture.bpp;
			this->id = texture.id;
		}
		return *this;
	}

	void Texture::dispose() { glDeleteTextures(1, &id); stbi_image_free(localBuffer); }

}

#include "Voyage/texture.hpp"
#include "Voyage/maths.hpp"

namespace Voyage {
	float Texture::MAX_ANISOTROPY_LEVEL = 0.0;

	Texture::Texture(const char* filepath, const bool& flip_vertically, const float& anisotropic, const int& type): localBuffer(nullptr), filepath(filepath), width(0), height(0), bpp(0), type(type) {
		if(MAX_ANISOTROPY_LEVEL == 0) glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &MAX_ANISOTROPY_LEVEL);
		stbi_set_flip_vertically_on_load(flip_vertically);
		localBuffer = stbi_load(filepath, &width, &height, &bpp, 4);
		glGenTextures(1, &id);
		glBindTexture(type, id);
		glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(type, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glGenerateMipmap(type);
		glTexParameterf(type, GL_TEXTURE_MAX_ANISOTROPY,std::min(anisotropic, MAX_ANISOTROPY_LEVEL) );
		glTexImage2D(type, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);
		glBindTexture(type, 0);
	}

	Texture::Texture(const std::vector<const char*>& filepaths, const std::vector<bool>& flip_vertically, const float& levelOfDetail, const float& anisotropic): localBuffer(nullptr), filepath(filepaths[0]), width(0), height(0), bpp(0), type(GL_TEXTURE_CUBE_MAP) {
		if(MAX_ANISOTROPY_LEVEL == 0) glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &MAX_ANISOTROPY_LEVEL);
		// Out-of-bounds parameters check
		if(filepaths.size() != 6 && (flip_vertically.size() != 6 || flip_vertically.size() != 1)) { std::cerr << "ERROR: While creating cubemap texture." << std::endl; return; }
		glGenTextures(1, &id);
		glBindTexture(type, id);
		for(unsigned int i = 0; i < filepaths.size(); i++) {
			stbi_set_flip_vertically_on_load(flip_vertically[flip_vertically.size() == 1 ? 0 : i]);
			localBuffer = stbi_load(filepaths[i], &width, &height, &bpp, 4);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);
		}
		glGenerateMipmap(type);
		glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(type, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY, std::max(anisotropic, MAX_ANISOTROPY_LEVEL));
		glTextureParameterf(type, GL_TEXTURE_LOD_BIAS, levelOfDetail);
		glBindTexture(type, 0);
	}

	Texture::Texture(const Texture& texture) {
		this->localBuffer = texture.localBuffer;
		this->filepath = texture.filepath;
		this->width = texture.width;
		this->height = texture.height;
		this->bpp = texture.bpp;
		this->id = texture.id;
		this->type = texture.type;
	}

	Texture::Texture(Texture&& texture) {
		this->localBuffer = texture.localBuffer;
		this->filepath = texture.filepath;
		this->width = texture.width;
		this->height = texture.height;
		this->bpp = texture.bpp;
		this->id = texture.id;
		this->type = texture.type;
	}

	Texture::~Texture() { dispose(); }

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
			this->type = texture.type;

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
			this->type = texture.type;
		}
		return *this;
	}

	void Texture::dispose() {
		if(!localBuffer) return;
		glDeleteTextures(1, &id);
		stbi_image_free(localBuffer);
		localBuffer = nullptr;
	}

}

#include "Voyage/particle_texture.hpp"

namespace Voyage {
	ParticleTexture::ParticleTexture(const std::shared_ptr<Texture>& texture, const unsigned int& numberOfRows, const unsigned int& numberOfCols): texture(texture), numberOfRows(std::max<unsigned int>(numberOfRows, 1)), numberOfCols(numberOfCols ? std::max<unsigned int>(numberOfCols, 1) : numberOfRows) {}

	ParticleTexture::~ParticleTexture() noexcept {}

	const std::shared_ptr<Texture>& ParticleTexture::getTexture() const { return texture; }

	const unsigned int& ParticleTexture::getNumberOfRows() const { return numberOfRows; }

	const unsigned int& ParticleTexture::getNumberOfCols() const { return numberOfCols; }
}


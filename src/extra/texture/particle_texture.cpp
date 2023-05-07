#include "Voyage/particle_texture.hpp"

namespace Voyage {
	ParticleTexture::ParticleTexture(const std::shared_ptr<Texture>& texture, const uint32_t& numberOfRows, const uint32_t& numberOfCols): texture(texture), numberOfRows(std::max<uint32_t>(numberOfRows, 1)), numberOfCols(numberOfCols ? std::max<uint32_t>(numberOfCols, 1) : numberOfRows) {}

	ParticleTexture::ParticleTexture(const ParticleTexture& texture) noexcept: texture(texture.texture), numberOfCols(texture.numberOfCols), numberOfRows(texture.numberOfRows) {}

	ParticleTexture::ParticleTexture(ParticleTexture&& texture) noexcept: texture(std::move(texture.texture)), numberOfCols(texture.numberOfCols), numberOfRows(texture.numberOfRows) {}

	ParticleTexture& ParticleTexture::operator=(const ParticleTexture& lhs) noexcept {
		if(this == &lhs) return *this;
		texture = lhs.texture;
		numberOfCols = lhs.numberOfCols;
		numberOfRows = lhs.numberOfRows;
		return *this;
	}

	ParticleTexture& ParticleTexture::operator=(ParticleTexture&& rhs) noexcept {
		if(this == &rhs) return *this;
		texture = std::move(rhs.texture);
		numberOfCols = rhs.numberOfCols;
		numberOfRows = rhs.numberOfRows;
		return *this;
	}

	ParticleTexture::~ParticleTexture() noexcept {}

	const std::shared_ptr<Texture>& ParticleTexture::getTexture() const { return texture; }

	const uint32_t& ParticleTexture::getNumberOfRows() const { return numberOfRows; }

	const uint32_t& ParticleTexture::getNumberOfCols() const { return numberOfCols; }
}


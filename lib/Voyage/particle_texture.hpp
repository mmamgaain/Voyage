#pragma once
#include <voyage.hpp>
#include "Voyage/texture.hpp"

namespace Voyage {
	typedef struct ParticleTexture {
		public:
			ParticleTexture(const std::shared_ptr<Texture>& texture, const uint32_t& numberOfRows = 1, const uint32_t& numberOfCols = 0);

			ParticleTexture(const ParticleTexture& texture) noexcept;

			ParticleTexture(ParticleTexture&& texture) noexcept;

			ParticleTexture& operator=(const ParticleTexture& lhs) noexcept;

			ParticleTexture& operator=(ParticleTexture&& rhs) noexcept;

			~ParticleTexture() noexcept;

			const std::shared_ptr<Texture>& getTexture() const;

			const uint32_t& getNumberOfRows() const;

			const uint32_t& getNumberOfCols() const;
		private:
			std::shared_ptr<Texture> texture;
			uint32_t numberOfRows, numberOfCols;

	} ParticleTexture;
}

#pragma once
#include <voyage.hpp>
#include "Voyage/texture.hpp"

namespace Voyage {
	typedef struct ParticleTexture {
		public:
			ParticleTexture(const std::shared_ptr<Texture>& texture, const unsigned int& numberOfRows = 1, const unsigned int& numberOfCols = 0);

			~ParticleTexture() noexcept;

			const std::shared_ptr<Texture>& getTexture() const;

			const unsigned int& getNumberOfRows() const;

			const unsigned int& getNumberOfCols() const;
		private:
			std::shared_ptr<Texture> texture;
			unsigned int numberOfRows, numberOfCols;

	} ParticleTexture;
}

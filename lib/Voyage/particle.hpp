#pragma once
#include <voyage.hpp>
#include "Voyage/particle_texture.hpp"
#include "Voyage/texture.hpp"

namespace Voyage {
	typedef struct ParticleProperties {
		glm::vec3 position, velocity, scale;
		float gravityEffect, lifeLength, rotation, elapsedTime;
		ParticleTexture* texture;

		ParticleProperties(const glm::vec3& position, const glm::vec3& velocity = {}, const glm::vec3& scale = {1.0F, 1.0F, 1.0F}, const float& gravityEffect = 1.0, const float& lifeLength = 1.0, const float& rotation = 0.0, ParticleTexture* texture = nullptr);

		~ParticleProperties() noexcept = default;
	} ParticleProperties;

	class Particle {
		public:
			Particle(const ParticleProperties& props);

			~Particle() noexcept = default;

			const bool update();

			void setPosition(const glm::vec3& position);

			void setPosition(const float& x, const float& y, const float& z);

			const glm::vec3& getPosition() const;

			void setVelocity(const glm::vec3& velocity);

			void setVelocity(const float& x, const float& y, const float& z);

			const glm::vec3& getVelocity() const;

			void setScale(const glm::vec3& scale);

			void setScale(const float& x, const float& y, const float& z);

			const glm::vec3& getScale() const;

			const ParticleTexture* getTexture() const;

			const float& getRotation() const;
		private:
			ParticleProperties props;
	};

	class ParticleInstanced {
		public:
			glm::vec3 position, velocity, scale;
			float gravityEffect, lifeLength, rotation, elapsedTime;
			mutable glm::vec2 texOffsetCurr, texOffsetNext;
			mutable uint32_t numTexRows, numTexCols;
			mutable float blend;
			bool is_alive;

			ParticleInstanced() noexcept;

			ParticleInstanced(glm::vec3 position, const glm::vec3& velocity, const glm::vec3& scale, const float& gravityEffect, const float& lifeLength, const float& rotation, const uint32_t& numTexRows = 1, const uint32_t& numTexCols = 1);

			ParticleInstanced(const ParticleInstanced& particle) noexcept;

			// ParticleInstanced(ParticleInstanced&& particle) noexcept;

			ParticleInstanced& operator=(const ParticleInstanced& other) noexcept;

			// ParticleInstanced& operator=(ParticleInstanced&& other) noexcept;

			~ParticleInstanced() noexcept = default;

			void set(glm::vec3 position, glm::vec3 velocity, glm::vec3 scale, float gravityEffect, float lifeLength, float rotation, uint32_t numTexRows, uint32_t numTexCols);

			bool update();
		private:
			void updateTexCoords();

			void setTextureOffset(glm::vec2& offset, int index) const;
	};

}

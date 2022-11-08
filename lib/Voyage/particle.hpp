#include <voyage.hpp>
#include "Voyage/material.hpp"

namespace Voyage {
	class Particle {
		public:
			Particle(const glm::vec3& position, const glm::vec3& velocity = {}, const glm::vec3& scale = {1.0F, 1.0F, 1.0F}, const float& gravityEffect = 1.0, const float& lifeLength = 1.0, const float& rotation = 0.0);

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

			void setMaterial(const Material& material);

			const Material& getMaterial() const;

			const float& getRotation() const;
		private:
			glm::vec3 position, velocity, scale;
			float gravityEffect, lifeLength, rotation, elapsedTime;
			Material material;
	};
}

#pragma once
#include <voyage.hpp>
#include "Voyage/particle_texture.hpp"
#include "loader.hpp"

namespace Voyage {
	class ParticleSystem {
		public:
			ParticleSystem(const char* const vertex_file, const char* const fragment_file, Loader& loader, const float& pps, const float& speed, const float& lifeLength, const ParticleTexture& texture, const float& gravityComplient = 1.0F, const float& scale = 1.0F);

			ParticleSystem(const char* const vertex_file, const char* const fragment_file, Loader& loader, const float& pps, const float& speed, const float& lifeLength, ParticleTexture&& texture, const float& gravityComplient = 1.0F, const float& scale = 1.0F);

			~ParticleSystem() noexcept = default;

			void setDirection(const glm::vec3& direction, const float& deviation);

			void randomizeRotation(const bool& randomize = true);

			void setSpeedError(const float& speedError);

			void setLifeError(const float& lifeError);

			void setScaleError(const float& scaleError);

			void generateParticles(const glm::vec3& systemCenter);

			const glm::vec3 generateRandomUnitVectorWithinCone(const glm::vec3& coneDirection, const float& angle) const;
		private:
			float pps, averageSpeed, gravityComplient, averageLifeLength, averageScale, speedError, lifeError, scaleError, directionDeviation;
			bool randomRotation;
			glm::vec3 direction;
			ParticleTexture texture;

			void emitParticles(const glm::vec3& center, const int& count);

			const float generateValue(const float& average, const float& errorMargin) const;

			const float generateRotation() const;

			const glm::vec3 generateRandomUnitVector() const;
	};

}

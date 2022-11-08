#include <voyage.hpp>
#include "loader.hpp"

namespace Voyage {
	class ParticleSystem {
		public:
			ParticleSystem(const char* const vertex_file, const char* const fragment_file, Loader& loader, const glm::mat4& projection, const float& pps, const float& speed, const float& lifeLength, const float& gravityComplient = 9.8F, const float& scale = 1.0F);

			~ParticleSystem() noexcept;

			void setDirection(const glm::vec3& direction, const float& deviation);

			void randomizeRotation(const bool& randomize = true);

			void setSpeedError(const float& speedError);

			void setLifeError(const float& lifeError);

			void setScaleError(const float& scaleError);

			void generateParticles(const glm::vec3& systemCenter);
		private:
			float pps, averageSpeed, gravityComplient, averageLifeLength, averageScale, speedError, lifeError, scaleError, directionDeviation;
			bool randomRotation;
			glm::vec3 direction;

			void emitParticles(const glm::vec3& center);

			const float generateValue(const float& average, const float& errorMargin) const;

			const float generateRotation() const;

			const glm::vec3 generateRandomUnitVectorWithinCone(const glm::vec3& coneDirection, const float& angle) const;

			const glm::vec3 generateRandomUnitVector() const;

			static const float getRandom();
	};

}

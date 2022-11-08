#define _USE_MATH_DEFINES
#include "Voyage/particle_system.hpp"
#include "Voyage/particle_master.hpp"
#include <cmath>
#include "Voyage/core.hpp"
#include <time.h>

namespace Voyage {
	ParticleSystem::ParticleSystem(const char* const vertex_file, const char* const fragment_file, Loader& loader, const glm::mat4& projection, const float& pps, const float& speed, const float& lifeLength, const float& gravityComplient, const float& scale): pps(pps), averageSpeed(speed), gravityComplient(gravityComplient), averageScale(scale), speedError(0.0F), lifeError(0.0F), scaleError(0.0F), directionDeviation(0.0F), randomRotation(false), direction({0.0F, 1.0F, 0.0F}) { srand(time(0)); ParticleMaster::init(vertex_file, fragment_file, loader, projection); }

	ParticleSystem::~ParticleSystem() noexcept {
		ParticleMaster::dispose();
	}

	void ParticleSystem::setDirection(const glm::vec3& direction, const float& deviation) { this->direction = direction; directionDeviation = deviation; }

	void ParticleSystem::randomizeRotation(const bool& randomize) { randomRotation = true; }

	void ParticleSystem::setSpeedError(const float& speedError) { this->speedError = speedError; }

	void ParticleSystem::setLifeError(const float& lifeError) { this->lifeError = lifeError; }

	void ParticleSystem::setScaleError(const float& scaleError) { this->scaleError = scaleError; }

	void ParticleSystem::generateParticles(const glm::vec3& systemCenter) {
		float particlesToCreate = pps * Core::deltaTime;
		int count = (int) floor(particlesToCreate);
		float partialParticles = particlesToCreate - count;
		for(unsigned int i = 0; i < count; i++) emitParticles(systemCenter);
		if(getRandom() < partialParticles) emitParticles(systemCenter);
	}

	void ParticleSystem::emitParticles(const glm::vec3& center) {
		glm::vec3 velocity = generateRandomUnitVectorWithinCone(direction, directionDeviation);
		velocity = glm::normalize(velocity);
		velocity *= generateValue(averageSpeed, speedError);
		float scale = generateValue(averageScale, scaleError), lifeLength = generateValue(averageLifeLength, lifeError);
		ParticleMaster::addParticle(center, velocity, {scale, scale, scale}, gravityComplient, lifeLength, generateRotation());
	}

	const float ParticleSystem::generateValue(const float& average, const float& errorMargin) const { return average + ((rand() - 0.5F) * 2.0F * errorMargin); }

	const float ParticleSystem::generateRotation() const { if(randomRotation) return getRandom(); else return 0.0F;  }

	const glm::vec3 ParticleSystem::generateRandomUnitVectorWithinCone(const glm::vec3& coneDirection, const float& angle) const {
		float cosAngle = std::cos(angle);
		float theta = getRandom() * 2 * M_PI;
		float z = cosAngle + (getRandom() * (1 - cosAngle));
		float rootOneMinusZSquared = std::sqrt(1 - z * z),
			  x = rootOneMinusZSquared * cos(theta),
			  y = rootOneMinusZSquared * sin(theta);
		glm::vec4 direction = {x, y, z, 1};
		if(coneDirection.x != 0 || coneDirection.y != 0 || (coneDirection.z != 1 && coneDirection.z != -1)) {
			glm::vec3 rotateAxis = glm::cross(coneDirection, {0, 0, 1});
			rotateAxis = glm::normalize(rotateAxis);
			float rotateAngle = glm::cos(glm::dot(coneDirection, {0, 0, 1}));
			glm::mat4 rotationMatrix;
			rotationMatrix = glm::rotate(rotationMatrix, -rotateAngle, rotateAxis);
			direction = rotationMatrix * direction;
		}
		else if(coneDirection.z == -1) direction.z *= -1;
		return direction;
	}

	const glm::vec3 ParticleSystem::generateRandomUnitVector() const {
		float theta = (float) getRandom() * 2 * M_PI;
		float z = getRandom() * 2 - 1;
		float rootOneMinusZSquared = std::sqrt(1 - z * z);
		float x = rootOneMinusZSquared * std::cos(theta), y = rootOneMinusZSquared * std::sin(theta);
		return {x, y, z};
	}

	const float ParticleSystem::getRandom() { return rand() / (float)RAND_MAX; }
}

#include "Voyage/particle.hpp"
#include "Voyage/core.hpp"

namespace Voyage {
	Particle::Particle(const glm::vec3& position, const glm::vec3& velocity, const glm::vec3& scale, const float& gravityEffect, const float& lifeLength, const float& rotation): position(position), velocity(velocity), scale(scale), gravityEffect(gravityEffect), lifeLength(lifeLength), rotation(rotation), elapsedTime(0.0) {}

	const bool Particle::update() {
		glm::vec3 GRAVITY(0, 9.2, 0);
		velocity.y = GRAVITY.y * gravityEffect * Core::deltaTime;
		position += velocity * (float)Core::deltaTime;
		elapsedTime += Core::deltaTime;
		return elapsedTime < lifeLength;
	}

	void Particle::setPosition(const glm::vec3& position) { this->position = std::move(position); }

	void Particle::setPosition(const float& x, const float& y, const float& z) { position = {x, y, z}; }

	const glm::vec3& Particle::getPosition() const { return position; }

	void Particle::setVelocity(const glm::vec3& velocity) { this->velocity = std::move(velocity); }

	void Particle::setVelocity(const float& x, const float& y, const float& z) { velocity = {x, y, z}; }

	const glm::vec3& Particle::getVelocity() const { return velocity; }

	void Particle::setScale(const glm::vec3& scale) { this->scale = std::move(scale); }

	void Particle::setScale(const float& x, const float& y, const float& z) { scale = {x, y, z}; }

	const glm::vec3& Particle::getScale() const { return scale; }

	void Particle::setMaterial(const Material& material) { this->material = std::move(material); }

	const float& Particle::getRotation() const { return rotation; }

	const Material& Particle::getMaterial() const { return material; }
}

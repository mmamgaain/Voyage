#include "Voyage/particle.hpp"
#include "Voyage/core.hpp"
#include "Voyage/particle_texture.hpp"
#include "Voyage/raw_model.hpp"
#include "Voyage/texture.hpp"
#include "Voyage/maths.hpp"
#include "glm/ext/vector_float2.hpp"
#include <cmath>

namespace Voyage {
	ParticleProperties::ParticleProperties(const glm::vec3& position, const glm::vec3& velocity, const glm::vec3& scale, const float& gravityEffect, const float& lifeLength, const float& rotation, ParticleTexture* texture): position(position), velocity(velocity), scale(scale), gravityEffect(gravityEffect), lifeLength(lifeLength), rotation(rotation), elapsedTime(0.0), texture(texture)  {}

	Particle::Particle(const ParticleProperties& props): props(props) {}

	const bool Particle::update() {
		props.velocity.y = GRAVITY_2D.y * props.gravityEffect * Core::deltaTime;
		props.position += props.velocity * (float)Core::deltaTime;
		props.elapsedTime += Core::deltaTime;
		return props.elapsedTime < props.lifeLength;
	}

	void Particle::setPosition(const glm::vec3& position) { this->props.position = std::move(position); }

	void Particle::setPosition(const float& x, const float& y, const float& z) { props.position = {x, y, z}; }

	const glm::vec3& Particle::getPosition() const { return props.position; }

	void Particle::setVelocity(const glm::vec3& velocity) { this->props.velocity = std::move(velocity); }

	void Particle::setVelocity(const float& x, const float& y, const float& z) { props.velocity = {x, y, z}; }

	const glm::vec3& Particle::getVelocity() const { return props.velocity; }

	void Particle::setScale(const glm::vec3& scale) { this->props.scale = scale; }

	void Particle::setScale(const float& x, const float& y, const float& z) { props.scale = {x, y, z}; }

	const glm::vec3& Particle::getScale() const { return props.scale; }

	const float& Particle::getRotation() const { return props.rotation; }

	const ParticleTexture* Particle::getTexture() const { return props.texture; }

	ParticleInstanced::ParticleInstanced(glm::vec3 position, const glm::vec3& velocity, const glm::vec3& scale, const float& gravityEffect, const float& lifeLength, const float& rotation, const unsigned int& numTexRows, const unsigned int& numTexCols): position(position), velocity(velocity), scale(scale), gravityEffect(gravityEffect), lifeLength(lifeLength), rotation(rotation), elapsedTime(0), texOffsetCurr(0.0F), texOffsetNext(0.0F), numTexRows(numTexRows), numTexCols(numTexCols) {}

	const bool ParticleInstanced::update() {
		velocity.y -= GRAVITY_2D.y * gravityEffect * Core::deltaTime;
		position += velocity * (float)Core::deltaTime;
		updateTexCoords();
		elapsedTime += Core::deltaTime;
		return elapsedTime < lifeLength;
	}

	void ParticleInstanced::updateTexCoords() {
		float lifeFactor = elapsedTime / lifeLength;
		unsigned int stageCount = numTexRows * numTexCols;
		float atlasProgression = lifeFactor * stageCount;
		int indexCurr = (int)std::floor(atlasProgression),
			indexNext = indexCurr < stageCount - 1 ? indexCurr + 1 : indexCurr;
		blend = std::fmod(atlasProgression, 1);
		setTextureOffset(texOffsetCurr, indexCurr);
		setTextureOffset(texOffsetNext, indexNext);
	}

	void ParticleInstanced::setTextureOffset(glm::vec2& offset, const int& index) {
		int column = index % numTexCols,
			row = index / numTexCols;
		offset.x = (float)column / numTexCols;
		offset.y = (float)row / numTexCols;
	}
}

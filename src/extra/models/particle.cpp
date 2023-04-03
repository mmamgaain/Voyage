#include "Voyage/particle.hpp"
#include "Voyage/core.hpp"
#include "Voyage/particle_texture.hpp"
#include "Voyage/raw_model.hpp"
#include "Voyage/texture.hpp"
#include "Voyage/maths.hpp"

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

	ParticleInstanced::ParticleInstanced() noexcept: position({}), velocity({}), scale({}), gravityEffect(0.0F), lifeLength(0.0F), rotation(0.0F), elapsedTime(0), texOffsetCurr(0.0F), texOffsetNext(0.0F), numTexRows(0), numTexCols(0), is_alive(false) {}

	ParticleInstanced::ParticleInstanced(glm::vec3 position, const glm::vec3& velocity, const glm::vec3& scale, const float& gravityEffect, const float& lifeLength, const float& rotation, const uint32_t& numTexRows, const uint32_t& numTexCols): position(position), velocity(velocity), scale(scale), gravityEffect(gravityEffect), lifeLength(lifeLength), rotation(rotation), elapsedTime(0), texOffsetCurr(0.0F), texOffsetNext(0.0F), numTexRows(numTexRows), numTexCols(numTexCols), is_alive(true) {}

	ParticleInstanced::ParticleInstanced(const ParticleInstanced& particle) noexcept: position(particle.position), velocity(particle.velocity), scale(particle.scale), gravityEffect(particle.gravityEffect), lifeLength(particle.lifeLength), rotation(particle.rotation), elapsedTime(particle.elapsedTime), texOffsetCurr(particle.texOffsetCurr), texOffsetNext(particle.texOffsetNext), numTexRows(particle.numTexRows), numTexCols(particle.numTexCols), blend(particle.blend), is_alive(true) {}

	// ParticleInstanced::ParticleInstanced(ParticleInstanced&& particle) noexcept {}

	ParticleInstanced& ParticleInstanced::operator=(const ParticleInstanced& other) noexcept {
		if(this == &other) return *this;
		position = other.position;
		velocity = other.velocity;
		scale = other.scale;
		gravityEffect = other.gravityEffect;
		lifeLength = other.lifeLength;
		rotation = other.rotation;
		elapsedTime = other.elapsedTime;
		texOffsetCurr = other.texOffsetCurr;
		texOffsetNext = other.texOffsetNext;
		numTexRows = other.numTexRows;
		numTexCols = other.numTexCols;
		blend = other.blend;
		is_alive = other.is_alive;

		return *this;
	}

	void ParticleInstanced::set(glm::vec3 position, const glm::vec3& velocity, const glm::vec3& scale, const float& gravityEffect, const float& lifeLength, const float& rotation, const uint32_t& numTexRows, const uint32_t& numTexCols) {
		this->position = position;
		this->velocity = velocity;
		this->scale = scale;
		this->gravityEffect = gravityEffect;
		this->lifeLength = lifeLength;
		this->rotation = rotation;
		this->elapsedTime = 0;
		this->texOffsetCurr = glm::vec2();
		this->texOffsetNext = glm::vec2();
		this->numTexRows = numTexRows;
		this->numTexCols = numTexCols;
		blend = 0;
		is_alive = true;
	}

	bool ParticleInstanced::update() {
		if(!is_alive) return is_alive;
		velocity.y -= GRAVITY_2D.y * gravityEffect * Core::deltaTime;
		position += velocity * (float)Core::deltaTime;
		if(numTexCols > 1 || numTexRows > 1) updateTexCoords();
		elapsedTime += Core::deltaTime;
		is_alive = elapsedTime < lifeLength;
		return is_alive;
	}

	void ParticleInstanced::updateTexCoords() {
		float lifeFactor = elapsedTime / lifeLength;
		uint32_t stageCount = numTexRows * numTexCols;
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

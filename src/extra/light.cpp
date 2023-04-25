#include "Voyage/light.hpp"

namespace Voyage {
	DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 color, glm::vec3 attenuation): direction(direction), color(color), attenuation(attenuation) {}

	DirectionalLight::DirectionalLight(const DirectionalLight& light) noexcept: direction(light.direction), color(light.color), attenuation(light.attenuation) {}

	const glm::vec3& DirectionalLight::getDirection() const { return direction; }

	glm::vec3& DirectionalLight::getDirection() { return direction; }

	const glm::vec3& DirectionalLight::getColor() const { return color; }

	const glm::vec3& DirectionalLight::getAttenuation() const { return attenuation; }

	DirectionalLight& DirectionalLight::operator=(const DirectionalLight& other) {
		if(this == &other) return *this;
		direction = other.direction;
		color = other.color;
		attenuation = other.attenuation;
		return *this;
	}

	PointLight::PointLight(glm::vec3 position, glm::vec3 color, glm::vec3 attenuation): position(position), color(color), attenuation(attenuation) {}

	PointLight::PointLight(const PointLight& light) noexcept: position(light.position), color(light.color), attenuation(light.attenuation) {}

	const glm::vec3& PointLight::getPosition() const { return position; }

	const glm::vec3& PointLight::getColor() const { return color; }

	const glm::vec3& PointLight::getAttenuation() const { return attenuation; }

	PointLight& PointLight::operator=(const PointLight& other) {
		if(this == &other) return *this;
		position = other.position;
		color = other.color;
		attenuation = other.attenuation;
		return *this;
	}


	SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 color, glm::vec3 attenuation): position(position), direction(direction), color(color), attenuation(attenuation) {}

	SpotLight::SpotLight(const SpotLight& light) noexcept: position(light.position), direction(light.direction), color(light.color), attenuation(light.attenuation) {}

	const glm::vec3& SpotLight::getPosition() { return position; }

	const glm::vec3& SpotLight::getDirection() { return direction; }

	const glm::vec3& SpotLight::getColor() { return color; }

	const glm::vec3& SpotLight::getAttenuation() { return attenuation; }

	SpotLight& SpotLight::operator=(const SpotLight& other) {
		if(this == &other) return *this;
		position = other.position;
		direction = other.direction;
		color = other.color;
		attenuation = other.attenuation;
		return *this;
	}
}

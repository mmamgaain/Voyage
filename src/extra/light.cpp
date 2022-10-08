#include "light.hpp"

namespace Voyage {
	DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 color, glm::vec3 attenuation): direction(direction), color(color), attenuation(attenuation) {}

	DirectionalLight::DirectionalLight(const DirectionalLight& light): direction(light.direction), color(light.color), attenuation(light.attenuation) {}

	const glm::vec3& DirectionalLight::getDirection() const { return direction; }

	const glm::vec3& DirectionalLight::getColor() const { return color; }

	const glm::vec3& DirectionalLight::getAttenuation() const { return attenuation; }

	const DirectionalLight& DirectionalLight::operator=(const DirectionalLight& light) {
		direction = light.direction;
		color = light.color;
		attenuation = light.attenuation;
		return *this;
	}


	PointLight::PointLight(glm::vec3 position, glm::vec3 color, glm::vec3 attenuation): position(position), color(color), attenuation(attenuation) {}

	PointLight::PointLight(const PointLight& light): position(light.position), color(light.color), attenuation(light.attenuation) {}

	const glm::vec3& PointLight::getPosition() const { return position; }

	const glm::vec3& PointLight::getColor() const { return color; }

	const glm::vec3& PointLight::getAttenuation() const { return attenuation; }

	const PointLight& PointLight::operator=(const PointLight& light) {
		position = light.position;
		color = light.color;
		attenuation = light.attenuation;
		return *this;
	}


	SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 color, glm::vec3 attenuation): position(position), direction(direction), color(color), attenuation(attenuation) {}

	SpotLight::SpotLight(const SpotLight& light): position(light.position), direction(light.direction), color(light.color), attenuation(light.attenuation) {}

	const glm::vec3& SpotLight::getPosition() { return position; }

	const glm::vec3& SpotLight::getDirection() { return direction; }

	const glm::vec3& SpotLight::getColor() { return color; }

	const glm::vec3& SpotLight::getAttenuation() { return attenuation; }

	const SpotLight& SpotLight::operator=(const SpotLight& light) {
		position = light.position;
		direction = light.direction;
		color = light.color;
		attenuation = light.attenuation;
		return *this;
	}
}

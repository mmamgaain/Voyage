#pragma once
#include <voyage.hpp>

namespace Voyage {
	class DirectionalLight {
		public:
			DirectionalLight(glm::vec3 direction, glm::vec3 color = {1.0, 1.0, 1.0}, glm::vec3 attenuation = {1.0, 0.0, 0.0});

			DirectionalLight(const DirectionalLight& light);

			~DirectionalLight() = default;

			const glm::vec3& getDirection() const;

			const glm::vec3& getColor() const;

			const glm::vec3& getAttenuation() const;

			const DirectionalLight& operator=(const DirectionalLight& light);
		private:
			glm::vec3 direction, color, attenuation;
	};

	class PointLight {
		public:
			PointLight(glm::vec3 position, glm::vec3 color, glm::vec3 attenuation);

			PointLight(const PointLight& light);

			~PointLight() = default;

			const glm::vec3& getPosition() const;

			const glm::vec3& getColor() const;

			const glm::vec3& getAttenuation() const;

			const PointLight& operator=(const PointLight& light);
		private:
			glm::vec3 position, color, attenuation;
	};

	class SpotLight {
		public:
			SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 color, glm::vec3 attenuation);

			SpotLight(const SpotLight& light);

			~SpotLight() = default;

			const glm::vec3& getPosition();

			const glm::vec3& getDirection();

			const glm::vec3& getColor();

			const glm::vec3& getAttenuation();

			const SpotLight& operator=(const SpotLight& light);
		private:
			glm::vec3 position, direction, color, attenuation;
	};
}

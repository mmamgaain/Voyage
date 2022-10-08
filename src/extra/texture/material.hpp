#pragma once
#include <voyage.hpp>
#include "texture.hpp"

namespace Voyage {
	typedef struct Material {
		public:
			float shineDamper = 1, specularReflectivity = 0, transparency = 0, enviroRefractivity = 0.5, fresnelPower = 2;
			bool isDoubleSided = false, hasFresnel = true;

			Material(const glm::vec3& diffuse = {0, 0, 0}, const glm::vec3& ambient = {0, 0, 0}, const glm::vec3& specular = {0, 0, 0}, Texture* texture = 0);

			Material(const Material& material);

			Material(Material&& material);

			~Material() noexcept = default;

			const glm::vec3& getDiffuseColor() const;

			const glm::vec3& getAmbientColor() const;

			const glm::vec3& getSpecularColor() const;

			const glm::vec2& getRows() const;

			Texture* getTexture() const;

			Texture* getNormalMap() const;

			Texture* getSpecularMap() const;

			void setDiffuseColor(const glm::vec3& diffuse);

			void setAmbientColor(const glm::vec3& ambient);

			void setSpecularColor(const glm::vec3& specular);

			void setRows(const glm::vec2& rows);

			void setTexture(Texture* const texture);

			void setNormalMap(Texture* const normal);

			void setSpecularMap(Texture* const specular);

			const Material& operator=(const Material& material);

			const Material& operator=(Material&& material);

			void dispose();
		private:
			static const unsigned int NO_TEXTURE = 0;
			glm::vec3 diffuse, ambient, specular;
			Texture *texture, *normalMap, *specularMap;
			glm::vec2 rows;
	} Material;
}

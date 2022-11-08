#pragma once
#include <voyage.hpp>
#include "Voyage/texture.hpp"

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

			Texture* const getTexture() const;

			Texture* const getNormalMap() const;

			Texture* const getSpecularMap() const;

			void setDiffuseColor(const glm::vec3& diffuse);

			void setAmbientColor(const glm::vec3& ambient);

			void setSpecularColor(const glm::vec3& specular);

			void setDiffuseColor(const aiColor3D& diffuse);

			void setAmbientColor(const aiColor3D& ambient);

			void setSpecularColor(const aiColor3D& specular);

			void setRows(const glm::vec2& rows);

			void setTexture(Texture* const texture);

			void setNormalMap(Texture* const normal);

			void setSpecularMap(Texture* const specular);

			const bool hasTexture() const;

			const bool hasNormalMap() const;

			const bool hasSpecularMap() const;

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

#pragma once
#include <memory>
#include <voyage.hpp>
#include "Voyage/texture.hpp"

namespace Voyage {
	typedef struct Material {
		public:
			float shineDamper = 1, specularReflectivity = 0, transparency = 0, enviroRefractivity = 0.5, fresnelPower = 2;
			bool isDoubleSided = false, hasFresnel = true;

			Material(const glm::vec3& diffuse = {0, 0, 0}, const glm::vec3& ambient = {0, 0, 0}, const glm::vec3& specular = {0, 0, 0}, std::shared_ptr<Texture> texture = 0);

			Material(const Material& material);

			Material(Material&& material);

			~Material() noexcept = default;

			const glm::vec3& getDiffuseColor() const;

			const glm::vec3& getAmbientColor() const;

			const glm::vec3& getSpecularColor() const;

			const glm::vec2& getRows() const;

			const Texture& getTexture() const;

			const Texture& getNormalMap() const;

			const Texture& getSpecularMap() const;

			void setDiffuseColor(const glm::vec3& diffuse);

			void setAmbientColor(const glm::vec3& ambient);

			void setSpecularColor(const glm::vec3& specular);

			void setDiffuseColor(const aiColor3D& diffuse);

			void setAmbientColor(const aiColor3D& ambient);

			void setSpecularColor(const aiColor3D& specular);

			void setRows(const glm::vec2& rows);

			void setTexture(std::shared_ptr<Texture> const texture);

			void setNormalMap(std::shared_ptr<Texture> const normal);

			void setSpecularMap(std::shared_ptr<Texture> const specular);

			const bool hasTexture() const;

			const bool hasNormalMap() const;

			const bool hasSpecularMap() const;

			const Material& operator=(const Material& material);

			const Material& operator=(Material&& material);
		private:
			static const uint32_t NO_TEXTURE = 0;
			glm::vec3 diffuse, ambient, specular;
			std::shared_ptr<Texture> texture, normalMap, specularMap;
			glm::vec2 rows;
	} Material;
}

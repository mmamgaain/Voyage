#include "Voyage/material.hpp"
#include "assimp/types.h"

namespace Voyage {

	Material::Material(const glm::vec3& diffuse, const glm::vec3& ambient, const glm::vec3& specular, std::shared_ptr<Texture> texture): diffuse(diffuse), ambient(ambient), specular(specular), texture(texture), normalMap(0), specularMap(0), shineDamper(1), specularReflectivity(0), transparency(0), enviroRefractivity(0.5), fresnelPower(2), isDoubleSided(false), hasFresnel(true), rows({}) {}

	Material::Material(const Material& material): diffuse(material.diffuse), ambient(material.ambient), specular(material.specular), texture(material.texture), normalMap(material.normalMap), specularMap(material.specularMap), shineDamper(1), specularReflectivity(0), transparency(0), enviroRefractivity(0.5), fresnelPower(2), isDoubleSided(false), hasFresnel(true), rows({}) {}

	Material::Material(Material&& material): diffuse(std::move(material.diffuse)), ambient(std::move(material.ambient)), specular(std::move(material.specular)), texture(std::move(material.texture)), normalMap(std::move(material.normalMap)), specularMap(std::move(material.specularMap)), shineDamper(1), specularReflectivity(0), transparency(0), enviroRefractivity(0.5), fresnelPower(2), isDoubleSided(false), hasFresnel(true), rows({}) {}

	const glm::vec3& Material::getDiffuseColor() const { return diffuse; }

	const glm::vec3& Material::getAmbientColor() const { return ambient; }

	const glm::vec3& Material::getSpecularColor() const { return specular; }

	const glm::vec2& Material::getRows() const { return rows; }

	const Texture& Material::getTexture() const { return *texture; }

	const Texture& Material::getNormalMap() const { return *normalMap; }

	const Texture& Material::getSpecularMap() const { return *specularMap; }

	void Material::setDiffuseColor(const glm::vec3& diffuse) { this->diffuse = diffuse; }

	void Material::setAmbientColor(const glm::vec3& ambient) { this->ambient = ambient; }

	void Material::setSpecularColor(const glm::vec3& specular) { this->specular = specular; }

	void Material::setDiffuseColor(const aiColor3D& diffuse) { this->diffuse[0] = diffuse.r; this->diffuse[1] = diffuse.g; this->diffuse[2] = diffuse.b; }

	void Material::setAmbientColor(const aiColor3D& ambient) { this->ambient[0] = ambient.r; this->ambient[1] = ambient.g; this->ambient[2] = ambient.b; }

	void Material::setSpecularColor(const aiColor3D& specular) { this->specular[0] = specular.r; this->specular[1] = specular.g; this->specular[2] = specular.b; }

	void Material::setRows(const glm::vec2& rows) { this->rows = rows; }

	void Material::setTexture(std::shared_ptr<Texture> const texture) { this->texture = texture; }

	void Material::setNormalMap(std::shared_ptr<Texture> const normal) { this->normalMap = normal; }

	void Material::setSpecularMap(std::shared_ptr<Texture> const specular) { this->specularMap = specular; }

	const bool Material::hasTexture() const { return texture != nullptr && texture->getID(); }

	const bool Material::hasNormalMap() const { return normalMap != nullptr && normalMap->getID(); }

	const bool Material::hasSpecularMap() const { return specularMap != nullptr && specularMap->getID(); }

	const Material& Material::operator=(const Material& material) {
		if(this != &material) {
			this->dispose();

			diffuse = material.diffuse;
			ambient = material.ambient;
			specular = material.specular;
			texture = material.texture;
			normalMap = material.normalMap;
			specularMap = material.specularMap;
			rows = material.rows;
		}
		return *this;
	}

	const Material& Material::operator=(Material&& material) {
		if(this != &material) {
			this->dispose();

			diffuse = std::move(material.diffuse);
			ambient = std::move(material.ambient);
			specular = std::move(material.specular);
			texture = std::move(material.texture);
			normalMap = std::move(material.normalMap);
			specularMap = std::move(material.specularMap);
			rows = material.rows;

			// material.dispose();
		}
		return *this;
	}

	void Material::dispose() { if(texture) texture->dispose(); if(normalMap) normalMap->dispose(); if(specularMap) specularMap->dispose(); }

}

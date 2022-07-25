#include "material.hpp"
#include <algorithm>

namespace Cyclone {

	Material::Material(const glm::vec3& diffuse, const glm::vec3& ambient, const glm::vec3& specular, Texture* texture): diffuse(diffuse), ambient(ambient), specular(specular), texture(texture), normalMap(0), specularMap(0), shineDamper(1), specularReflectivity(0), transparency(0), enviroRefractivity(0.5), fresnelPower(2), isDoubleSided(false), hasFresnel(true), rows({}) {}

	Material::Material(const Material& material): diffuse(material.diffuse), ambient(material.ambient), specular(material.specular), texture(material.texture), normalMap(material.normalMap), specularMap(material.specularMap), shineDamper(1), specularReflectivity(0), transparency(0), enviroRefractivity(0.5), fresnelPower(2), isDoubleSided(false), hasFresnel(true), rows({}) {}

	Material::Material(Material&& material): diffuse(std::move(material.diffuse)), ambient(std::move(material.ambient)), specular(std::move(material.specular)), texture(std::move(material.texture)), normalMap(std::move(material.normalMap)), specularMap(std::move(material.specularMap)), shineDamper(1), specularReflectivity(0), transparency(0), enviroRefractivity(0.5), fresnelPower(2), isDoubleSided(false), hasFresnel(true), rows({}) {}

	const glm::vec3& Material::getDiffuseColor() const { return diffuse; }

	const glm::vec3& Material::getAmbientColor() const { return ambient; }

	const glm::vec3& Material::getSpecularColor() const { return specular; }

	const glm::vec2& Material::getRows() const { return rows; }

	Texture* Material::getTexture() const { return texture; }

	Texture* Material::getNormalMap() const { return normalMap; }

	Texture* Material::getSpecularMap() const { return specularMap; }

	void Material::setDiffuseColor(const glm::vec3& diffuse) { this->diffuse = diffuse; }

	void Material::setAmbientColor(const glm::vec3& ambient) { this->ambient = ambient; }

	void Material::setSpecularColor(const glm::vec3& specular) { this->specular = specular; }

	void Material::setRows(const glm::vec2& rows) { this->rows = rows; }

	void Material::setTexture(Texture* const texture) { this->texture = texture; }

	void Material::setNormalMap(Texture* const normal) { this->normalMap = normal; }

	void Material::setSpecularMap(Texture* const specular) { this->specularMap = specular; }

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

	void Material::dispose() { texture->dispose(); normalMap->dispose(); specularMap->dispose(); }

}

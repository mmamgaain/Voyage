#pragma once
#include <voyage.hpp>
#include "Voyage/loader.hpp"
#include "Voyage/material.hpp"
#include <thread>

namespace Voyage {

#define INVALID_MATERIAL 0xFFFFFFFF;

	typedef struct Model {
		public:
			Model(const char* const filename, Loader& loader, const glm::vec3& position = {}, const glm::vec3& rotation = {}, const glm::vec3& scale = {1.0, 1.0, 1.0}, Material& material = DEFAULT_MATERIAL);

			Model(const Model& other) noexcept;

			Model(Model&& other) noexcept;

			const std::vector<std::shared_ptr<RawModel>>& getModels() const;

			const Material& getMaterial() const;

			void getTransformation(glm::mat4& dest) const;

			Model& operator=(const Model& other);

			Model& operator=(Model&& other);

			~Model() noexcept;

			void dispose();
		private:
			Assimp::Importer importer;
			std::vector<std::shared_ptr<RawModel>> models;
			glm::vec3 position, rotation, scale;
			std::string filepath;
			Material material;
			static Material DEFAULT_MATERIAL;
			// std::thread worker;

			void processScene(const aiScene* scene, Loader& loader);

			void processMaterials(const aiScene* scene, const std::string& filepath);

			void processMesh(const aiMesh* mesh, Loader& loader);
	} Model;

}

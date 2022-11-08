#pragma once
#include <voyage.hpp>
#include "Voyage/loader.hpp"
#include "Voyage/material.hpp"

namespace Voyage {

	typedef struct Model {
		public:
			Model(const char* const filename, Loader& loader, const glm::vec3& position = {}, const glm::vec3& rotation = {}, const glm::vec3& scale = {1.0, 1.0, 1.0}, Material& material = DEFAULT_MATERIAL);

			Model(const Model& model);

			Model(Model&& model) noexcept;

			const std::vector<RawModel>& getModels() const;

			const Material& getMaterial() const;

			void getTransformation(glm::mat4& dest) const;

			const Model& operator=(Model&& model);

			~Model();
		private:
			std::vector<RawModel> models;
			glm::vec3 position, rotation, scale;
			std::string filepath;
			Material material;
			static Material DEFAULT_MATERIAL;

			void processScene(const aiScene* scene, Loader& loader);

			void processMaterials(const aiScene* scene, const std::string& filepath);

			void processMesh(const aiMesh* mesh, Loader& loader);

			void dispose();
	} Model;

}

#pragma once
#include <voyage.hpp>
#include "extra/loader.hpp"

namespace Voyage {

	typedef struct Model {
		public:
			Model(const char* const filename, Loader& loader, const glm::vec3& position = {}, const glm::vec3& rotation = {}, const glm::vec3& scale = {1.0, 1.0, 1.0});

			Model(const Model& model);

			Model(Model&& model) noexcept;

			const std::vector<RawModel>& getModels() const;

			void getTransformation(glm::mat4& dest) const;

			const Model& operator=(Model&& model);

			~Model();
		private:
			std::vector<RawModel> models;
			glm::vec3 position, rotation, scale;
			std::string filepath;

			void processScene(const aiScene* scene, Loader& loader);

			void processMaterials(const aiScene* scene, const std::string& filepath);

			void processMesh(const aiMesh* mesh, Loader& loader);

			void dispose();
	} Model;

}

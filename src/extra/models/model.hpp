#pragma once
#include <voyage.hpp>
#include "../loader.hpp"

namespace Cyclone {

	typedef struct Model {
		public:
			Model(const char* filename, Loader& loader);

			Model(const Model& model);

			Model(Model&& model);

			std::vector<RawModel>& getModels();
	
			const Model& operator=(Model&& model);
		private:
			std::vector<RawModel> models;
			std::string filepath;
			void processScene(const aiScene* scene, Loader& loader);

			void processMaterials(const aiScene* scene, const std::string& filepath);

			void processMesh(const aiMesh* mesh, Loader& loader);

			void dispose(Loader& loader);
	} Model;

}

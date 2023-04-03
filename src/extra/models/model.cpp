#include "Voyage/model.hpp"
#include "Voyage/maths.hpp"
#include "Voyage/material.hpp"
#include "Voyage/raw_model.hpp"

namespace Voyage {

	Material Model::DEFAULT_MATERIAL;

	Model::Model(const char* const filename, Loader& loader, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, Material& material): position(position), rotation(rotation), scale(scale) {
		const aiScene* scene = aiImportFile(filename, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenUVCoords | aiProcess_CalcTangentSpace | aiProcess_FixInfacingNormals | aiProcess_OptimizeMeshes | aiProcess_GenNormals);
		int slashIndex = -1;

		for(size_t i = 0; i < strlen(filename); i++) { if(filename[i] == '/') slashIndex = i; }

		if(slashIndex == -1) filepath = ".";
		else if(slashIndex == 0) filepath = "/";
		else filepath.copy((char*)filename, slashIndex);

		if(scene == nullptr || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr) {
			std::cerr << "Assimp : Error while loading file " << filename << std::endl;
			std::cerr << "Assimp flag ID : " << scene->mFlags << std::endl;
		}

		processScene(scene, loader);

		aiReleaseImport(scene);
	}

	Model::Model(const Model& model) noexcept: models(model.models), position(model.position), rotation(model.rotation), scale(model.scale), filepath(model.filepath), material(model.material) {}

	Model::Model(Model&& model) noexcept: models(std::move(model.models)), position(model.position), rotation(model.rotation), filepath(std::move(model.filepath)), material(std::move(model.material)) {}

	Model::~Model() noexcept { dispose(); }

	const std::vector<std::shared_ptr<RawModel>>& Model::getModels() const { return models; }

	const Material& Model::getMaterial() const { return material; }

	void Model::getTransformation(glm::mat4& dest) const { getTransformationMatrix(dest, position, rotation, scale); }

	Model& Model::operator=(const Model& other) {
		if(this == &other) return *this;
		dispose();
		models = other.models;
		position = other.position;
		rotation = other.rotation;
		filepath = other.filepath;
		material = other.material;
		return *this;
	}

	Model& Model::operator=(Model&& other) {
		if(this == &other) return *this;
		dispose();
		models = std::move(other.models);
		position = std::move(other.position);
		rotation = std::move(other.rotation);
		filepath = std::move(other.filepath);
		material = std::move(other.material);
		return *this;
	}

	void Model::processMaterials(const aiScene* scene, const std::string& filepath) {
		aiMaterial* mat = scene->mMaterials[0];
		aiColor3D diffuse, specular, ambient;
		mat->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
		mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
		mat->Get(AI_MATKEY_COLOR_SPECULAR, specular);
		material.setAmbientColor(ambient);
		material.setDiffuseColor(diffuse);
		material.setSpecularColor(specular);
	}

	void setData(void* dest, const void* src, const size_t copy_size) {
		if(src != nullptr) memcpy(dest, src, copy_size);
		else memset(dest, 0, copy_size);
	}

	void Model::processMesh(const aiMesh* mesh, Loader& loader) {
		size_t num_indices = mesh->mNumFaces * 3;	// The number of elements in the indices array
		size_t num_vertices = mesh->mNumVertices;	// One vertex represents 3 (X, Y, Z) elements in the vertices array
		uint32_t *indices = new uint32_t[num_indices];
		float *position = new float[num_vertices * 3], *texture_coords = new float[num_vertices * 2], *normals = new float[num_vertices * 3], *tangents = new float[num_vertices * 3], *bitangents = new float[num_vertices * 3];
		aiVector3D curr_vert, curr_norm, curr_tang, curr_bitang;
		for(uint32_t i = 0; i < num_vertices; i++) {
			uint32_t x = i * 3, y = x + 1, z = y + 1;
			// VERTICES
			curr_vert = mesh->mVertices[i];
			position[x] = curr_vert.x;
			position[y] = curr_vert.y;
			position[z] = curr_vert.z;

			// TEXTURE_COORDINATES
			if(mesh->HasTextureCoords(0)) {
				aiVector3D tex_coords = mesh->mTextureCoords[0][i];
				texture_coords[i * 2	] = tex_coords.x;
				texture_coords[i * 2 + 1] = tex_coords.y;
			}
			else {
				texture_coords[i * 2	] = 0.0;
				texture_coords[i * 2 + 1] = 0.0;
			}

			// NORMALS
			if(mesh->mNormals != nullptr) {
				curr_norm = mesh->mNormals[i];
				normals[x] = curr_norm.x;
				normals[y] = curr_norm.y;
				normals[z] = curr_norm.z;
			}

			// TANGENTS
			if(mesh->mTangents != nullptr) {
				curr_tang = mesh->mTangents[i];
				tangents[x] = curr_tang.x;
				tangents[y] = curr_tang.y;
				tangents[z] = curr_tang.z;
			}

			// BITANGENTS
			if(mesh->mBitangents != nullptr) {
				curr_bitang = mesh->mBitangents[i];
				bitangents[x] = curr_bitang.x;
				bitangents[y] = curr_bitang.y;
				bitangents[z] = curr_bitang.z;
			}
		}

		// INDICES
		for(uint32_t i = 0; i < mesh->mNumFaces; i++) {
			const aiFace& face = mesh->mFaces[i];
			indices[(i * 3)	   ] = face.mIndices[0];
			indices[(i * 3) + 1] = face.mIndices[1];
			indices[(i * 3) + 2] = face.mIndices[2];
		}
		models.push_back(std::move(loader.loadToVAO(position, 3, num_vertices, num_indices, indices, texture_coords, normals, tangents, bitangents)));

		delete[] position;
		delete[] indices;
		delete[] texture_coords;
		delete[] normals;
		delete[] tangents;
		delete[] bitangents;
	}

	void Model::processScene(const aiScene* scene, Loader& loader) {
		// Process materials, if any...
		processMaterials(scene, "");

		//Process models, if any...
		models.reserve(scene->mNumMeshes);
		for(uint32_t i = 0; i < scene->mNumMeshes; i++) processMesh(scene->mMeshes[i], loader);
	}

	void Model::dispose() {
		for(auto& model : models) model->dispose();
		models.clear();
	}

}

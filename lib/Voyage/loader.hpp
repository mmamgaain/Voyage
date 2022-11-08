#pragma once
#include <voyage.hpp>
#include "Voyage/raw_model.hpp"
#include "Voyage/texture.hpp"

namespace Voyage {

	enum RENDER_DATATYPE { RENDER_DATATYPE_ALL, RENDER_DATATYPE_VAO, RENDER_DATATYPE_VBO, RENDER_DATATYPE_TEXTURE };

	class Loader {
		public:
			Loader() noexcept = default;

			Loader(const Loader& loader);

			Loader(Loader&& loader) noexcept;

			~Loader();

			std::shared_ptr<RawModel> loadToVAO(const std::vector<float>& vertices, const int& dimension, const std::vector<unsigned int>& indices = {}, const std::vector<float>& texture_coords = {}, const std::vector<float>& normals = {}, const std::vector<float>& tangents = {}, const std::vector<float>& bitangents = {});

			std::shared_ptr<RawModel> loadToVAO(const float* vertices, const int& dimension, const size_t& num_vertices, const size_t& num_indices, const unsigned int* indices = nullptr, const float* texture_coords = nullptr, const float* normals = nullptr, const float* tangents = nullptr, const float* bitangents = nullptr);

			std::shared_ptr<Texture> const loadTexture(const char* filename, const bool& flip_vertically = true);

			std::shared_ptr<Texture> const loadTexture(const std::vector<const char*>& filenames, const std::vector<bool>& flip_vertically = {false, false, false, false, false, false});

			void dispose();

		private:
			std::vector<std::weak_ptr<RawModel>> vaos;
			std::vector<std::weak_ptr<Texture>> textures;

			void createVAO(unsigned int& vaoID);

			void bindIndicesBuffer(const std::vector<unsigned int>& indices, std::vector<unsigned int>* vbos);

			void bindIndicesBuffer(const unsigned int* indices, const size_t& length, std::vector<unsigned int>* vbos);

			void storeDataInAttributeList(const unsigned int& attribNumber, const unsigned int& dimension, const std::vector<float>& data, std::vector<unsigned int>* vbos);

			void storeDataInAttributeList(const unsigned int& attribNumber, const unsigned int& dimension, const float* data, const size_t& length, std::vector<unsigned int>* vbos);

			void unbindVAO() const;
	};

}


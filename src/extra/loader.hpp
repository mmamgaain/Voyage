#pragma once
#include <voyage.hpp>
#include "models/raw_model.hpp"

namespace Cyclone {

	class Loader {
		public:
			enum RENDER_DATATYPE { RENDER_DATATYPE_ALL, RENDER_DATATYPE_VAO, RENDER_DATATYPE_VBO, RENDER_DATATYPE_TEXTURE };

			Loader() = default;

			Loader(const Loader& loader);

			Loader(Loader&& loader) noexcept;

			~Loader() = default;

			std::unique_ptr<RawModel> loadToVAO(const std::vector<float>& vertices, const int& dimension, const std::vector<unsigned int>& indices = {}, const std::vector<float>& texture_coords = {}, const std::vector<float>& normals = {}, const std::vector<float>& tangents = {}, const std::vector<float>& bitangents = {});

std::unique_ptr<RawModel> loadToVAO(const float* vertices, const int& dimension, const size_t& num_vertices, const size_t& num_indices, const unsigned int* indices = nullptr, const float* texture_coords = nullptr, const float* normals = nullptr, const float* tangents = nullptr, const float* bitangents = nullptr);

			Texture* const loadTexture(const char* filename, const bool& flip_vertically = true);

			void dispose(const unsigned int& id = 0, const RENDER_DATATYPE& type = RENDER_DATATYPE_ALL);

		private:
			std::vector<unsigned int> vaos, vbos;
			std::vector<Texture> textures;

			void createVAO(unsigned int& vaoID);

			void bindIndicesBuffer(const std::vector<unsigned int>& indices);

			void bindIndicesBuffer(const unsigned int* indices, const size_t& length);

			void storeDataInAttributeList(const unsigned int& attribNumber, const unsigned int& dimension, const std::vector<float>& data);

			void storeDataInAttributeList(const unsigned int& attribNumber, const unsigned int& dimension, const float* data, const size_t& length);

			void unbindVAO() const;

	};

}


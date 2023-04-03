#pragma once
#include <voyage.hpp>
#include "Voyage/raw_model.hpp"
#include "Voyage/texture.hpp"

namespace Voyage {

	enum RENDER_DATATYPE { RENDER_DATATYPE_ALL, RENDER_DATATYPE_VAO, RENDER_DATATYPE_VBO, RENDER_DATATYPE_TEXTURE };

	class Loader {
		public:
			Loader() noexcept = default;

			Loader(const Loader& loader) noexcept;

			Loader(Loader&& loader) noexcept;

			Loader& operator=(const Loader& lhs) noexcept;

			Loader& operator=(Loader&& rhs) noexcept;

			~Loader() noexcept;

			std::shared_ptr<RawModel> loadToVAO(const std::vector<float>& vertices, const int& dimension, const std::vector<uint32_t>& indices = {}, const std::vector<float>& texture_coords = {}, const std::vector<float>& normals = {}, const std::vector<float>& tangents = {}, const std::vector<float>& bitangents = {});

			std::shared_ptr<RawModel> loadToVAO(const float* vertices, const int& dimension, const size_t& num_vertices, const size_t& num_indices, const uint32_t* indices = nullptr, const float* texture_coords = nullptr, const float* normals = nullptr, const float* tangents = nullptr, const float* bitangents = nullptr);

			const uint32_t loadEmptyVBO(const uint32_t& floatCount) const;

			std::shared_ptr<Texture> const loadTexture(const char* const filename, const bool& flip_vertically = true);

			std::shared_ptr<Texture> const loadTexture(const std::vector<const char*>& filenames, const std::vector<bool>& flip_vertically = {false, false, false, false, false, false});

			void dispose();

			void updateVBO(const uint32_t& vboID, const float* const data, const unsigned long& size_in_bytes) const;
		private:
			std::vector<std::weak_ptr<RawModel>> vaos;
			std::vector<std::weak_ptr<Texture>> textures;

			void createVAO(uint32_t& vaoID);

			void bindIndicesBuffer(const std::vector<uint32_t>& indices, std::vector<uint32_t>* vbos);

			void bindIndicesBuffer(const uint32_t* indices, const size_t& length, std::vector<uint32_t>* vbos);

			void storeDataInAttributeList(const uint32_t& attribNumber, const uint32_t& dimension, const std::vector<float>& data, std::vector<uint32_t>* vbos);

			void storeDataInAttributeList(const uint32_t& attribNumber, const uint32_t& dimension, const float* const data, const size_t& length, std::vector<uint32_t>* vbos);

			void unbindVAO() const;
	};

}


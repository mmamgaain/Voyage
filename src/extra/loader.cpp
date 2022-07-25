#include "loader.hpp"

namespace Cyclone {

	Loader::Loader(const Loader& loader) {
		vaos.reserve(loader.vaos.size());
		vbos.reserve(loader.vbos.size());
		textures.reserve(loader.textures.size());

		memcpy(vaos.data(), loader.vaos.data(), vaos.size() * sizeof(unsigned int));
		memcpy(vbos.data(), loader.vbos.data(), vbos.size() * sizeof(unsigned int));
		memcpy(textures.data(), loader.textures.data(), textures.size() * sizeof(unsigned int));
	}

	Loader::Loader(Loader&& loader) noexcept: vaos(std::move(loader.vaos)), vbos(std::move(loader.vbos)), textures(std::move(loader.textures)) { loader.vaos.clear(); loader.vbos.clear(); loader.textures.clear(); }

	Texture* const Loader::loadTexture(const char* filename, const bool& flip_vertically) {
		textures.emplace_back(filename, flip_vertically);
		return &textures.at(textures.size() - 1);
	}

	void Loader::dispose(const unsigned int& id, const RENDER_DATATYPE& type) {
		if(type == RENDER_DATATYPE_ALL) {
			glDeleteVertexArrays(vaos.size(), &vaos[0]);
			glDeleteBuffers(vbos.size(), &vbos[0]);
			for(Texture& texture : textures) texture.dispose();
			vaos.clear();
			vbos.clear();
			textures.clear();
		}
		else if(!id) return;
		else {
			switch (type) {
			case RENDER_DATATYPE_VAO:
				glDeleteVertexArrays(1, &id);
			case RENDER_DATATYPE_VBO:
				glDeleteBuffers(1, &id);
			case RENDER_DATATYPE_TEXTURE:
				glDeleteTextures(1, &id);
			case RENDER_DATATYPE_ALL:
				break;
			}
		}
	}

	void Loader::createVAO(unsigned int& vaoID) {
		glGenVertexArrays(1, &vaoID);
		glBindVertexArray(vaoID);
		vaos.push_back(vaoID);
	}

	void Loader::bindIndicesBuffer(const std::vector<unsigned int>& indices) {
		unsigned int vboID;
		glGenBuffers(1, &vboID);
		vbos.push_back(vboID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	}

	void Loader::bindIndicesBuffer(const unsigned int* indices, const size_t& length) {
		unsigned int vboID;
		glGenBuffers(1, &vboID);
		vbos.push_back(vboID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, length * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	}

	std::unique_ptr<RawModel> Loader::loadToVAO(const float* vertices, const int& dimension, const size_t& num_vertices, const size_t& num_indices, const unsigned int* indices, const float* texture_coords, const float* normals, const float* tangents, const float* bitangents) {
		unsigned int vaoID, i = 0, vertex_count = num_vertices * dimension;
		createVAO(vaoID);
		storeDataInAttributeList(i++, dimension, vertices, vertex_count);
		if(indices) bindIndicesBuffer(indices, num_indices);
		if(texture_coords) storeDataInAttributeList(i++, 2, texture_coords, vertex_count);
		if(normals) storeDataInAttributeList(i++, dimension, normals, vertex_count);
		if(tangents) storeDataInAttributeList(i++, dimension, tangents, vertex_count);
		if(bitangents) storeDataInAttributeList(i++, dimension, bitangents, vertex_count);
		unbindVAO();
		return std::make_unique<RawModel>(vaoID, !indices ? vertex_count : num_indices, i, indices != nullptr);
	}

	std::unique_ptr<RawModel> Loader::loadToVAO(const std::vector<float>& vertices, const int& dimension, const std::vector<unsigned int>& indices, const std::vector<float>& texture_coords, const std::vector<float>& normals, const std::vector<float>& tangents, const std::vector<float>& bitangents) {
		unsigned int vaoID, i = 0;
		createVAO(vaoID);
		storeDataInAttributeList(i++, dimension, vertices);
		if(indices.size() != 0) bindIndicesBuffer(indices);
		if(texture_coords.size() != 0) storeDataInAttributeList(i++, 2, texture_coords);
		if(normals.size()) storeDataInAttributeList(i++, dimension, normals);
		if(tangents.size() != 0) storeDataInAttributeList(i++, dimension, tangents);
		if(bitangents.size() != 0) storeDataInAttributeList(i++, dimension, bitangents);
		unbindVAO();
		return std::make_unique<RawModel>(vaoID, indices.size() == 0 ? vertices.size() / dimension : indices.size(), i, indices.size() != 0);
	}

	void Loader::storeDataInAttributeList(const unsigned int& attribNumber, const unsigned int& dimension, const std::vector<float>& data) {
		unsigned int vboID;
		glGenBuffers(1, &vboID);
		vbos.push_back(vboID);
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(attribNumber);
		glVertexAttribPointer(attribNumber, dimension, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Loader::storeDataInAttributeList(const unsigned int& attribNumber, const unsigned int& dimension, const float* data, const size_t& length) {
		unsigned int vboID;
		glGenBuffers(1, &vboID);
		vbos.push_back(vboID);
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glBufferData(GL_ARRAY_BUFFER, length * sizeof(float), data, GL_STATIC_DRAW);
		glEnableVertexAttribArray(attribNumber);
		glVertexAttribPointer(attribNumber, dimension, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Loader::unbindVAO() const { glBindVertexArray(0); }


}

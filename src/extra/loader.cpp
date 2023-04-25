#include "Voyage/loader.hpp"
#include "Voyage/raw_model.hpp"
#include "Voyage/texture.hpp"

namespace Voyage {

	Loader::Loader(const Loader& loader) noexcept {
		vaos.reserve(loader.vaos.size());
		textures.reserve(loader.textures.size());
		for(uint32_t i = 0; i < loader.vaos.size(); ++i) { vaos.push_back(loader.vaos[i]); }
		for(uint32_t i = 0; i < loader.textures.size(); ++i) { textures.push_back(loader.textures[i]); }
	}

	Loader::Loader(Loader&& loader) noexcept: vaos(std::move(loader.vaos)), textures(std::move(loader.textures)) {
		loader.vaos.clear();
		loader.textures.clear();
	}

	std::shared_ptr<Texture> const Loader::loadTexture(const char* const filename, const bool& flip_vertically) {
		std::shared_ptr<Texture> tex = std::make_shared<Texture>(filename, flip_vertically);
		textures.push_back(tex);
		return tex;
	}

	Loader::~Loader() noexcept { dispose(); }

	std::shared_ptr<Texture> const Loader::loadTexture(const std::vector<const char*>& filenames, const std::vector<bool>& flip_vertically) {
		std::shared_ptr<Texture> tex = std::make_shared<Texture>(filenames, flip_vertically);
		textures.push_back(tex);
		return tex;
	}

	void Loader::dispose() {
		std::for_each(vaos.begin(), vaos.end(), [](const std::weak_ptr<RawModel>& vao) { if(!vao.expired()) vao.lock().get()->dispose(); });
		std::for_each(textures.begin(), textures.end(), [](const std::weak_ptr<Texture>& texture) { if(!texture.expired()) texture.lock().get()->dispose(); });
		vaos.clear();
		textures.clear();
	}

	void Loader::updateVBO(const uint32_t& vboID, const float* const data, const uint64_t& size_in_floats) const {
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size_in_floats * sizeof(float), data);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Loader::createVAO(uint32_t& vaoID) {
		glGenVertexArrays(1, &vaoID);
		glBindVertexArray(vaoID);
	}

	void Loader::bindIndicesBuffer(const std::vector<uint32_t>& indices, std::vector<uint32_t>* vbos) {
		uint32_t vboID;
		glGenBuffers(1, &vboID);
		vbos->push_back(vboID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), &indices[0], GL_STATIC_DRAW);
	}

	void Loader::bindIndicesBuffer(const uint32_t* indices, const size_t& length, std::vector<uint32_t>* vbos) {
		uint32_t vboID;
		glGenBuffers(1, &vboID);
		vbos->push_back(vboID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, length * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	const uint32_t Loader::loadEmptyVBO(const uint32_t& floatCount) const {
		uint32_t vboID;
		glGenBuffers(1, &vboID);
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glBufferData(GL_ARRAY_BUFFER, floatCount * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		return vboID;
	}

	std::shared_ptr<RawModel> Loader::loadToVAO(const float* vertices, const int& dimension, const size_t& num_vertices, const size_t& num_indices, const uint32_t* indices, const float* texture_coords, const float* normals, const float* tangents, const float* bitangents) {
		uint32_t vaoID, i = 0, vertex_count = num_vertices * dimension;
		std::vector<uint32_t> vbos;
		createVAO(vaoID);
		storeDataInAttributeList(i++, dimension, vertices, vertex_count, &vbos);
		if(indices) bindIndicesBuffer(indices, num_indices, &vbos);
		if(texture_coords) storeDataInAttributeList(i++, 2, texture_coords, vertex_count, &vbos);
		if(normals) storeDataInAttributeList(i++, dimension, normals, vertex_count, &vbos);
		if(tangents) storeDataInAttributeList(i++, dimension, tangents, vertex_count, &vbos);
		if(bitangents) storeDataInAttributeList(i++, dimension, bitangents, vertex_count, &vbos);
		unbindVAO();
		std::shared_ptr<RawModel> model = std::make_shared<RawModel>(vaoID, !indices ? vertex_count : num_indices, vbos, indices != nullptr);
		vaos.push_back(model);
		return model;
	}

	std::shared_ptr<RawModel> Loader::loadToVAO(const std::vector<float>& vertices, const int& dimension, const std::vector<uint32_t>& indices, const std::vector<float>& texture_coords, const std::vector<float>& normals, const std::vector<float>& tangents, const std::vector<float>& bitangents) {
		uint32_t vaoID, i = 0;
		createVAO(vaoID);
		std::vector<uint32_t> vbos;
		storeDataInAttributeList(i++, dimension, vertices, &vbos);
		if(indices.size() != 0) bindIndicesBuffer(indices, &vbos);
		if(texture_coords.size() != 0) storeDataInAttributeList(i++, 2, texture_coords, &vbos);
		if(normals.size()) storeDataInAttributeList(i++, dimension, normals, &vbos);
		if(tangents.size() != 0) storeDataInAttributeList(i++, dimension, tangents, &vbos);
		if(bitangents.size() != 0) storeDataInAttributeList(i, dimension, bitangents, &vbos);
		unbindVAO();
		std::shared_ptr<RawModel> model = std::make_shared<RawModel>(vaoID, indices.size() == 0 ? vertices.size() / dimension : indices.size(), vbos, indices.size() != 0);
		vaos.push_back(model);
		return model;
	}

	void Loader::storeDataInAttributeList(const uint32_t& attribNumber, const uint32_t& dimension, const std::vector<float>& data, std::vector<uint32_t>* vbos) {
		uint32_t vboID;
		glGenBuffers(1, &vboID);
		vbos->push_back(vboID);
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(attribNumber);
		glVertexAttribPointer(attribNumber, dimension, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Loader::storeDataInAttributeList(const uint32_t& attribNumber, const uint32_t& dimension, const float* const data, const size_t& length, std::vector<uint32_t>* vbos) {
		uint32_t vboID;
		glGenBuffers(1, &vboID);
		vbos->push_back(vboID);
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glBufferData(GL_ARRAY_BUFFER, length * sizeof(float), data, GL_STATIC_DRAW);
		glEnableVertexAttribArray(attribNumber);
		glVertexAttribPointer(attribNumber, dimension, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Loader::unbindVAO() const { glBindVertexArray(0); }
}

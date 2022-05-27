#include "loader.hpp"
#include "../../lib/glad.h"
#include <cstring>
#include <memory>
#include <type_traits>
#include <vector>
#include <iostream>

using namespace Cyclone;

Loader::Loader(const Loader& loader) {
	vaos.reserve(loader.vaos.size());
	vbos.reserve(loader.vbos.size());
	textures.reserve(loader.textures.size());

	memcpy(vaos.data(), loader.vaos.data(), vaos.size() * sizeof(unsigned int));
	memcpy(vbos.data(), loader.vbos.data(), vbos.size() * sizeof(unsigned int));
	memcpy(textures.data(), loader.textures.data(), textures.size() * sizeof(unsigned int));
}

Loader::Loader(Loader&& loader) noexcept: vaos(std::move(loader.vaos)), vbos(std::move(loader.vbos)), textures(std::move(loader.textures)) { loader.vaos.clear(); loader.vbos.clear(); loader.textures.clear(); }

Loader::~Loader() {}

void Loader::createVAO(unsigned int& vaoID) {
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);
	vaos.push_back(vaoID);
}

void Loader::bindIndicesBuffer(const std::vector<unsigned int>& indices) {
	unsigned int vboID;
	vbos.push_back(vboID);
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size(), &indices[0], GL_STATIC_DRAW);
}

void Loader::storeDataInAttributeList(const unsigned int& attribNumber, const unsigned int& dimension, const std::vector<float>& data) {
	unsigned int vboID;
	glGenBuffers(1, &vboID);
	vbos.push_back(vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, data.size(), &data[0], GL_STATIC_DRAW);
	glVertexAttribPointer(attribNumber, dimension, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Loader::unbindVAO() { glBindVertexArray(0); }

std::unique_ptr<RawModel> Loader::loadToVAO(const std::vector<float>& vertices, const int& dimension, const std::vector<unsigned int>& indices, const std::vector<float>& texture_coords, const std::vector<float>& normals, const std::vector<float>& tangents, const std::vector<float>& bitangents) {
	unsigned int vaoID, i = 0;
	createVAO(vaoID);
	storeDataInAttributeList(i++, dimension, vertices);
	if(indices.size() != 0) bindIndicesBuffer(indices);
	if(texture_coords.size() != 0) storeDataInAttributeList(i++, dimension, texture_coords);
	if(normals.size()) storeDataInAttributeList(i++, dimension, normals);
	if(tangents.size() != 0) storeDataInAttributeList(i++, dimension, tangents);
	if(bitangents.size() != 0) storeDataInAttributeList(i++, dimension, bitangents);
	unbindVAO();
	return std::make_unique<RawModel>(vaoID, indices.size() == 0 ? vertices.size() : indices.size() / dimension, i, indices.size() != 0);
}

void Loader::dispose() {
	glDeleteVertexArrays(vaos.size(), &vaos[0]);
	glDeleteBuffers(vbos.size(), &vbos[0]);
	glDeleteTextures(textures.size(), &textures[0]);
	vaos.clear();
	vbos.clear();
	textures.clear();
}


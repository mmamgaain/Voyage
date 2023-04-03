#include "Voyage/raw_model.hpp"

namespace Voyage {

	// Raw Model declarations start
	RawModel::RawModel(const uint32_t& vao_id, const size_t& vertex_count, const std::vector<uint32_t>& vbos, const bool& has_index_buffer): vao_id(vao_id), vertex_count(vertex_count), vbos(vbos), has_index_buffer(has_index_buffer) {}

	RawModel::RawModel(const RawModel& model): vao_id(model.vao_id), vertex_count(model.vertex_count), vbos(model.vbos), has_index_buffer(model.has_index_buffer) {}

	RawModel::RawModel(RawModel&& model) noexcept: vao_id(model.vao_id), vertex_count(model.vertex_count), vbos(std::move(model.vbos)), has_index_buffer(model.has_index_buffer) {}

	RawModel::~RawModel() { dispose(); }

	const bool& RawModel::hasIndexBuffer() const { return has_index_buffer; }

	uint32_t RawModel::getNumBuffers() const { return vbos.size(); }

	bool RawModel::isValidNumBuffers() const { return vbos.size() > 0; }

	const uint32_t& RawModel::getID() const { return vao_id; }

	const size_t& RawModel::getVertexCount() const { return vertex_count; }

	RawModel& RawModel::operator=(const RawModel& other) noexcept {
		if(this == &other) return *this;
		dispose();
		vao_id = other.vao_id;
		vertex_count = other.vertex_count;
		vbos = other.vbos;
		has_index_buffer = other.has_index_buffer;
		return *this;
	}

	RawModel& RawModel::operator=(RawModel&& other) noexcept {
		if(this == &other) return *this;
		dispose();
		vao_id = other.vao_id;
		vertex_count = other.vertex_count;
		vbos = std::move(other.vbos);
		has_index_buffer = other.has_index_buffer;
		return *this;
	}

	std::ostream& operator<<(std::ostream& stream, const RawModel& model) noexcept {
		stream << "Raw Model: id:-- " << model.vao_id << ", vertex count:-- " << model.vertex_count << ", number of buffers:-- " << model.vbos.size() << ", has_index_buffer:-- " << model.has_index_buffer;
		return stream;
	}

	void RawModel::addInstancedAttribute(const uint32_t& vboID, const uint32_t& attribute, const uint32_t& dimension, const uint32_t& instancedDataLength, const void* offset_in_bytes) const {
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glBindVertexArray(vao_id);
		glVertexAttribPointer(attribute, dimension, GL_FLOAT, GL_FALSE, instancedDataLength * sizeof(float), offset_in_bytes);
		glVertexAttribDivisor(attribute, 1);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void RawModel::_setVBOs(const std::vector<uint32_t>& vbos) const { this->vbos.reserve(vbos.size()); }

	void RawModel::dispose() {
		if(vao_id == 0) return;
		glDeleteVertexArrays(1, &vao_id);
		glDeleteBuffers(vbos.size(), vbos.data());
		vao_id = 0;
	}

	// Raw Model declarations end

	// Textured Model declarations start
	// Textured Model declarations end


}

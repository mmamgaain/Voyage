#include "Voyage/raw_model.hpp"

namespace Voyage {

	// Raw Model declarations start
	RawModel::RawModel(const unsigned int& vao_id, const size_t& vertex_count, const std::vector<unsigned int>& vbos, const bool& has_index_buffer): vao_id(vao_id), vertex_count(vertex_count), vbos(vbos), has_index_buffer(has_index_buffer) {}

	RawModel::RawModel(const RawModel& model): vao_id(model.vao_id), vertex_count(model.vertex_count), vbos(model.vbos), has_index_buffer(model.has_index_buffer) {}

	RawModel::RawModel(RawModel&& model) noexcept: vao_id(std::move(model.vao_id)), vertex_count(std::move(model.vertex_count)), vbos(std::move(model.vbos)), has_index_buffer(std::move(model.has_index_buffer)) {}

	RawModel::~RawModel() { dispose(); }

	const bool& RawModel::hasIndexBuffer() const { return has_index_buffer; }

	unsigned int RawModel::getNumBuffers() const { return vbos.size(); }

	bool RawModel::isValidNumBuffers() const { return vbos.size() > 0; }

	const unsigned int& RawModel::getID() const { return vao_id; }

	const size_t& RawModel::getVertexCount() const { return vertex_count; }

	RawModel& RawModel::operator=(RawModel&& other) noexcept {
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

	void RawModel::dispose() {
		if(vao_id == -1) return;
		vao_id = -1;
		glDeleteVertexArrays(1, &vao_id);
		for(unsigned int vbo : vbos) glDeleteBuffers(1, &vbo);
	}

	void RawModel::_setVBOs(const std::vector<unsigned int>& vbos) const { this->vbos.reserve(vbos.size()); }

	// Raw Model declarations end

	// Textured Model declarations start
	// Textured Model declarations end


}

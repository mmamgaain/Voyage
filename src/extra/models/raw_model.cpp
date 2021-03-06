#include "raw_model.hpp"

namespace Cyclone {

	RawModel::RawModel(const unsigned int& vao_id, const size_t& vertex_count, const int& num_buffers, const bool& has_index_buffer): vao_id(vao_id), vertex_count(vertex_count), num_buffers(std::max(-1, num_buffers)), has_index_buffer(has_index_buffer) {}
	
	RawModel::RawModel(const RawModel& model): vao_id(model.vao_id), vertex_count(model.vertex_count), num_buffers(model.num_buffers), has_index_buffer(model.has_index_buffer) {}
	
	RawModel::RawModel(RawModel&& model) noexcept: vao_id(std::move(model.vao_id)), vertex_count(std::move(model.vertex_count)), num_buffers(std::move(model.num_buffers)), has_index_buffer(std::move(model.has_index_buffer)) {}
	
	void RawModel::setNumBuffers(const unsigned int& num_buffers) { this->num_buffers = num_buffers; }
	
	bool RawModel::hasIndexBuffer() const { return has_index_buffer; }
	
	int RawModel::getNumBuffers() const { return num_buffers; }
	
	bool RawModel::isValidNumBuffers() const { return num_buffers > -1; }
	
	unsigned int RawModel::getID() const { return vao_id; }
	
	size_t RawModel::getVertexCount() const { return vertex_count; }
	
	RawModel& RawModel::operator=(RawModel&& other) noexcept {
		vao_id = other.vao_id;
		vertex_count = other.vertex_count;
		num_buffers = other.num_buffers;
		has_index_buffer = other.has_index_buffer;
		return *this;
	}
	
	std::ostream& operator<<(std::ostream& stream, const RawModel& model) noexcept {
		stream << "Raw Model: id:-- " << model.vao_id << ", vertex count:-- " << model.vertex_count << ", number of buffers:-- " << model.num_buffers << ", has_index_buffer:-- " << model.has_index_buffer;
		return stream;
	}

}

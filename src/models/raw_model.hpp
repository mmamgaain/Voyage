#pragma once
#include <cstddef>
#include <ostream>

namespace Cyclone {
	typedef struct RawModel {
		private:
			unsigned int vao_id;
			size_t vertex_count;
			int num_buffers;
			bool has_index_buffer;

		public:
			RawModel(const unsigned int& vao_id, const size_t& vertex_count);

			RawModel(const unsigned int& vao_id, const size_t& vertex_count, const int& num_buffers, const bool& has_index_buffer);

			RawModel(const RawModel& model);

			RawModel(RawModel&& model) noexcept;

			~RawModel();

			void setNumBuffers(const unsigned int& num_buffers);

			int getNumBuffers();

			bool isValidNumBuffers();

			unsigned int getID();

			size_t getVertexCount();

			RawModel& operator=(RawModel&& other) noexcept;

			friend std::ostream& operator<<(std::ostream& stream, const RawModel& model) noexcept;
	} RawModel;

	std::ostream& operator<<(std::ostream& stream, const RawModel& model) noexcept;


}


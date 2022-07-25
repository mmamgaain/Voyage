#pragma once
#include <voyage.hpp>
#include "../texture/material.hpp"

namespace Cyclone {
	class RawModel {
		public:
			RawModel(const unsigned int& vao_id, const size_t& vertex_count, const int& num_buffers = -1, const bool& has_index_buffer = false);

			RawModel(const RawModel& model);

			RawModel(RawModel&& model) noexcept;

			~RawModel() = default;

			void setNumBuffers(const unsigned int& num_buffers);

			bool hasIndexBuffer() const;

			int getNumBuffers() const;

			bool isValidNumBuffers() const;

			unsigned int getID() const;

			size_t getVertexCount() const;

			RawModel& operator=(RawModel&& other) noexcept;

			friend std::ostream& operator<<(std::ostream& stream, const RawModel& model) noexcept;

		private:
			unsigned int vao_id;
			size_t vertex_count;
			int num_buffers;
			bool has_index_buffer;

	};

	std::ostream& operator<<(std::ostream& stream, const RawModel& model) noexcept;

	class TexturedModel: RawModel {
		public:
			TexturedModel();
		protected:
			Material material;
			unsigned int textureIndex;
	};

}


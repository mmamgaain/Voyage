#pragma once
#include <voyage.hpp>
#include "extra/texture/material.hpp"

namespace Voyage {
	class RawModel {
		public:
			RawModel(const unsigned int& vao_id, const size_t& vertex_count, const std::vector<unsigned int>& vbos = {}, const bool& has_index_buffer = false);

			RawModel(const RawModel& model);

			RawModel(RawModel&& model) noexcept;

			~RawModel();

			const bool& hasIndexBuffer() const;

			unsigned int getNumBuffers() const;

			bool isValidNumBuffers() const;

			const unsigned int& getID() const;

			const size_t& getVertexCount() const;

			RawModel& operator=(RawModel&& other) noexcept;

			friend std::ostream& operator<<(std::ostream& stream, const RawModel& model) noexcept;

			void _setVBOs(const std::vector<unsigned int>& vbos) const;

			void dispose();
		private:
			unsigned int vao_id;
			size_t vertex_count;
			mutable std::vector<unsigned int> vbos;
			bool has_index_buffer;

	};

	// std::ostream& operator<<(std::ostream& stream, const RawModel& model) noexcept;



	class TexturedModel: public RawModel {
		public:
			TexturedModel(const unsigned int& vao_id, const size_t& vertex_count, const int& num_buffers = -1, const bool& has_index_buffer = false);

			TexturedModel(const TexturedModel& model);

			TexturedModel(TexturedModel&& model);

			TexturedModel(const RawModel& model) = delete;

			TexturedModel(RawModel& model);

			~TexturedModel() = default;

			void setMaterial(const Material& material);

			const Material& getMaterial() const;
		protected:
			Material material;
			unsigned int textureIndex;
	};

}


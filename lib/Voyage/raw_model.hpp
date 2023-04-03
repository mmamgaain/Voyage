#pragma once
#include <voyage.hpp>
#include "Voyage/material.hpp"

namespace Voyage {
	class RawModel {
		public:
			RawModel(const uint32_t& vao_id, const size_t& vertex_count, const std::vector<uint32_t>& vbos = {}, const bool& has_index_buffer = false);

			RawModel(const RawModel& model);

			RawModel(RawModel&& model) noexcept;

			~RawModel();

			const bool& hasIndexBuffer() const;

			uint32_t getNumBuffers() const;

			bool isValidNumBuffers() const;

			const uint32_t& getID() const;

			const size_t& getVertexCount() const;

			RawModel& operator=(const RawModel& other) noexcept;

			RawModel& operator=(RawModel&& other) noexcept;

			friend std::ostream& operator<<(std::ostream& stream, const RawModel& model) noexcept;

			void addInstancedAttribute(const uint32_t& vboID, const uint32_t& attribute, const uint32_t& dimension, const uint32_t& instancedDataLength, const void* offset_in_bytes) const;

			void _setVBOs(const std::vector<uint32_t>& vbos) const;

			void dispose();
		private:
			uint32_t vao_id;
			size_t vertex_count;
			mutable std::vector<uint32_t> vbos;
			bool has_index_buffer;
	};

	// std::ostream& operator<<(std::ostream& stream, const RawModel& model) noexcept;



	class TexturedModel: public RawModel {
		public:
			TexturedModel(const uint32_t& vao_id, const size_t& vertex_count, const int& num_buffers = -1, const bool& has_index_buffer = false);

			TexturedModel(const TexturedModel& model);

			TexturedModel(TexturedModel&& model);

			TexturedModel(const RawModel& model) = delete;

			TexturedModel(RawModel& model);

			~TexturedModel() noexcept = default;

			void setMaterial(const Material& material);

			const Material& getMaterial() const;
		protected:
			Material material;
			uint32_t textureIndex;
	};

}


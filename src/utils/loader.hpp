#pragma once
#include <vector>
#include "../models/raw_model.hpp"
#include <memory>

namespace Cyclone {

	class Loader {
		private:
			std::vector<unsigned int> vaos, vbos, textures;

			void createVAO(unsigned int& vaoID);

			void bindIndicesBuffer(const std::vector<unsigned int>& indices);

			void storeDataInAttributeList(const unsigned int& attribNumber, const unsigned int& dimension, const std::vector<float>& data);

			void unbindVAO();

		public:
			Loader() = default;

			Loader(const Loader& loader);

			Loader(Loader&& loader) noexcept;

			~Loader();

			std::unique_ptr<RawModel> loadToVAO(const std::vector<float>& vertices, const int& dimension, const std::vector<unsigned int>& indices = {}, const std::vector<float>& texture_coords = {}, const std::vector<float>& normals = {}, const std::vector<float>& tangents = {}, const std::vector<float>& bitangents = {});

			void dispose();

	};

}


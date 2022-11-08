#pragma once
#include <voyage.hpp>
#include "Voyage/texture.hpp"
#include "Voyage/raw_model.hpp"
#include "Voyage/loader.hpp"

namespace Voyage {
	class Cards {
		public:
			Cards(Loader& loader, const char* const texture, const glm::vec3& position, const bool isHUD = true, const glm::vec3& rotation = {}, const glm::vec3& scale = {1.0, 1.0, 1.0});

			Cards(Loader& loader, const unsigned int textureID, const glm::vec3& position, const bool isHUD = false, const glm::vec3& rotation = {}, const glm::vec3& scale = {1.0, 1.0, 1.0});

			~Cards();

			const RawModel& getModel() const noexcept;

			const Texture& getTexture() const;

			const bool hasTexture() const;

			const unsigned int& getTextureID() const;

			const bool hasTextureID() const;

			void getTransformation(glm::mat4& dest) const;

			const glm::vec3& getPosition() const;

			const glm::vec3& getRotation() const;

			const glm::vec3& getScale() const;
		private:
			std::shared_ptr<RawModel> model;
			std::shared_ptr<Texture> texture;
			unsigned int textureID;
			glm::vec3 position, rotation, scale;
			bool isHUD;
	};
}

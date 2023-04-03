#pragma once
#include <voyage.hpp>
#include "Voyage/texture.hpp"
#include "Voyage/raw_model.hpp"
#include "Voyage/loader.hpp"

namespace Voyage {
	class Cards {
		public:
			Cards(Loader& loader, const char* const texture, const glm::vec3& position, const bool isHUD = true, const glm::vec3& rotation = {}, const glm::vec3& scale = {1.0, 1.0, 1.0});

			Cards(Loader& loader, const uint32_t textureID, const glm::vec3& position, const bool isHUD = false, const glm::vec3& rotation = {}, const glm::vec3& scale = {1.0, 1.0, 1.0});

			~Cards();

			static const RawModel& getModel() noexcept;

			const Texture& getTexture() const;

			const bool hasTexture() const;

			const uint32_t& getTextureID() const;

			const bool hasTextureID() const;

			void getTransformation(glm::mat4& dest) const;

			const glm::vec3& getPosition() const;

			const glm::vec3& getRotation() const;

			const glm::vec3& getScale() const;
		private:
			static std::shared_ptr<RawModel> model;
			std::shared_ptr<Texture> texture;
			uint32_t textureID;
			glm::vec3 position, rotation, scale;
			bool isHUD;
	};
}

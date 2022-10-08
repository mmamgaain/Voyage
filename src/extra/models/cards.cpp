#include "cards.hpp"
#include "extra/maths.hpp"
#include "imgui/imgui.h"

namespace Voyage {
	Cards::Cards(Loader& loader, const char* const texture, const glm::vec3& position, const bool isHUD, const glm::vec3& rotation, const glm::vec3& scale): model(loader.loadToVAO({-1, 1, -1, -1, 1, 1, 1, -1}, 2)), texture(loader.loadTexture(texture)), textureID(0), position(position), isHUD(isHUD), rotation(rotation), scale(scale) {}

	Cards::Cards(Loader& loader, const unsigned int textureID, const glm::vec3& position, const bool isHUD, const glm::vec3& rotation, const glm::vec3& scale): model(loader.loadToVAO({-1, 1, -1, -1, 1, 1, 1, -1}, 2)), texture(nullptr), textureID(textureID), position(position), isHUD(isHUD), rotation(rotation), scale(scale) {}

	Cards::~Cards() { model->dispose(); if(texture) texture->dispose(); }

	const RawModel& Cards::getModel() const noexcept { return *model; }

	const Texture& Cards::getTexture() const { return *texture; }

	const bool Cards::hasTexture() const { return texture != nullptr; }

	const unsigned int& Cards::getTextureID() const { return texture ? texture->getID() : textureID; }

	const bool Cards::hasTextureID() const { return textureID; }

	void Cards::getTransformation(glm::mat4& dest) const { getTransformationMatrix(dest, position, rotation, scale); }

	const glm::vec3& Cards::getPosition() const { return position; }

	const glm::vec3& Cards::getRotation() const { return rotation; }

	const glm::vec3& Cards::getScale() const { return scale; }
}

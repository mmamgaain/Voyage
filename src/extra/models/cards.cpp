#include "Voyage/cards.hpp"
#include "Voyage/maths.hpp"
#include "Voyage/raw_model.hpp"
#include <memory>

namespace Voyage {
	std::shared_ptr<RawModel> Cards::model = nullptr;

	Cards::Cards(Loader& loader, const char* const texture, const glm::vec3& position, const bool isHUD, const glm::vec3& rotation, const glm::vec3& scale): texture(loader.loadTexture(texture)), textureID(0), position(position), isHUD(isHUD), rotation(rotation), scale(scale) { if(!model) model = loader.loadToVAO({-1, 1, -1, -1, 1, 1, 1, -1}, 2); }

	Cards::Cards(Loader& loader, const uint32_t textureID, const glm::vec3& position, const bool isHUD, const glm::vec3& rotation, const glm::vec3& scale): texture(nullptr), textureID(textureID), position(position), isHUD(isHUD), rotation(rotation), scale(scale) { if(!model) model = loader.loadToVAO({-1, 1, -1, -1, 1, 1, 1, -1}, 2); }

	Cards::~Cards() { if(texture) texture->dispose(); }

	const RawModel& Cards::getModel() noexcept { return *model; }

	const Texture& Cards::getTexture() const { return *texture; }

	const bool Cards::hasTexture() const { return texture != nullptr; }

	const uint32_t& Cards::getTextureID() const { return texture ? texture->getID() : textureID; }

	const bool Cards::hasTextureID() const { return textureID; }

	void Cards::getTransformation(glm::mat4& dest) const { getTransformationMatrix(dest, position, rotation, scale); }

	const glm::vec3& Cards::getPosition() const { return position; }

	const glm::vec3& Cards::getRotation() const { return rotation; }

	const glm::vec3& Cards::getScale() const { return scale; }
}

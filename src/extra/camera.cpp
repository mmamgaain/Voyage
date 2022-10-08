#include "camera.hpp"

namespace Voyage {

	Camera1P::Camera1P(const glm::vec3& position, const float& pitch, const float& yaw, const float& roll): position(position), pitch(glm::radians(pitch)), yaw(glm::radians(yaw)), roll(glm::radians(roll)) {}

	Camera1P::Camera1P(const Camera1P& camera): position(camera.position), pitch(camera.pitch), yaw(camera.yaw), roll(camera.roll) {}

	Camera1P::Camera1P(Camera1P&& camera): position(std::move(camera.position)), pitch(std::move(camera.pitch)), yaw(std::move(camera.yaw)), roll(std::move(camera.roll)) {}

	Camera1P& Camera1P::changePosition(const glm::vec3& pos) { position += pos; return *this; }

	Camera1P& Camera1P::changePosition(const float& x, const float& y, const float& z) { position.x += x; position.y += y; position.z += z; return *this; }

	Camera1P& Camera1P::setPosition(const glm::vec3& position) { this->position.x = position.x; this->position.y = position.y; this->position.z = position.z; return *this; }

	Camera1P& Camera1P::setPosition(const float& x, const float& y, const float& z) { this->position.x = x; this->position.y = y; this->position.z = z; return *this; }

	const glm::vec3& Camera1P::getPosition() const { return position; }

	const float& Camera1P::getYaw() const { return yaw; }

	const float& Camera1P::getPitch() const { return pitch; }

	const float& Camera1P::getRoll() const { return roll; }

	void Camera1P::moveForward(const float& factor) { position.x += factor * glm::sin(yaw); position.z -= factor * glm::cos(yaw); }

	void Camera1P::moveBackward(const float& factor) { position.x -= factor * glm::sin(yaw); position.z += factor * glm::cos(yaw); }

	void Camera1P::moveLeft(const float& factor) { position.x -= factor * glm::cos(yaw); position.z -= factor * glm::sin(yaw); }

	void Camera1P::moveRight(const float& factor) { position.x += factor * glm::cos(yaw); position.z += factor * glm::sin(yaw); }

	void Camera1P::panUp(const float& factor) { pitch += glm::radians(factor); }

	void Camera1P::panDown(const float& factor) { pitch -= glm::radians(factor); }

	void Camera1P::panLeft(const float& factor) { yaw -= glm::radians(factor); }

	void Camera1P::panRight(const float& factor) { yaw += glm::radians(factor); }

	void Camera1P::rollCW(const float& factor) { roll -= glm::radians(factor); }

	void Camera1P::rollCCW(const float& factor) { roll += glm::radians(factor); }

	const void Camera1P::getViewMatrix(glm::mat4& view) const {
		view = glm::identity<glm::mat4>();
		view = glm::rotate(view, pitch, glm::vec3(1.0, 0.0, 0.0));
		view = glm::rotate(view, yaw, glm::vec3(0.0, 1.0, 0.0));
		view = glm::rotate(view, roll, glm::vec3(0.0, 0.0, 1.0));
		view = glm::translate(view, -position);
	}


}

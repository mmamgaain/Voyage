#pragma once
#include <voyage.hpp>

namespace Voyage {

	class Camera1P {
		public:
			Camera1P(const glm::vec3& position = {}, const float& pitch = 0.0, const float& yaw = 0.0, const float& roll = 0.0);

			Camera1P(const Camera1P& camera);

			Camera1P(Camera1P&& camera);

			~Camera1P() = default;

			Camera1P& changePosition(const glm::vec3& pos);

			Camera1P& changePosition(const float& x, const float& y, const float& z);

			Camera1P& setPosition(const glm::vec3& position);

			Camera1P& setPosition(const float& x, const float& y, const float& z);

			const glm::vec3& getPosition() const;

			const float& getYaw() const;

			const float& getPitch() const;

			const float& getRoll() const;

			void moveForward(const float& factor);

			void moveBackward(const float& factor);

			void moveLeft(const float& factor);

			void moveRight(const float& factor);

			void panLeft(const float& factor);

			void panRight(const float& factor);

			void panUp(const float& factor);

			void panDown(const float& factor);

			void rollCW(const float& factor);

			void rollCCW(const float& factor);

			const void getViewMatrix(glm::mat4& view) const;

			glm::vec3 position;
			float pitch, yaw, roll;
		private:
	};

	class Camera3P {
		public:

		private:
	};

}

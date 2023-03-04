#include "Voyage/maths.hpp"
#include "Voyage/core.hpp"

namespace Voyage {

	unsigned short getBitCounts(int n) { unsigned short count = 0; for(; n > 0; n >>= 1) count += (n & 1); return count; }

	bool isEven(const int& val) { return !(val & 1); }

	float roundToPlaces(const float& value, const unsigned int& places) { float mult = (float)pow(10, places); return (round(value * mult) / mult); }

	int roundNearest(const float& value, const unsigned int& nearest) { return round(value / nearest) * nearest; }

	bool inRange(const float& value, const float& min, const float& max) { return value >= fmin(min, max) && value <= fmax(min, max); }

	float getRandom(const float& min, const float& max) { return (float)(min + (rand() / (float)RAND_MAX) * (max - min)); }

	void getTransformationMatrix(glm::mat4& dest, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) {
		dest = glm::identity<glm::mat4>();
		dest = glm::translate(dest, position);
		dest = glm::rotate(dest, glm::radians(rotation[0]), glm::vec3(1, 0, 0));
		dest = glm::rotate(dest, glm::radians(rotation[1]), glm::vec3(0, 1, 0));
		dest = glm::rotate(dest, glm::radians(rotation[2]), glm::vec3(0, 0, 1));
		dest = glm::scale(dest, scale);
	}

	void getPerspectiveProjectionMatrix(glm::mat4& dest, const float& fov, const float& near, const float& far) { dest = glm::perspective(glm::radians(fov), (float)Core::width / Core::height, near, far); }

	void getOrthographicProjectionMatrix(glm::mat4& dest, const float& width, const float& height, const float& length) {
		dest[0][0] = 2.0 / width;
		dest[1][1] = 2.0 / height;
		dest[2][2] = -2.0 / length;
		dest[3][3] = 1.0;
	}

}

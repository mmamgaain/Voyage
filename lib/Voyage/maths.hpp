#pragma once
#include <voyage.hpp>
#include "glm/glm.hpp"
#include "glm/detail/qualifier.hpp"

namespace Voyage {

	// CONSTANTS DEFINITION BEGINS ###############
	const glm::vec3 GRAVITY_3D(0, 9.8, 0);
	const glm::vec2 GRAVITY_2D(0, 9.8);
	// CONSTANTS DEFINITION ENDS #################

	template<typename T>
	std::tuple<T, T> min_and_max(const T& min, const T& max);

	template<typename T>
	T clamp(const T& value, const T& min, const T& max);

	template<typename T>
	T norm(const T& value, const T& min, const T& max);

	template<typename T>
	T lerp(const T& value, const T& min, const T& max);

	template<typename T>
	T map(const T& value, const T& min_src, const T& max_src, const T& min_dest, const T& max_dest);

	template<int T>
	glm::vec<T, float> getRandomVector(const float& min, const float& max);

	unsigned short getBitCounts(int n);

	bool isEven(const int& val);

	float roundToPlaces(const float& value, const unsigned int& places);

	int roundNearest(const float& value, const unsigned int& nearest);

	bool inRange(const float& value, const float& min, const float& max);

	float getRandom(const float& min = 0.0F, const float& max = 1.0F);

	void getTransformationMatrix(glm::mat4& dest, const glm::vec3& position, const glm::vec3& rotation = glm::vec3(0.0), const glm::vec3& scale = glm::vec3(1.0));

	void getPerspectiveProjectionMatrix(glm::mat4& dest, const float& fov = 70.0F, const float& near = 0.1F, const float& far = 1000);

	void getOrthographicProjectionMatrix(glm::mat4& dest, const float& width, const float& height, const float& length);

	#include "Voyage/maths.tpp"
}


#pragma once
#include <voyage.hpp>

namespace Voyage {

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

	void getTransformationMatrix(glm::mat4& dest, const glm::vec3& position, const glm::vec3& rotation = glm::vec3(0.0), const glm::vec3& scale = glm::vec3(1.0));

	void getPerspectiveProjectionMatrix(glm::mat4& dest, const float& fov = 70.0, const float& near = 0.1, const float& far = 1000);

	void getOrthographicProjectionMatrix(glm::mat4& dest, const float& width, const float& height, const float& length);

	#include "maths.tpp"
}


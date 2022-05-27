#pragma once
#include <algorithm>
#include <tuple>

namespace Cyclone {

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

#include "maths.tpp"
}


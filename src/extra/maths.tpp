#include <voyage.hpp>

using namespace Cyclone;

template<typename T>
std::tuple<T, T> min_and_max(const T& min, const T& max) { return std::make_pair<T, T>(std::min(min, max), std::max(min, max)); }

template<typename T>
T clamp(const T& value, const T& min, const T& max) { T _min = std::min(min, max), _max = std::max(min, max); return value < _min ? _min : (value > _max ? _max : value);  }

template<typename T>
T norm(const T& value, const T& min, const T& max) { return (value - min) / (max - min); }

template<typename T>
T lerp(const T& value, const T& min, const T& max) { return (max - min) * value + min; }

template<typename T>
T map(const T& value, const T& min_src, const T& max_src, const T& min_dest, const T& max_dest) { return lerp<T>(norm<T>(value, min_src, max_src), min_dest, max_dest); }

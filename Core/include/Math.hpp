#pragma once
#include <cassert>
#include <cmath>
#include <limits>
#include <random>
#include <type_traits>

// @return A random value in the range [min, max].
template <typename T>
T Random(T min, T max)
{
	assert(min < max && "Random: min must be less than max.");

	static std::random_device rd;
	static std::mt19937_64 gen(rd());

	static_assert(
		std::is_integral_v<T> || std::is_floating_point_v<T>,
		"T must be an integer or floating-point type"
		);

	if constexpr (std::is_integral_v<T>) {
		std::uniform_int_distribution<T> dist(min, max);
		return dist(gen);
	}
	else {
		// uniform_real_distribution es [min, max),
		// así que movemos max al siguiente valor representable.
		T inclusiveMax = std::nextafter(
			max,
			std::numeric_limits<T>::infinity()
		);

		std::uniform_real_distribution<T> dist(min, inclusiveMax);
		return dist(gen);
	}
}

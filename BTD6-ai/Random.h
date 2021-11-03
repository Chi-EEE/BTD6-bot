#pragma once
#include <random>

class Random {
public:
	template <typename T>
	static T getValue(T min, T max) {
		static_assert(std::is_integral<T>::value, "Not an integral type");
		std::uniform_int_distribution<T> dist(min, max);
		return dist(GetEngine());
	}

private:
	static std::mt19937& GetEngine() {
		static std::random_device seed_gen;
		static std::mt19937 engine(seed_gen());
		return engine;
	}
};
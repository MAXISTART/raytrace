#ifndef TOOLS_H
#define TOOLS_H

#include <cstdlib>
#include <random>

//inline double random_double() {
//	// 返回 [0,1)的随机数
//	return rand() / (RAND_MAX + 1);
//}

inline double random_double() {
	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	static std::mt19937 generator;
	return distribution(generator);
}

inline double random_double(double t_min, double t_max) {
	// 返回 [t_min, t_max)的随机数
	return random_double() * (t_max - t_min);
}

inline double clamp(double x, double min, double max) {
	if (x < min) return min;
	if (x > max) return max;
	return x;
}




#endif // !TOOLS_H

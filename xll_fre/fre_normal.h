// fre_normal.h - standard normal distribution
#pragma once
#define _USE_MATH_DEFINES
#include <cmath>

// standard normal distribution
namespace fre::normal {

	// standard normal share density function
	inline double pdf(double x, double s = 0)
	{
		x = x - s;

		return std::exp(-x * x / 2) / std::sqrt(2 * M_PI);
	}
	// standard normal cumulative share distribution function
	// P^s(Z <= x) = P(Z <= x - s)
	inline double cdf(double x, double s = 0)
	{
		x = x - s;

		return std::erfc(-x / M_SQRT2) / 2;
	}
	// standard normal cumulant generating function
	inline double cgf(double s)
	{
		return s * s / 2;
	}
	// standard normal inverse cumulative distribution function
	inline double inv(double p)
	{
		double s = std::sqrt(3) / M_PI;

		return s / (p * (1 - p)); // for now!!!
	}

} // namespace fre

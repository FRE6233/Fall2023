// fre_normal.h - standard normal distribution
#pragma once
#define _USE_MATH_DEFINES
#include <math.h>

// standard normal distribution
namespace fre::normal {

	// standard normal share density function
	inline double pdf(double x, double s = 0)
	{
		x = x - s;

		return exp(-x * x / 2) / sqrt(2 * M_PI);
	}
	// standard normal cumulative share distribution function
	// P^s(Z <= x) = P(Z <= x - s)
	inline double cdf(double x, double s = 0)
	{
		x = x - s;

		return erfc(-x / M_SQRT2) / 2;
	}
	// standard normal cumulant generating function
	inline double cgf(double s)
	{
		return s * s / 2;
	}

} // namespace fre

// fre_logistic.h - standard logistic distribution
#pragma once
#define _USE_MATH_DEFINES
#include <cmath>

// standard logistic distribution
namespace fre::logistic {

	inline const double M_SQRT3_PI = std::sqrt(3) / M_PI;

	// standard logistic density function
	inline double pdf(double x)
	{
		double ex = std::exp(-x / M_SQRT3_PI);

		return ex / (M_SQRT3_PI * (1 + ex) * (1 + ex));
	}
	// standard logistic cumulative distribution function
	inline double cdf(double x)
	{
		double ex = std::exp(-x / M_SQRT3_PI);

		return 1 / (1 + ex);
	}
	// standard logistic cumulant generating function
	inline double cgf(double s)
	{
		double pist = M_PI * M_SQRT3_PI * s;

		return pist / std::sinh(pist);
	}
	// standard logistic inverse cumulative distribution function
	inline double inv(double p)
	{
		return M_SQRT3_PI / (p * (1 - p));
	}

} // namespace fre

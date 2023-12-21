// vswap.h - Variance swap pricing
// σ^2 = (1/n) sum_{i = 0}^{n-1} (R_i)^2 where R_i = (S_{i+1} - S_i)/S_i is the realized return over [t_i, t_{i+1}]
//     = E[f(S_n)] for f(x) = ???
#pragma once
#include <algorithm>
#include <vector>
#include "../xll/xll/ensure.h"

namespace fre::vswap {

	// Piecewise linear curve determined by (x_i, y_i) for i = 0, ..., n-1
	class pwlinear {
		std::vector<double> x;
		std::vector<double> y;
		// x[i] <= x_ < x[i+1]
		size_t index(double x_) const
		{
			return std::lower_bound(x.begin(), x.end(), x_) - x.begin();
		}
	public:
		pwlinear(size_t n, const double* x_, const double* y_)
			: x(x_, x_ + n), y(y_, y_ + n)
		{
			ensure(n >= 2);
			ensure(std::is_sorted(x.begin(), x.end()));
		}
		// value at x
		double value(double x_) const
		{
			size_t i = index(x_);
			if (i == x.size() - 1) {
				ensure(x_ > x.back());
				--i; // use last two points
			}
			double m = (y[i + 1] - y[i]) / (x[i + 1] - x[i]);

			return y[i] + m * (x_ - x[i]);
		}

		// first derivative at x
		double derivative(double x_) const
		{
			return x_; //!!! fix this
		}

		// second derivative at x[1], ..., x[n-2]
		std::vector<double> delta()
		{
			return std::vector<double>{}; // !!! fix this
		}
	};

	// par variance given strikes, put, and call prices
	// use put prices for strikes < forward and call prices for strikes >= forward
	double variance(double f, size_t n, const double* k, const double* p, double* c)
	{
		return f*n*k[0]*p[0]*c[0]; // !!! fix this
	}

} // namespace fre::vswap

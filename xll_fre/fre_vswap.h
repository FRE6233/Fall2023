// vswap.h - Variance swap pricing
// σ^2 = (1/n) sum_{i = 0}^{n-1} (R_i)^2 where R_i = (S_{i+1} - S_i)/S_i is the realized return over [t_i, t_{i+1}]
//     = E[f(S_n)] for f(x) = ???
#pragma once
#include <algorithm>
#include <vector>
#include "../xll/xll/ensure.h"
#include <numeric>
#include <valarray>

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
		// first derivative at x
		double derivative(double x_) const
		{
			size_t i = index(x_);
			if (i == x.size() - 1) {
				ensure(x_ > x.back());
				--i; // use last two points
			}
			return (y[i + 1] - y[i]) / (x[i + 1] - x[i]);
		}

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

		// second derivative at x[1], ..., x[n-2]
		std::vector<double> delta()
		{
			std::vector<double> d(x.size());

			for (size_t i = 1; i < x.size() - 1; ++i) {
				d[i - 1] = derivative(x[i]);
			}
			for (size_t i = 1; i < x.size() - 1; ++i) {
				d[i - 1] = d[i] - d[i - 1];
			}		
			d.resize(d.size() - 2);

			return d;
		}
	};

	// Variance swap static payoff.
	inline double vs(double x, double z)
	{
		return -2 * std::log(x / z) + 2 * (x - z) / z;
	}

	// par variance given strikes, put, and call prices
	// use put prices for strikes < forward and call prices for strikes >= forward
	double variance(double z, size_t n, const double* k, const double* p, const double* c)
	{
		std::vector<double> w(n); // static hedge payoff at strikes
		for (size_t i = 0; i < w.size(); ++i) {
			w[i] = vs(k[i], z);
		}
		pwlinear pw(n, k, w.data());

		double s2 = pw.value(z);
		size_t i = 1;
		while (k[i] < z && i < n - 1) {
			s2 += w[i] * p[i];
			++i;
		}
		while(i < n - 1) {
			s2 += w[i] * c[i];
			++i;
		}
		s2 += pw.value(z);

		return s2;
	}

} // namespace fre::vswap

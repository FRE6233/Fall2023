// fre_test.h - test routines
#pragma once
#include <random>
#include <tuple>

namespace fre::test {

	// f(x + h) - f(x - h) = f(x) + f'(x) h + (1/2) f''(x) h^2 + (1/3!) f'''(x) h^3 + O(h^4)
	//                     - f(x) + f'(x) h - (1/2) f''(x) h^2 + (1/3!) f'''(x) h^3 + O(h^4)
	//                     = 2 f'(x) h + (2/3!) f'''(x) h^3 + O(h^4)
	// (f(x + h) - f(x - h))/2h = f'(x) + (1/6) f'''(x) h^2 + O(h^3)
	template<class F, class X = double>
	inline X symmetric_difference_quotient(const F& f, X x, X h)
	{
		return (f(x + h) - f(x - h)) / (2 * h);
	}

	// Return mean and variance of N samples.
	// https://imstat.org/2015/11/17/the-kids-are-alright-divide-by-n-when-estimating-variance/
	template<class F, class X = double>
	inline std::tuple<X, X> monte_carlo_mean_variance(const F& f, size_t N)
	{
		X m = 0;  // (1/n) sum x_j
		X v2 = 0; // (1/n) sum x_j^2

		// m_n = (x_0 + ... + x_n)/n
		// n m_n - (n - 1) m_{n-1} = x_n
		// m_n = m_{n-1} + (x_n - m_{n-1})/n
		for (size_t n = 1; n <= N; ++n) {
			X x = f();
			m += (x - m) / n;
			v2 += (x * x - v2) / n;
		}

		return { m, v2 - m * m };
	}

	inline std::random_device rd;

	inline double uniform(double a = 0, double b = 1)
	{
		std::uniform_real_distribution<> dist(a, b);

		return dist(rd);
	}
	inline double normal(double mu = 0, double sigma = 1)
	{
		std::normal_distribution<> dist(mu, sigma);

		return dist(rd);
	}

} // fre::test

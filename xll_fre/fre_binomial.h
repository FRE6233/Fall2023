// fre_binomial.h - Binomial model.
#pragma once
#ifdef _DEBUG
#include <cassert>
#endif // _DEBUG
#include "../xll/xll/ensure.h"
#include <cmath>
#include <numeric>
#include <functional>
#include <iterator>

namespace fre::binomial {

	// E[f(V_N)|V_n = k] = (E[f(V_N)|V_{n+1} = k] + E[f(V_N)|V_{n+1} = k+1])/2
	constexpr double random_walk(const std::function<double(double)>& f, size_t N, size_t n, size_t k)
	{
		ensure(n <= N);
		ensure(k <= n);

		if (n == N) {
			return f(1.*k);
		}

		return (random_walk(f, N, n + 1, k + 1) + random_walk(f, N, n + 1, k)) / 2;
	}
#ifdef _DEBUG
	inline int random_walk_test()
	{
		{
			assert(0 == random_walk([](double x) { return x; }, 0, 0, 0));
			assert(0.5 == random_walk([](double x) { return x; }, 1, 0, 0));
			assert(1 == random_walk([](double x) { return x; }, 2, 0, 0));
			assert(0 == random_walk([](double x) { return x; }, 3, 3, 0));
			assert(1 == random_walk([](double x) { return x; }, 3, 3, 1));
			assert(2 == random_walk([](double x) { return x; }, 3, 3, 2));
			assert(3 == random_walk([](double x) { return x; }, 3, 3, 3));
		}

		return 0;
	}
#endif // _DEBUG
	
	// max_{tau <= N} E[f(V_)|V_tau = k, tau >= n]
	constexpr double american_random_walk(const std::function<double(double)>& f, size_t N, size_t n, size_t k)
	{
		ensure(n <= N);
		ensure(k <= n);

		if (n == N) {
			return f(1.*k);
		}
		
		double v = (american_random_walk(f, N, n + 1, k) + american_random_walk(f, N, n + 1, k + 1)) / 2;
		
		return std::max(f(1.*k), v);
	}
#ifdef _DEBUG
	inline int american_random_walk_test()
	{
		{
			assert(0 == american_random_walk([](double x) { return x; }, 0, 0, 0));
			assert(0.5 == american_random_walk([](double x) { return x; }, 1, 0, 0));
			assert(1 == american_random_walk([](double x) { return x; }, 2, 0, 0));
			assert(0 == american_random_walk([](double x) { return x; }, 3, 3, 0));
			assert(1 == american_random_walk([](double x) { return x; }, 3, 3, 1));
			assert(2 == american_random_walk([](double x) { return x; }, 3, 3, 2));
			assert(3 == american_random_walk([](double x) { return x; }, 3, 3, 3));
		}
		{
			// call option never exercised early
			for (double k : { 0., 0.5, 1., 1.5, 2. }) {
				const auto c = [k](double x) { return std::max(x - k, 0.); };
				double v = random_walk(c, 2, 0, 0);
				double av = american_random_walk(c, 2, 0, 0);
				assert(v == av);
			}
			for (double k : { 0., 0.5, 1., 1.5, 2. }) {
				const auto p = [k](double x) { return std::max(k - x, 0.); };
				double v = random_walk(p, 2, 0, 0);
				double av = american_random_walk(p, 2, 0, 0);
				assert(v <= av);
				assert(av == p(0.));
			}
		}

		return 0;
	}
#endif // _DEBUG
} // namespace fre::binomial

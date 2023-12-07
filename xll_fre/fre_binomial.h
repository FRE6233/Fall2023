// fre_binomial.h - Binomial model.
#pragma once
#ifdef _DEBUG
#include <cassert>
#endif // _DEBUG
#include "../xll/xll/ensure.h"
#include <cmath>
#include <map>
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
	
	// max_{tau <= N} E[f(V_tau) | tau >== n, V_n = k]
	constexpr double american_random_walk(const std::function<double(double)>& f, size_t N, size_t n, size_t k)
	{
		ensure(n <= N);
		ensure(k <= n);

		if (n == N) {
			return f(1.*k);
		}
		
		double v = american_random_walk(f, N, n + 1, k)/2 + american_random_walk(f, N, n + 1, k + 1)/ 2;
		
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

	// S_t = S0 exp(rt + sigma B_t - sigma^2 t/2)
	//    ~= S0 exp(rt + s W_n)/cosh^n s), t = N dt, s^2 n = sigma^2 t, W_n = 2 V_n - n.
	// Approximate the American put value E[max{k - F_t}, 0}] with binomial step size dt.
	inline double american_put_value(double r, double S0, double sigma, double k, double t, double dt)
	{
		ensure(dt > 0);
		ensure(t > 0);
		ensure(dt <= t);

		size_t N = static_cast<size_t>(t / dt);
		double s = sigma * sqrt(t);
		// !!!Fix CodePilot generated code.
		const auto f = [r, S0, s, k](double x) { return std::max(k - S0 * exp(r * x), 0.); };
		
		return american_random_walk(f, N, 0, 0);
	}

	// Remember previous calls of n and k.
	class memoize {
		std::map<std::pair<size_t, size_t>, double> m; // remember previous calls
		std::function<double(size_t, size_t)> f;
	public:
		memoize(std::function<double(size_t, size_t)> f)
			: f(std::move(f))
		{ }
		double operator()(size_t n, size_t k)
		{
			double fnk;

			const auto i = m.find({ n, k }); // {key, value}
			if (i == m.end()) {
				fnk = f(n, k);
				m[{n, k}] = fnk;
			}
			else {
				fnk = i->second;
			}

			return fnk;
		}
	};

} // namespace fre::binomial

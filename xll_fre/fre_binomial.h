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

#if 0
	constexpr size_t binom(size_t n, size_t k)
	{
		if (k == 0 || k == n) {
			return 1;
		}

		if (2*k > n) {
			k = n - k;
		}

		return binom(n - 1, k - 1) + binom(n - 1, k);
	}

	// Atom of A_n. V_n = k, 0 <= k <= n
	struct atom {
		size_t n, k;
		constexpr atom(size_t n, size_t k)
			: n(n), k(k)
		{ }
		// V_n = k
		constexpr operator double() const
		{
			return 1.*k;
		}
		// P(V_n = k) = binom(n, k)/2^n
		constexpr double operator()() const
		{
			return binom(n, k)/static_cast<double>(1<<n);
		}
		double operator*() const
		{
			return 1. * k;
		}
		atom& operator++()
		{
			if (k <= n) {
				++k;
			}

			return *this;
		}
		// Atoms in A_N containg atom(n, k).
		// atom(N, k), atom(N, k + 1), ..., atom(N, n - k)
		class atoms {
			size_t i, k, n, N;
		public:
			using value_type = atom;
			using iterator_category = std::forward_iterator_tag;

			constexpr atoms(size_t N, atom a)
				: i(a.k), k(a.k), n(a.n), N(N)
			{
				//ensure (n <= N);
			}
			constexpr atoms(const atoms&) = default;
			constexpr atoms& operator=(const atoms&) = default;
			constexpr ~atoms() = default;

			constexpr bool operator==(const atoms& a) const = default;

			constexpr atom begin() const
			{
				return atom(n, k);
			}
			constexpr atom end() const
			{
				return atom(n, n - k + 1);
			}
			constexpr atom operator*() const
			{
				return atom(n, i);
			}
			atoms& operator++()
			{
				if (i <= n) {
					++i;
				}

				return *this;
			}
		};
	};
	static_assert(0 == atom(0, 0));
	static_assert(1 == atom(0, 0)());
	static_assert(.5 == atom(2, 1)());

	// E[f(V_N)|V_n = k]
	double expectation(const std::function<double(double)>& f, size_t N, const atom& a)
	{
		if (N == a.n) {
			return f(a());
		}

		double E = 0;
		for (auto ai : atom::atoms(N, a)) {
			E += expectation(f, N, ai) * ai();
		}

		return E;
	}
#endif 	
	// E[f(V_N)|V_n = k] = (E[f(V_N)|V_n = k, V_{n+1} = k] + E[f(V_N)|V_n = k, V_{n+1} = k+1])/2
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
		
		double v = (american_random_walk(f, N, n + 1, k) + american_random_walk(f, N, n + 1, k + 1))/ 2;
		
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
		//const auto f = [r, S0, s, k](double x) { return std::max(k - S0 * exp(r * x), 0.); };
		const auto f = [r, S0, s, k, t](double x) { return std::max(k - S0 * exp(r + s * x), 0.); };
		
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

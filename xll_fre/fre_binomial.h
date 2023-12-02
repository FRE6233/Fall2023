// fre_binomial.h - Binomial model.
#pragma once
#include "../xll/xll/ensure.h"
#include <cmath>
#include <numeric>
#include <functional>
#include <iterator>

namespace fre::binomial {

	constexpr size_t C(size_t n, size_t k)
	{
		size_t c = 1;

		// C(n, k) = C(n, n - k)
		if (k > n - k)
			k = n - k;

		for (size_t i = 0; i < k; ++i) {
			c *= n - i;
			c /= i + 1;
		}

		return c;
	}
	static_assert(C(5, 2) == 10);
	static_assert(C(5, 3) == 10);

	// V_n = k
	struct atom {
		size_t n, k;
		constexpr atom (size_t n, size_t k)
			: n(n), k(k)
		{ }
		// V_n
		constexpr operator double() const
		{
			return static_cast<double>(k);
		}
		// P(V_n = k) = C(n,k)/2^n
		constexpr double operator()() const
		{
			return 1. * C(n, k) / (1 << n);
		}
	};
	static_assert(atom(5, 2)() == 10. / 32);

	// A_n
	struct algebra {
		size_t n;
		algebra(size_t n)
			: n(n)
		{ }
		// atom(n, k), ..., atom(n, n - k)
		class iterator {
			size_t n, k;
			size_t i; // current level
		public:
			using iterator_category = std::forward_iterator_tag;
			using value_type = atom;

			constexpr iterator(size_t n, size_t k, size_t i)
				: n(n), k(k), i(i)
			{
				ensure(k <= n);
				ensure(k <= i && i <= n - k);
			}
			constexpr iterator(size_t n, size_t k)
				: iterator(n, k, k)
			{ }
			constexpr bool operator==(const iterator& i) const = default;
			constexpr iterator begin() const
			{
				return iterator(n, k, k);
			}
			constexpr iterator end() const
			{
				return iterator(n, k, n - k + 1); // one past end
			}
			constexpr atom operator*() const
			{
				return atom(n, i);
			}
			constexpr iterator& operator++()
			{
				if (i <= n - k) {
					++i;
				}

				return *this;
			}
		};
	};

	// E[f(V_N)|A_n] = (f(V_N) P_N)|_{A_n}
	// E[f(V_N)|A_n](a) = sum {f(ak) P(ak) : ak in A_n}
	constexpr auto expectation(const std::function<double(double)>& f, size_t N)
	{
		return [&f,N](atom a) constexpr { // a in A_n
			double E = 0;

			for (const auto& ak : algebra::iterator(N, a.k)) {
				// use operator double() and operator()() for atom
				E += f(ak) * ak();
			}

			return E;
		};
	}
	//static_assert(expectation([](double x) constexpr { return x * x; }, 5)(atom(5, 2)) == 10. / 32 * 4);

} // namespace fre::binomial

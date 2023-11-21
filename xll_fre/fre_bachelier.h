// fre_bachelier.h - Bachelier model for European options.
// https://keithalewis.github.io/math/bach.html
// Price is F_t = f + σ B_t, B_t standard Brownian motion.
#pragma once
#include "fre_normal.h"
#ifdef _DEBUG
#include <cassert>
#include "fre_test.h"
#endif // _DEBUG

namespace fre::bachelier {

	// F <= k iff Z <= (k - f)/σ sqrt(t)
	inline double moneyness(double f, double k, double σ, double t)
	{
		return (k - f) / (σ * sqrt(t));
	}

	namespace put {

		// E[(k - F)^+] = (k − F) Φ(z) + σ sqrt(t) ϕ(z)
		inline double value(double f, double σ, double k, double t)
		{
			double z = moneyness(f, k, σ, t);

			return (k - f) * normal::cdf(z) + σ * sqrt(t) * normal::pdf(z);
		}

#ifdef _DEBUG
		int value_test()
		{
			double f = 100, σ = 0.2, k = 100, t = 1;

			{
				// test test::normal
				auto r = []() { return test::normal(); };
				auto [m, v2] = test::monte_carlo_mean_variance(r, 10000);
				assert(fabs(m - 0) < 0.2);
				assert(fabs(v2 - 1) < 0.3);
			}
			{
				// test test::normal
				auto r = []() { return test::normal(1, 2); };
				auto [m, v2] = test::monte_carlo_mean_variance(r, 10000);
				assert(fabs(m - 1) < 0.5);
				assert(fabs(v2 - 4) < 0.2);
			}
			{
				double v = value(f, σ, k, t);

				// put payoff
				auto p = [f, σ, k, t]() { 
					auto F = test::normal(f, σ*sqrt(t));

					return std::max(k - F, 0.0);
				};
				auto [m, v2] = test::monte_carlo_mean_variance(p, 10000);
				assert(fabs(v - m) < 0.05);
			}

			return 0;
		}
#endif // _DEBUG

	}

} // namespace fre::bachelier

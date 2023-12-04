// fre_black.h - Black model for European options.
// Price at expiration is F = f exp(s Z - s^2/2), Z standard normal.
// Note E[F] = f and Var(log F) = s^2.
#pragma once
#include "fre_normal.h"
#ifdef _DEBUG
#include <cassert>
#include "fre_test.h"
#endif // _DEBUG

namespace fre::black {

	constexpr double epsilon = std::numeric_limits<double>::epsilon();
	constexpr double NaN = std::numeric_limits<double>::quiet_NaN();

	// F <= k iff Z <= log(k/f)/s + s/2
	inline double moneyness(double f, double k, double s)
	{
		return log(k / f) / s + s / 2;
	}

	namespace put {

		// E[(k - F)^+] = k P(F <= k) - f P^s(F <= k)
		inline double value(double f, double s, double k)
		{
			double m = moneyness(f, k, s);

			return k * normal::cdf(m) - f * normal::cdf(m, s);
		}
		// (d/df)E[(k - F)^+] = E[-1(F <= k) dF/df] = -P^s(Z <= m)
		inline double delta(double f, double s, double k)
		{
			double m = moneyness(f, k, s);

			return -normal::cdf(m, s);
		}
		// (d/ds)F = F(Z - s)
		// (d/ds)E[(k - F)^+] = E[-1(F <= k) dF/ds] = -f E^s[1(Z <= m) (Z - s)]
		inline double vega(double f, double s, double k)
		{
			double m = moneyness(f, k, s);

			return f*normal::pdf(m, s);
		}
#ifdef _DEBUG
		inline int vega_test()
		{
			double f = 100, s = 0.2, k = 100;
			double v = vega(f, s, k);
			double h = 0.00001;
			double vup= value(f, s + h, k);
			double vdn = value(f, s - h, k);
			double dv = (vup - vdn) / (2 * h);
			double err = v - dv;
			assert(fabs(err) < 10 * h);

			return 0;
		}
#endif // _DEBUG
		// Return vol s with p = value(f, s, k)
		inline double implied(double f, double p, double k,
			double eps = sqrt(epsilon), size_t N = 100, double s = 0.1)
		{
			while (fabs(p - value(f, s, k)) > eps) {
				if (N-- == 0) {
					return NaN;
				}
				double s_ = s - (value(f, s, k) - p) / vega(f, s, k); // Newton-Raphson x' = x - f(x)/f'(x)
				if (s_ < 0) {
					s_ = s / 2;
				}
				std::swap(s_, s);
			}

			return s;
		}
#ifdef _DEBUG
		inline int implied_test()
		{
			{
				double f = 100, s = 0.2, k = 100;
				double p = value(f, s, k);
				s = implied(f, p, k);
				assert(fabs(p - value(f, s, k)) < sqrt(epsilon));
			}

			return 0;
		}
#endif // _DEBUG
	}
	namespace call {

		// (F - k)^+ - (k - F)^+ = F - k
		inline double value(double f, double s, double k)
		{
			return put::value(f, s, k) + f - k;
		}

		// call delta - put delta = 1
		inline double delta(double f, double s, double k)
		{
			return put::delta(f, s, k) + 1;
		}
	}

} // namespace fre::black

// fre_black.h - Black model for European options.
// Price at expiration is F = f exp(s Z - s^2/2), Z standard normal.
// Note E[F] = f and Var(log F) = s^2.
#pragma once
#include "fre_normal.h"

namespace fre::black {

	// F <= k iff Z <= log(k/f)/s + s/2
	inline double moneyness(double f, double k, double s)
	{
		return std::log(k / f) / s + s / 2;
	}

	namespace put {

		// E[(k - F)^+] = k P(F <= k) - f P^s(F <= k)
		inline double value(double f, double s, double k)
		{
			double m = moneyness(f, k, s);

			return k * fre::normal::cdf(m) - f * fre::normal::cdf(m, s);
		}

		// (d/df)E[(k - F)^+] = E[-1(F <= k) exp(sZ - s^2/2] = -P^s(Z <= k)
		inline double delta(double f, double s, double k)
		{
			double m = moneyness(f, k, s);

			return -fre::normal::cdf(m, s);
		}
	}
	namespace call {

		// (F - k)^+ - (k - F)^+ = F - k
		inline double value(double f, double s, double k)
		{
			return put::value(f, s, k) + f - k;
		}

		// call delts - put delta = 1
		inline double delta(double f, double s, double k)
		{
			return put::delta(f, s, k) + 1;
		}
	}

} // namespace fre::black

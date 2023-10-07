// fre_black.h - Black model for European options.
// Price at expiration is F = f exp(s Z - s^2/2), Z standard normal.
// Note E[F] = f and Var(log F) = s^2.
#pragma once
#include "fre_normal.h"

namespace fre::black {

	// F <= k iff Z <= log(k/f)/s + s/2
	double moneyness(double f, double k, double s)
	{
		return std::log(k / f) / s + s / 2;
	}

	namespace put {
		// E[(k - F)^+] = k P(F <= k) - f P^s(F <= k)
		double value(double f, double k, double s)
		{
			double m = moneyness(f, k, s);

			return k * fre::normal::cdf(m) - f * fre::normal::cdf(m, s);
		}
	}

} // namespace fre::black

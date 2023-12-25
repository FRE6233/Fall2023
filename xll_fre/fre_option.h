// Created by: More Zeng
// According to http://www.cnblogs.com/MoreWindows/archive/2011/08/06/2137457.html
// ... and CodePilot
// fre_option.h: https://keithalewis.github.io/math/op.html
// F = f exp(sX - kappa(s)), where kappa(s) = log E[e^{sX}] is the cumulant of X.
// If E[X] = 0 and Var(X) = 1, then E[F] = f and Var(log F) = s^2.
#pragma once
#include <cmath>
#include <numeric>
#include <random>
#include <valarray>
#include <vector>
#include "fre_variate.h"

namespace fre::option {

	// F <= k iff X <= (log(k/f) + kappa(s))/s 
	inline double moneyness(double f, double s, double k, const variate::nvi& v = variate::normal{})
	{
		return (std::log(k / f) + v.cgf(s)) / s;
	}

	namespace put {

		// E[(k - F)^+] = E[(k - F) 1(F <= k)] 
		//              = k P(F <= k) - E[F 1(F <= k)]
		//              = k P(F <= k) - E[F] E(F/E[F] 1(F <= k)]
		//              = k P(F <= k) - f P_s(F <= k)]
		inline double value(double f, double s, double k, const variate::nvi& v = variate::normal{})
		{
			double z = moneyness(f, s, k, v);

			return k * v.cdf(z, 0) - f * v.cdf(z, s);
		}

		// (d/df) E[(k - F)^+] = E[-1(F <= k) dF/df] = -P_s(F <= k)
		inline double delta(double f, double s, double k, const variate::nvi& v = variate::normal{})
		{
			double z = moneyness(f, s, k, v);

			return -v.cdf(z, s);
		}

	} // namespace put


} // namespace fre::option
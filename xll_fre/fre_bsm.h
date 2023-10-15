// fre_bsm.h - Black-Scholes/Merton model for European options.
// Spot price is S_t = S0 exp(rt) exp(σ B_t - σ^2 t/2).
// Black price at expiration is F = f exp(s Z - s^2/2), Z standard normal.
// Note S_t = F where f = S0 exp(rt) and s = σ√t.
#pragma once
#include <tuple>
#include "fre_black.h"

namespace fre::bsm {

#pragma warning(disable: 4100)

	// Convert from Black-Scholes/Merton to Black parameters.
	inline std::tuple<double, double, double> bsm_to_black(double r, double S0, double σ, double t)
	{
		double R = exp(r * t);

		return { R, R * S0, σ * sqrt(t) }; // R, f, s
	}

	namespace put {
		// Put value is E[(k - S_t)^+]/R, where R = exp(r t).
		//                  ------------market-----------
		//                  --bond--|-------stock--------  ------option------
		inline double value(double r, double S0, double σ, double k, double t)
		{
			auto [R, f, s] = bsm_to_black(r, S0, σ, t);

			return black::put::value(f, s, k)/R;  
		}
		// Delta is (d/dS0)E[(k - S_t)^+]/R = (d/df)E[(k - F)^+]/R df/dS0.
		// df/dS0 = R
		inline double delta(double r, double S0, double σ, double k, double t)
		{
			auto [R, f, s] = bsm_to_black(r, S0, σ, t);

			return black::put::delta(f, s, k);
		}
	}

} // namespace fre::bsm



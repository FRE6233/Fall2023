// fre_bsm.h - Black-Scholes/Merton model for European options.
// Spot price is S_t = S0 exp(rt) exp(σ B_t - σ^2 t/2).
// Black price at expiration is F = f exp(s Z - s^2/2), Z standard normal.
// Note S_t = F where f = S0 exp(rt) and s = σ√t.
#pragma once
#include "fre_black.h"

namespace fre::bsm {

#pragma warning(disable: 4100)
	namespace put {
		// Value is E[(k - S_t)^+]/R, where R = exp(r t).
		//                  ------------market-----------
		//                  --bond--|-------stock--------  ------option------
		inline double value(double r, double S0, double σ, double k, double t)
		{
			double R = std::exp(r*t); 
			double f = R*S0; 
			double s = σ*std::sqrt(t);

			return fre::black::put::value(f, s, k)/R;  
		}
		// Delta is (d/dS0)E[(k - S_t)^+]/R = (d/df)E[(k - F)^+]/R df/dS0.
		// df/dS0 = R
		inline double delta(double r, double S0, double σ, double k, double t)
		{
			double R = std::exp(r * t);
			double f = R * S0;
			double s = σ * std::sqrt(t);

			return fre::black::put::delta(f, s, k);
		}
	}
} // namespace fre::bsm



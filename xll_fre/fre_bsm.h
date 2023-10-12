// fre_bsm.h - Black-Scholes-Merton model for European options.
// Spot price is S_t = R S0 exp(σ√t Z - σ^2 t/2), Z standard normal.
// Black price at expiration is F = f exp(s Z - s^2/2), Z standard normal.
// Note S = F where f = R S0 and s = σ√t.
#pragma once
#include "fre_black.h"

namespace fre::bsm {

#pragma warning(disable: 4100)
	namespace put {
		// Value is E[(k - S)^+]/R, where R = exp(r t).
		//                  ------------market-----------
		//                  --bond--|-------stock--------  ------option------
		inline double value(double r, double S0, double σ, double k, double t)
		{
			double R = std::exp(r*t); 
			double f = R*S0; //!!! implement this
			double s = σ*std::sqrt(t); //!!! implement this

			return fre::black::put::value(f, s, k)/R;  
		}
	}
} // namespace fre::bsm



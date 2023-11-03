// fre_ho_lee.h - Ho-Lee interest rate model for constant futures and volatility.
#pragma once
#include <cmath>
#include <random>

namespace fre::ho_lee {

	// E[exp(N)] = exp(E[N] + Var(N)/2)
	inline double Eexp(const std::normal_distribution<double>& N)
	{
		return std::exp(N.mean() + N.sigma() * N.sigma() / 2);
	}

	// D(t) = E[D_t] = ???
	inline double ED(double φ, double σ, double t)
	{
		return φ * σ * t; // !!!
	}

	// φ(t) - f(t) = ???
	inline double convexity(double σ, double t)
	{
		return σ * t; // !!!
	}

	// log D_t(u) = ???
	// Return mean and standard deviation of log D_t(u).
	inline std::normal_distribution<double> logD(double φ, double σ, double t, double u)
	{
		return std::normal_distribution(φ, σ * t * u); // !!!
	}

	// φ_t(u) - f_t(u) = ???
	// Return mean and standard deviation of φ_t(u) - f_t(u)
	inline std::normal_distribution<double> convexity(double σ, double t, double u)
	{
		return std::normal_distribution(t*u, σ); // !!!
	}

} // namespace fre::ho_lee

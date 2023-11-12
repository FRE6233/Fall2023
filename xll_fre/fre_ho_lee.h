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

	// D(t) = E[D_t] = exp(-φ t + σ^2 t^3/6)
	inline double ED(double φ, double σ, double t)
	{
		return std::exp(-φ * t + σ * σ * t * t * t / 6);
	}

	// φ(t) - f(t) = σ^2 t^2/2
	inline double convexity(double σ, double t)
	{
		return σ * σ * t * t / 2;
	}

	// log D_t(u) = −φ * (u - t) + σ^2 (u−t)^3/6 − σ (u−t) B_t
	// Return mean and standard deviation of log D_t(u).
	inline std::normal_distribution<double> logD(double φ, double σ, double t, double u)
	{
		double mu = -φ * (u - t) + σ * σ * (u - t) * (u - t) * (u - t) / 6;
		double var = σ * σ * (u - t) * (u - t) * t;
		
		return std::normal_distribution(mu, std::sqrt(var));
	}

	// φ(u) - f_t(u) = σ^2 (u−t)^2/2 − σ B_t
	// Return mean and standard deviation of φ_t(u) - f_t(u)
	inline std::normal_distribution<double> convexity(double σ, double t, double u)
	{
		double mu = σ * σ * (u - t) * (u - t) / 2;
		double var = σ * σ * t;	

		return std::normal_distribution(mu, std::sqrt(var));
	}

} // namespace fre::ho_lee

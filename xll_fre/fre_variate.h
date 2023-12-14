// fre_variate.h: Interface to standard random variates, E[X] = 0 and Var(X) = 1.
#pragma once
#include <cmath>
#include <numeric>
#include <random>
#include <valarray>

namespace fre::variate {

	inline auto dre = std::default_random_engine{};
	inline auto& seed(std::default_random_engine::result_type seed)
	{
		dre.seed(seed);

		return dre;
	}

	// Non Virtual Interface to random variates.
	struct nvi {

		virtual ~nvi()
		{ }

		// Generate a random variate
		double operator()()
		{
			return gen_();
		}

		// normalize X to have E[X] = 0 and Var(X) = 1
		nvi& std()
		{
			return std_();
		}

		// Cumulant generating function of X, log E[e^{sX}]
		double cgf(double s) const
		{
			return cgf_(s);
		}

		// Share measure P_s(X <= x) = E[e^{s X - kappa(s)}1(X <= x)]
		double cdf(double x, double s = 0) const
		{
			return cdf_(x, s);
		}

	private:
		virtual double gen_() = 0;
		virtual nvi& std_() = 0;
		virtual double cgf_(double s) const = 0;
		virtual double cdf_(double x, double s) const = 0;
	};

	// X standard normal random variate
	struct normal : public nvi {
		std::normal_distribution<> n;
		
		normal(double mean = 0, double sigma = 1)
			: n(mean, sigma)
		{ }
		normal(const normal&) = default;
		normal& operator=(const normal&) = default;
		~normal() = default;

		double gen_() override
		{
			return n(dre);
		}

		normal& std_() override
		{
			n = std::normal_distribution(0., 1.);

			return *this;
		}

		// log E[e^{sX}] = log exp(E[sX] + Var(sX)/2) = s^2/2
		double cgf_(double s) const override
		{
			return s * s / 2;
		}
		// P_s(X <= x) = P(X <= x - s)
		double cdf_(double x, double s) const override
		{
			double z = (x - s - n.mean())/ n.sigma();
			
			return std::erfc(-z / std::sqrt(2)) / 2;
		}
	};

	// Discrete random variate, P(X = x_i) = p_i
	class discrete : public nvi {
		std::valarray<double> x_;
		std::discrete_distribution<> p;
	public:
		discrete(size_t n, const double* x, const double* p)
			: x_(x, n), p(p, p + n)
		{ }
		discrete(const discrete&) = default;
		discrete& operator=(const discrete&) = default;
		~discrete() = default;

		// E[f(X)] = sum_i f(x_i) p_i
		template<class F>
		double E(F f) const
		{
			return std::inner_product(begin(x_), end(x_),
				p.probabilities().begin(), 0.0,
				std::plus<double>(),
				[f](double x, double p) { return f(x) * p; });
		}
		discrete& std_() override
		{
			double m = E([](double x) { return x; });
			double s2 = E([](double x) { return x * x; });

			x_ -= m;
			x_ /= sqrt(s2 - m * m);

			return *this;
		}

		double gen_() override
		{
			return x_[p(dre)];
		}

		// moment generating function of X, E[e^{sX}]
		double mgf_(double s) const
		{
			return E([s](double x) { return exp(s * x); });
		}
		double cgf_(double s) const override
		{
			return log(mgf_(s));
		}
		double cdf_(double x, double s) const override
		{
			double EsX = mgf_(s);

			return E([x, s, EsX](double y) { return (y <= x) * exp(s * y) / EsX; });
		}
	};

} // namespace fre

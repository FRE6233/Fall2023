// xll_binomial.cpp - binomial model
#include "fre_binomial.h"
#include "xll_fre.h"

using namespace fre;
using namespace xll;

#ifdef _DEBUG
int test_random_walk = binomial::random_walk_test();
int test_american_random_walk = binomial::american_random_walk_test();
#endif // _DEBUG

AddIn xai_binomial_american_put(
	Function(XLL_DOUBLE, "xll_binomial_american_put", "BINOMIAL.AMERICAN.PUT.VALUE")
	.Arguments({
		Arg(XLL_DOUBLE, "r", "is the risk free rate."),
		Arg(XLL_DOUBLE, "S0", "is the spot price."),
		Arg(XLL_DOUBLE, "sigma", "is the volatility."),
		Arg(XLL_DOUBLE, "k", "is the strike price."),
		Arg(XLL_DOUBLE, "t", "is the time in years to expiration."),
		Arg(XLL_DOUBLE, "dt", "is the binomial step size in years."),
		})
	.Category(CATEGORY)
	.FunctionHelp("Return American put value using the binomial model.")
);
double WINAPI xll_binomial_american_put(double r, double S0, double sigma, double k, double t, double dt)
{
#pragma XLLEXPORT

	return fre::binomial::american_put_value(r, S0, sigma, k, t, dt);		
}

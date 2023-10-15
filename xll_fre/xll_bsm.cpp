// xll_bsm.cpp - Black-Scholes-Merton model for European options.
#include "fre_bsm.h"
#include "xll_fre.h"	

using namespace xll;

AddIn xai_bsm_put_value(
	Function(XLL_DOUBLE, "xll_bsm_put_value", "BSM.PUT.VALUE")
	.Arguments({
		Arg(XLL_DOUBLE, "r", "is the risk-free rate."),
		Arg(XLL_DOUBLE, "S0", "is the initial stock price."),
		Arg(XLL_DOUBLE, "sigma", "is the volatility."),
		Arg(XLL_DOUBLE, "k", "is the strike price."),
		Arg(XLL_DOUBLE, "t", "is the time in years to expiration."),
		})
	.Category(CATEGORY)
	.FunctionHelp("Return the Black-Scholes-Merton put value.")
);
double WINAPI xll_bsm_put_value(double r, double S0, double sigma, double k, double t)
{
#pragma XLLEXPORT
	return fre::bsm::put::value(r, S0, sigma, k, t);
}

AddIn xai_bsm_put_delta(
	Function(XLL_DOUBLE, "xll_bsm_put_delta", "BSM.PUT.DELTA")
	.Arguments({
		Arg(XLL_DOUBLE, "r", "is the risk-free rate."),
		Arg(XLL_DOUBLE, "S0", "is the initial stock price."),
		Arg(XLL_DOUBLE, "sigma", "is the volatility."),
		Arg(XLL_DOUBLE, "k", "is the strike price."),
		Arg(XLL_DOUBLE, "t", "is the time in years to expiration."),
		})
	.Category(CATEGORY)
	.FunctionHelp("Return the Black-Scholes-Merton put delta.")
);
double WINAPI xll_bsm_put_delta(double r, double S0, double sigma, double k, double t)
{
#pragma XLLEXPORT
	return fre::bsm::put::delta(r, S0, sigma, k, t);
}
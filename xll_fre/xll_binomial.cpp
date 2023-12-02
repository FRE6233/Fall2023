// xll_binomial.cpp - binomial model
#include "fre_binomial.h"
#include "xll_fre.h"

using namespace fre;
using namespace xll;

static AddIn xai_fre_binomial(
	Function(XLL_DOUBLE, "xll_fre_binomial", "FRE.BINOMIAL")
	.Arguments({
		Arg(XLL_DOUBLE, "s", "is the spot price."),
		Arg(XLL_DOUBLE, "k", "is the strike price."),
		Arg(XLL_DOUBLE, "r", "is the risk-free interest rate."),
		Arg(XLL_DOUBLE, "sigma", "is the volatility."),
		Arg(XLL_DOUBLE, "t", "is the time to expiration."),
		Arg(XLL_DOUBLE, "n", "is the number of steps."),
	})
	.Category(CATEGORY)
	.FunctionHelp("Return the binomial price of a European option.")
);
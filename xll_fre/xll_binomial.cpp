// xll_binomial.cpp - binomial model
#include "fre_binomial.h"
#include "xll_fre.h"

using namespace fre;
using namespace xll;

#ifdef _DEBUG
int test_random_walk = binomial::random_walk_test();
int test_american_random_walk = binomial::american_random_walk_test();
#endif // _DEBUG
#if 0
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
#endif // 0
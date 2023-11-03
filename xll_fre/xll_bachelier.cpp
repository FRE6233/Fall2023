// xll_bachelier.h - Black model for European options.
#include "fre_bachelier.h"
#include "xll_fre.h"

using namespace fre;
using namespace xll;

#ifdef _DEBUG
int test_bachelier_put_value = bachelier::put::value_test();
#endif // _DEBUG

AddIn xai_bachelier_moneyness(
	Function(XLL_DOUBLE, "xll_bachelier_moneyness", "BACHELIER.MONEYNESS")
	.Arguments({
		Arg(XLL_DOUBLE, "f", "is the forward price."),
		Arg(XLL_DOUBLE, "σ", "is the volatility."),
		Arg(XLL_DOUBLE, "k", "is the strike price."),
		Arg(XLL_DOUBLE, "t", "is time in years."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Return the moneyness.")
);
double WINAPI xll_bachelier_moneyness(double f, double σ, double k, double t)
{
#pragma XLLEXPORT

	return bachelier::moneyness(f, σ, k, t);
}

AddIn xai_bachelier_put_value(
	Function(XLL_DOUBLE, "xll_bachelier_put_value", "BACHELIER.PUT.VALUE")
	.Arguments({
		Arg(XLL_DOUBLE, "f", "is the forward price."),
		Arg(XLL_DOUBLE, "σ", "is the volatility."),
		Arg(XLL_DOUBLE, "k", "is the strike price."),
		Arg(XLL_DOUBLE, "t", "is time in years."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Return the bachelier put value.")
);
double WINAPI xll_bachelier_put_value(double f, double σ, double k, double t)
{
#pragma XLLEXPORT

	return bachelier::put::value(f, σ, k, t);
}

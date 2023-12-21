// xll_black.h - Black model for European options.
#include "fre_black.h"
#include "xll_fre.h"

using namespace xll;

#ifdef _DEBUG
int test_vega = fre::black::put::vega_test();
int test_implied = fre::black::put::implied_test();
#endif // _DEBUG

AddIn xai_black_moneyness(
	Function(XLL_DOUBLE, "xll_black_moneyness", "BLACK.MONEYNESS")
	.Arguments({
		Arg(XLL_DOUBLE, "f", "is the forward price."),
		Arg(XLL_DOUBLE, "s", "is the volatility."),
		Arg(XLL_DOUBLE, "k", "is the strike price."),
		})
	.Category(CATEGORY)
	.FunctionHelp("Return the moneyness.")
);
double WINAPI xll_black_moneyness(double f, double s, double k)
{
#pragma XLLEXPORT

	return fre::black::moneyness(f, k, s);
}

AddIn xai_black_put(
	Function(XLL_DOUBLE, "xll_black_put", "BLACK.PUT.VALUE")
	.Arguments({
		Arg(XLL_DOUBLE, "f", "is the forward price."),
		Arg(XLL_DOUBLE, "s", "is the volatility."),
		Arg(XLL_DOUBLE, "k", "is the strike price."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Return the black put value.")
);
double WINAPI xll_black_put(double f, double s, double k)
{
#pragma XLLEXPORT

	return fre::black::put::value(f, s, k);
}

AddIn xai_black_put_delta(
	Function(XLL_DOUBLE, "xll_black_put_delta", "BLACK.PUT.DELTA")
	.Arguments({
		Arg(XLL_DOUBLE, "f", "is the forward price."),
		Arg(XLL_DOUBLE, "s", "is the volatility."),
		Arg(XLL_DOUBLE, "k", "is the strike price."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Return the black put delta.")
);
double WINAPI xll_black_put_delta(double f, double s, double k)
{
#pragma XLLEXPORT

	return fre::black::put::delta(f, s, k);
}

AddIn xai_black_put_implied(
	Function(XLL_DOUBLE, "xll_black_put_implied", "BLACK.PUT.IMPLIED")
	.Arguments({
		Arg(XLL_DOUBLE, "f", "is the forward price."),
		Arg(XLL_DOUBLE, "p", "is the put price."),
		Arg(XLL_DOUBLE, "k", "is the strike price."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Return the black put implied volatility.")
);
double WINAPI xll_black_put_implied(double f, double p, double k)
{
#pragma XLLEXPORT

	return fre::black::put::implied(f, p, k);
}

AddIn xai_black_call_implied(
	Function(XLL_DOUBLE, "xll_black_call_implied", "BLACK.CALL.IMPLIED")
	.Arguments({
		Arg(XLL_DOUBLE, "f", "is the forward price."),
		Arg(XLL_DOUBLE, "c", "is the call price."),
		Arg(XLL_DOUBLE, "k", "is the strike price."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Return the black put implied volatility.")
);
double WINAPI xll_black_call_implied(double f, double p, double k)
{
#pragma XLLEXPORT

	return fre::black::call::implied(f, p, k);
}

// xll_option.cpp - Generic option pricing.
#include "xll_fre.h"
#include "fre_option.h"

using namespace xll;
using namespace fre;

AddIn xai_fre_option_moneyness(
	Function(XLL_DOUBLE, "xll_fre_option_moneyness", "OPTION.MONENESS")
	.Arguments({
		Arg(XLL_DOUBLE, "f", "is the forward price."),
		Arg(XLL_DOUBLE, "s", "is the volatility."),
		Arg(XLL_DOUBLE, "k", "is the strike price."),
		Arg(XLL_HANDLEX, "handle", "is a handle to a standard variate.")
	})
	.FunctionHelp("Return the cumulant generating function of a standard normal random variate.")
);
double WINAPI xll_fre_option_moneyness(double f, double s, double k, HANDLEX h)
{
#pragma XLLEXPORT
	double z = std::numeric_limits<double>::quiet_NaN();

	try {
		handle<variate::nvi> h_(h);
		ensure(h_);

		z = option::moneyness(f, s, k, *h_);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return z;
}

AddIn xai_fre_option_put_value(
	Function(XLL_DOUBLE, "xll_fre_option_put_value", "OPTION.PUT.VALUE")
	.Arguments({
		Arg(XLL_DOUBLE, "f", "is the forward price."),
		Arg(XLL_DOUBLE, "s", "is the volatility."),
		Arg(XLL_DOUBLE, "k", "is the strike price."),
		Arg(XLL_HANDLEX, "handle", "is a handle to a standard variate.")
	})
	.FunctionHelp("Return the value of a put option.")
);
double WINAPI xll_fre_option_put_value(double f, double s, double k, HANDLEX h)
{
#pragma XLLEXPORT
	double z = std::numeric_limits<double>::quiet_NaN();

	try {
		handle<variate::nvi> h_(h);
		ensure(h_);

		z = option::put::value(f, s, k, *h_);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return z;
}

// xll_variate.cpp - Generic random variate.
#include "fre_variate.h"
#include "xll_fre.h"

using namespace xll;
using namespace fre;

AddIn xai_fre_variate_gen(
	Function(XLL_DOUBLE, "xll_fre_variate_gen", "VARIATE.GEN")
	.Arguments({
		Arg(XLL_HANDLEX, "handle", "is a handle to a variate.")
	})
	.Category(CATEGORY)
	.Volatile()
	.FunctionHelp("Return a random variate.")
);
double WINAPI xll_fre_variate_gen(HANDLEX h)
{
#pragma XLLEXPORT
	double result = std::numeric_limits<double>::quiet_NaN();

	try {
		handle<variate::nvi> h_(h);
		ensure(h_);

		result = (*h_)();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
		
	return result;
}

AddIn xai_fre_variate_normal(
	Function(XLL_HANDLEX, "xll_fre_variate_normal", "\\VARIATE.NORMAL")
	.Arguments({
		Arg(XLL_DOUBLE, "mean", "is the mean of the normal. Default is 0."),
		Arg(XLL_DOUBLE, "sigma", "is the standard deviation of the normal. Default is 1."),
	})
	.Uncalced()
	.FunctionHelp("Return a handle to a standard normal random variate.")
);
HANDLEX WINAPI xll_fre_variate_normal(double mean, double sigma)
{
#pragma XLLEXPORT
	HANDLEX h = INVALID_HANDLEX;

	try {
		if (sigma == 0) {
			sigma = 1;
		}
		handle<variate::nvi> h_(new variate::normal(mean, sigma));

		h = h_.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return h;
}

AddIn xai_fre_variate_discrete(
	Function(XLL_HANDLEX, "xll_fre_variate_discrete", "\\VARIATE.DISCRETE")
	.Arguments({
		Arg(XLL_FP, "x", "are the values of the random variables."),
		Arg(XLL_FP, "p", "are the probabilities of the values."),
		})
		.Uncalced()
	.FunctionHelp("Return a handle to a standard normal random variate.")
);
HANDLEX WINAPI xll_fre_variate_discrete(_FPX* px, _FPX* pp)
{
#pragma XLLEXPORT
	HANDLEX h = INVALID_HANDLEX;

	try {
		ensure(size(*px) == size(*pp));
		handle<variate::nvi> h_(new variate::discrete(size(*px), px->array, pp->array));

		h = h_.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return h;
}

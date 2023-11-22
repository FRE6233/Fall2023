// xll_ho_lee.cpp - Ho-Lee interest rate add-ins
#include "fre_ho_lee.h"
#include "xll_fre.h"

using namespace fre;
using namespace xll;

// !!! Implement the following add-ins !!!
// HO_LEE.ED
// HO_LEE.Convexity
// HO_LEE.LogD

AddIn xai_ho_lee_ED(
	Function(XLL_DOUBLE, "xll_ho_lee_ED", "HO_LEE.ED")
	.Arguments({
		Arg(XLL_DOUBLE, "φ", "is the futures."),
		Arg(XLL_DOUBLE, "σ", "is the volatility."),
		Arg(XLL_DOUBLE, "t", "is the forward time in years"),
		})
		.FunctionHelp("Return the expected discount.")
	.Category(CATEGORY)
);
double WINAPI xll_ho_lee_ED(double φ, double σ, double t)
{
#pragma XLLEXPORT
	return ho_lee::ED(φ, σ, t);
}

// This illustrates how to return an array of doubles to Excel.
AddIn xai_ho_lee_LogD(
	Function(XLL_FPX, "xll_ho_lee_LogD", "HO_LEE.LogD")
	.Arguments({
		Arg(XLL_DOUBLE, "φ", "is the futures."),
		Arg(XLL_DOUBLE, "σ", "is the volatility."),
		Arg(XLL_DOUBLE, "t", "is the forward time in years"),
		Arg(XLL_DOUBLE, "u", "is the time to futures expiration in years"),
		})
		.FunctionHelp("Return the mean and standard deviation of forward convexity of a Ho-Lee model.")
	.Category(CATEGORY)
);
_FPX* WINAPI xll_ho_lee_LogD(double φ, double σ, double t, double u)
{
#pragma XLLEXPORT
	static xll::FPX stdev(1, 2); // 1x2 array of doubles

	try {
		std::normal_distribution<double> N = ho_lee::logD(φ, σ, t, u);

		stdev[0] = N.mean();
		stdev[1] = N.stddev();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0; // #NUM!
	}

	return stdev.get();
}

AddIn xai_ho_lee_convexity(
	Function(XLL_DOUBLE, "xll_ho_lee_convexity", "HO_LEE.Convexity")
	.Arguments({
		Arg(XLL_DOUBLE, "σ", "is the volatility."),
		Arg(XLL_DOUBLE, "t", "is the forward time in years"),
		})
		.FunctionHelp("Return the convexity at time t.")
	.Category(CATEGORY)
);
double WINAPI xll_ho_lee_convexity(double σ, double t)
{
#pragma XLLEXPORT
	return ho_lee::convexity(σ, t);
}

// This illustrates how to return an array of doubles to Excel.
AddIn xai_ho_lee_convexity2(
	Function(XLL_FPX, "xll_ho_lee_convexity2", "HO_LEE.Convexity2")
	.Arguments({
		Arg(XLL_DOUBLE, "σ", "is the volatility."),
		Arg(XLL_DOUBLE, "t", "is the forward time in years"),
		Arg(XLL_DOUBLE, "u", "is the time to futures expiration in years"),
		})
	.FunctionHelp("Return the mean and standard deviation of forward convexity of a Ho-Lee model.")
	.Category(CATEGORY)
);
_FPX* WINAPI xll_ho_lee_convexity2(double σ, double t, double u)
{
#pragma XLLEXPORT
	static xll::FPX stdev(1, 2); // 1x2 array of doubles

	try {
		std::normal_distribution<double> N = ho_lee::convexity(σ, t, u);

		stdev[0] = N.mean();
		stdev[1] = N.stddev();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0; // #NUM!
	}

	return stdev.get();
}

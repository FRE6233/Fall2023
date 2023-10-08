// xll_normal.cpp - Standard normal distribution.
#include "fre_normal.h"
#include "xll_fre.h"

using namespace xll;

AddIn xai_norrmal_pdf(
	Function(XLL_DOUBLE, "xll_normal_pdf", "NORMAL.PDF")
	.Arguments({
		Arg(XLL_DOUBLE, "x", "is the value at which you evaluate the density function."),
		Arg(XLL_DOUBLE, "s", "is the share measure parameter."),
		})
	.Category(CATEGORY)
	.FunctionHelp("Evaluate the standard normal density function.")
);
double WINAPI xll_normal_pdf(double x, double s)
{
#pragma XLLEXPORT

	return fre::normal::pdf(x, s);
}

AddIn xai_norrmal_cdf(
	Function(XLL_DOUBLE, "xll_normal_cdf", "NORMAL.CDF")
	.Arguments({
		Arg(XLL_DOUBLE, "x", "is the value at which you evaluate the cumulative distribution function."),
		Arg(XLL_DOUBLE, "s", "is the share measure parameter."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Evaluate the standard normal cumulative distribution  function.")
);
double WINAPI xll_normal_cdf(double x, double s)
{
#pragma XLLEXPORT

	return fre::normal::cdf(x, s);
}
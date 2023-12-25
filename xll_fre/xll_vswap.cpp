// vswap.cpp
#include "fre_vswap.h"
#include "xll_fre.h"

using namespace xll;

//!!! implement vswap::variance
AddIn xai_vswap(
	Function(XLL_DOUBLE, "xll_vswap", "XLL.VSWAP")
	.Arguments({
		Arg(XLL_DOUBLE, "z", "is the put-call separator"),
		Arg(XLL_FP, "k", "is an array of strikes"),
		Arg(XLL_FP, "p", "is an array of put prices"),
		Arg(XLL_FP, "c", "is an array of call prices"),
	})
	.Category(CATEGORY)
	.FunctionHelp("Return the par variance of a variance swap.")
);
double WINAPI xll_vswap(double z, const _FP12* pk, const _FP12* pp, const _FP12* pc)
{
#pragma XLLEXPORT
	double result = std::numeric_limits<double>::quiet_NaN();

	try {
		ensure (size(*pk) == size(*pp));
		ensure (size(*pk) == size(*pc));

		result = fre::vswap::variance(z, size(*pk), pk->array, pp->array, pc->array);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return result;
	}

	return result;
}
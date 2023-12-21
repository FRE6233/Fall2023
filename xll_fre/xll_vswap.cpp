// vswap.cpp
#include "fre_vswap.h"
#include "xll_fre.h"

using namespace xll;

AddIn xai_pwlinear(
	Function(XLL_HANDLEX, "xll_pwlinear", "\\XLL.PWLINEAR")
	.Arguments({
		Arg(XLL_FPX, "x", "is an array of increasing numbers."),
		Arg(XLL_FPX, "y", "is an array of numbers."),
		})
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp("Return a handle to a piecewise linear function.")
);
HANDLEX WINAPI xll_pwlinear(const _FPX* px, const _FPX* py)
{
#pragma XLLEXPORT
	HANDLEX h = INVALID_HANDLEX;

	try {
		ensure(size(*px) == size(*py))
		handle<fre::vswap::pwlinear> h_(new fre::vswap::pwlinear(
			size(*px), px->array, py->array));
		ensure(h_);

		h = h_.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return h;
}

AddIn xai_pwlinear_value(
	Function(XLL_DOUBLE, "xll_pwlinear_value", "XLL.PWLINEAR.VALUE")
	.Arguments({
		Arg(XLL_HANDLEX, "handle", "is a handle returned by XLL.PWLINEAR."),
		Arg(XLL_DOUBLE, "x", "is a number."),
		})
	.Category(CATEGORY)
	.FunctionHelp("Return the value of the piecewise linear function at x.")
);
double WINAPI xll_pwlinear_value(HANDLEX h, double x)
{
#pragma XLLEXPORT
	try {
		handle<fre::vswap::pwlinear> h_(h);
		ensure(h_);

		return h_->value(x);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return std::numeric_limits<double>::quiet_NaN();
	}
}

//!!! implement vswap::variance
AddIn xai_vswap(
	Function(XLL_DOUBLE, "xll_vswap", "XLL.VSWAP")
	//...
);
double WINAPI xll_vswap()
{
#pragma XLLEXPORT
	return 0;
}
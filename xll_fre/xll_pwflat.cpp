// xll_bootstrap.cpp - Bootstrap a piecewise flat forward curve
#ifndef CATEGORY
#define CATEGORY "FI"
#define _CATEGORY "\\" CATEGORY
#endif
#include "fre_pwflat.h"
#include "xll_fre.h"

using namespace xll;
using namespace fre;

#ifdef _DEBUG
//int test_extend = bootstrap::extend_test();
#endif // _DEBUG

AddIn xai_pwflat_(
	Function(XLL_HANDLE, "xll_pwflat_", "\\" CATEGORY ".CURVE")
	.Arguments({
		Arg(XLL_FPX, "Time", "is an array of positive increasing times."),
		Arg(XLL_FPX, "Rate", "is an array of forward rates."),
		Arg(XLL_DOUBLE, "extrapolate", "is an optional extrapolation value. Default is 0."),
		})
		.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp("Return a handle to a piece-wise flat forward curve.")
);
HANDLEX WINAPI xll_pwflat_(const _FPX* pt, const _FPX* pf, double _f)
{
#pragma XLLEXPORT
	HANDLEX h = INVALID_HANDLEX; // default return value

	try {
		ensure(size(*pt) == size(*pf));

		if (size(*pt) == 1 and pt->array[0] == 0) {
			handle<pwflat::curve<>> c(new pwflat::curve<>());
			ensure(c);

			h = c.get();
		}
		else {
			handle<pwflat::curve<>> c(new pwflat::curve<>(size(*pt), pt->array, pf->array, _f));
			ensure(c);

			h = c.get();
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

AddIn xai_pwflat(
	Function(XLL_FPX, "xll_pwflat", CATEGORY ".CURVE")
	.Arguments({
		Arg(XLL_HANDLEX, "Curve", "is handle returned by " CATEGORY ".CURVE"),
		})
		.Category(CATEGORY)
	.FunctionHelp("Return times and rates of curve as a two row array.")
);
_FPX* WINAPI xll_pwflat(HANDLEX curve)
{
#pragma XLLEXPORT
	static FPX result;

	try {
		handle<pwflat::curve<>> c(curve);
		ensure(c);
		result.resize(2, (int)c->size());
		for (int i = 0; i < result.columns(); ++i) {
			result(0, i) = c->time()[i];
			result(1, i) = c->rate()[i];
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
		return 0;
	}

	return result.get();
}

AddIn xai_pwflat_value(
	Function(XLL_FPX, "xll_pwflat_value", CATEGORY ".CURVE.VALUE")
	.Arguments({
		Arg(XLL_HANDLEX, "Curve", "is handle returned by " _CATEGORY ".CURVE"),
		Arg(XLL_FPX, "Time", "is an array of times."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Return values of piece-wise flat forward curve at given times.")
);
_FPX* WINAPI xll_pwflat_value(HANDLEX curve, _FPX* pt)
{
#pragma XLLEXPORT
	try {
		handle<pwflat::curve<>> c(curve);
		ensure(c);
		for (unsigned i = 0; i < size(*pt); ++i) {
			pt->array[i] = c->value(pt->array[i]);
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
		return 0;
	}

	return pt;
}

AddIn xai_pwflat_spot(
	Function(XLL_FPX, "xll_pwflat_spot", CATEGORY ".CURVE.SPOT")
	.Arguments({
		Arg(XLL_HANDLEX, "Curve", "is handle returned by " _CATEGORY ".CURVE."),
		Arg(XLL_FPX, "Time", "is an array of times."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Return spot rates of piece-wise flat forward curve at given times.")
);
_FPX* WINAPI xll_pwflat_spot(HANDLEX curve, _FPX* pt)
{
#pragma XLLEXPORT
	try {
		handle<pwflat::curve<>> c(curve);
		ensure(c);
		for (unsigned i = 0; i < size(*pt); ++i) {
			pt->array[i] = c->spot(pt->array[i]);
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
		return 0;
	}

	return pt;
}

AddIn xai_pwflat_discount(
	Function(XLL_FPX, "xll_pwflat_discount", CATEGORY ".CURVE.DISCOUNT")
	.Arguments({
		Arg(XLL_HANDLEX, "Curve", "is handle returned by " _CATEGORY ".CURVE."),
		Arg(XLL_FPX, "Time", "is an array of times."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Return discount of a piecewise flat forward curve at given times.")
);
_FPX* WINAPI xll_pwflat_discount(HANDLEX curve, _FPX* pt)
{
#pragma XLLEXPORT
	try {
		handle<pwflat::curve<>> c(curve);
		ensure(c);
		for (unsigned i = 0; i < size(*pt); ++i) {
			pt->array[i] = c->discount(pt->array[i]);
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
		return 0;
	}

	return pt;
}

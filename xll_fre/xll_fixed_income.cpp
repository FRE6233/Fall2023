// xll_fixed_income.cpp - Fixed income instruments and valuation.
#include "fre_fixed_income.h"
#include "xll_fre.h"

#undef CATEGORY
#define CATEGORY "FI"

using namespace fre;
using namespace xll;

#ifdef _DEBUG
int test_bootstrap = fixed_income::bootstrap_test();
#endif // _DEBUG

// !!! Implement FI.INSTRUMENT 
AddIn xai_instrument_(
	Function(XLL_HANDLE, "xll_instrument_", "\\" CATEGORY ".INSTRUMENT")
	.Arguments({
		Arg(XLL_FPX, "Time", "is an array of positive increasing times."),
		Arg(XLL_FPX, "Cash", "is an array of cash flows."),
		})
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp("Return a handle to a piece-wise flat forward curve.")
);
HANDLEX WINAPI xll_instrument_(const _FPX* pu, const _FPX* pc)
{
#pragma XLLEXPORT
	HANDLEX h = INVALID_HANDLEX; // default return value

	try {
		ensure(size(*pu) == size(*pc));

		handle<fixed_income::instrument<>> c(new fixed_income::instrument(size(*pu), pu->array, pc->array));
		ensure(c);

		h = c.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}


// !!! Implement FI.PRESENT_VALUE

AddIn xai_present_value(
	Function(XLL_DOUBLE, "xll_present_value", CATEGORY ".PRESENT_VALUE")
	.Arguments({
		Arg(XLL_HANDLEX, "Instrument", "is handle returned by \\" CATEGORY ".INSTRUMENT"),
		Arg(XLL_HANDLEX, "Curve", "is handle returned by \\" CATEGORY ".CURVE"),
		})
		.Category(CATEGORY)
	.FunctionHelp("Return presenet value an instrument given a curve.")
);
double WINAPI xll_present_value(HANDLEX inst, HANDLEX curve)
{
#pragma XLLEXPORT
	double result = std::numeric_limits<double>::quiet_NaN();
	try {
		handle<fixed_income::instrument<>> i(inst);
		ensure(i);
		handle<pwflat::curve<>> c(curve);
		ensure(c);

		result = fixed_income::present_value(*i, *c);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}

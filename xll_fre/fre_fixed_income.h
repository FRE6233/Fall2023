// fre_fixed_income.h - fixed income instrument valuation
#pragma once
#include <type_traits>
#include "fre_pwflat.h"

namespace fre::fixed_income {

	template<class T>
	constexpr T epsilon = std::numeric_limits<T>::epsilon();
	template<class T>
	constexpr T NaN = std::numeric_limits<T>::quiet_NaN();

	// Fixed times and cash flows (u_i, c_i).
	template<class U = double, class C = double>
	class instrument {
		std::vector<U> u; // times
		std::vector<C> c; // cash flows
	public:
		instrument()
		{}
		instrument(size_t n, const U* u, const C* c)
			: u(u, u + n), c(c, c + n)
		{}
		// instrument({u0, ...}, {c0, ...})
		instrument(std::initializer_list<U> u, std::initializer_list<C> c)
			: u(u), c(c)
		{
			ensure (u.size() == c.size());
		}
		instrument(const instrument& i) = default;
		instrument& operator=(const instrument& i) = default;
		~instrument() = default;

		size_t size() const
		{
			return u.size();
		}
		const U* time() const
		{
			return u.data();
		}
		const C* cash() const
		{
			return c.data();
		}

		std::pair<U, C> back() const
		{
			if (size() != 0) {
				return { u.back(), c.back() };
			}

			return { NaN<U>, NaN<C> };
		}
		instrument& push_back(U u, C c)
		{
			ensure(u > u.back());

			u.push_back(u);
			c.psuh_back(c);

			return *this;
		}
	};

	// Present value is p = sum_{u_i <= u} c_i D(u_i)
	template<class U = double, class C = double, class T = double, class F = double>
	inline auto present_value(const instrument<U, C>& i, const pwflat::curve<T, F>& c,
		U u = std::numeric_limits<U>::infinity())
	{
		auto p = C(0)*F(0);

		const U* iu = i.time();
		const C* ic = i.cash();

		for (auto j = 0; j < i.size() && iu[j] <= u; ++j) {
			p += ic[j] * c.discount(iu[j]);
		}

		return p;
	}

	// Duration: derivative of present value wrt parallel shift of curve past t.
	template<class U = double, class C = double, class T = double, class F = double>
	inline auto duration(const instrument<U, C>& i, const pwflat::curve<T, F>& c, T t = 0)
	{
		auto p = C(0) * F(0);

		const U* iu = i.time();
		const C* ic = i.cash();

		for (auto j = 0; j < i.size(); ++j) {
			if (iu[j] > t) {
				p += -iu[j] * ic[j] * c.discount(iu[j]);
			}
		}

		return p;
	}
	// Bootstrap: extrapolate curve to reprice instrument
	template<class U = double, class C = double, class T = double, class F = double>
	inline F bootstrap(const instrument<U, C>& i, pwflat::curve<T, F>& c, F p = 0,
		F eps = sqrt(epsilon<F>), size_t N = 100)
	{
		// last instrument payment
		auto [u_, c_] = i.back();

		// last point on curve
		auto [t_, f_] = c.back();

		ensure(u_ > t_); // at least one cash flow past end of curve

		// extrapolate by _f initial guess
		F _f = c.size() != 0 ? f_ : 0.01;
		
		// present value and its derivative using extrapolated curve
		auto pv = [&i, &c](F f) { return present_value(i, c.extrapolate(f)); };
		auto dpv = [&i, &c, t_](F f) { return duration(i, c.extrapolate(f), t_); };
		
		while (fabs(pv(_f) - p) > eps) {
			ensure(N--);
			_f = _f - (pv(_f) - p)/dpv(_f); // use Newton-Raphson for root of pv - p
		}
		
		return _f;
	}
#ifdef _DEBUG
	inline int bootstrap_test()
	{
		{
			// initial cash deposit
			auto i = instrument({ 1. }, { 1.02 });
			auto c = pwflat::curve(0.03);
			auto p = present_value(i, c);// .extrapolate(0.03));
			auto _f = bootstrap(i, c, p);
			ensure(fabs(_f - .03) < 10*sqrt(epsilon<double>));
		}
		{
			auto i = instrument({ 1., 2. }, { 0.02, 1.02 });
			auto c = pwflat::curve({ 1. }, {0.03});
			auto p = present_value(i, c.extrapolate(0.04));
			auto _f = bootstrap(i, c, p);
			ensure(fabs(_f - .04) < 10 * sqrt(epsilon<double>));
		}
		{
			auto i = instrument({ 1., 2., 3.}, { 0.02, 0.02, 1.02 });
			auto c = pwflat::curve({ 1. }, { 0.03 });
			auto p = present_value(i, c.extrapolate(0.04));
			auto _f = bootstrap(i, c, p);
			ensure(fabs(_f - .04) < 10 * sqrt(epsilon<double>));
		}

		return 0;
	}
#endif // _DEBUG
	// Yield: constant forward rate y with p = present_value(i, curve(y))
	// Convexity

} // namespace fre::fixed_income

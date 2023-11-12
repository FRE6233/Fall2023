// fre_fixed_income.h - fixed income instrument valuation
#pragma once
#include <type_traits>
#include "fre_pwflat.h"

namespace fre::fixed_income {

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
	};

	// Present value is p = sum_i c_i D(u_i)
	template<class U = double, class C = double, class T = double, class F = double>
	inline auto present_value(const instrument<U, C>& i, const pwflat::curve<T, F>& c)
	{
		auto p = C(0)*F(0);

		p = i.size() * c.size(); // !!! implement

		return p;
	}

	// Duration
	// Yield
	// Convexity

} // namespace fre::fixed_income

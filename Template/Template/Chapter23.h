#pragma once

#include <array>
#include <type_traits>

#include "Chapter19.h"

namespace Note23dot1 {
	//C++14
	template<typename T>
	constexpr T sqrt(T x)
	{
		//special cases
		if(x <= 1) return x;

		T lo = 0, hi = x;
		for (;;) {
			auto mid = (hi + lo) / 2, midSquared = mid * mid;
			if (lo + 1 >= hi || midSquared == x) {
				return mid;
			}
			//half
			if (midSquared < x) {
				lo = mid;
			}
			else {
				hi = mid;
			}
		}
	}

	void test() {
		static_assert(sqrt(25) == 5, "");//compile time
		std::array<int, sqrt(40) + 1> arr;//compile time

		long long l = 53478;
		std::cout << sqrt(l) << '\n';//run time
	}

	//Type Metaprogramming

	//primary template
	template<typename T>
	struct RemoveAllExtentsT {
		using Type = T;
	};

	//partial specializations
	template<typename T, std::size_t SZ>
	struct RemoveAllExtentsT<T[SZ]> {
		using Type = typename RemoveAllExtentsT<T>::Type;
	};

	template<typename T>
	struct RemoveAllExtentsT<T[]> {
		using Type = typename RemoveAllExtentsT<T>::Type;
	};

	template<typename T>
	using RemoveAllExtents = typename RemoveAllExtentsT<T>::Type;
}

namespace Note21dot1dot3 {
	/*template<typename T, std::size_t N>
	auto dotProduct(std::array<T, N> const& x, std::array<T, N> const& y)
	{
		T result{};
		for (std::size_t k = 0; k < N; ++k) {
			result += x[k] * y[k];
		}
		return result;
	}*/

	template<typename T, std::size_t N>
	struct DotProductT {
		static inline T result(T* a, T* b) {
			return *a * *b + DotProductT<T, N - 1>::result(a + 1, b + 1);
		}
	};

	//partial specialization
	template<typename T>
	struct DotProductT<T, 0> {
		static inline T result(T*, T*) {
			return T{};
		}
	};

	template<typename T, std::size_t N>
	auto dotProduct(std::array<T, N> const& x,
		std::array<T, N> const& y)
	{
		return DotProductT<T, N>::result(x.begin(), y.begin());
	}
}

namespace Note23dot1dot4 {
	template<unsigned N, unsigned D = 1>
	struct Ratio {
		static constexpr unsigned num = N;//numerator
		static constexpr unsigned den = D;//denominator
		using Type = Ratio<num, den>;
	};

	/*
	a1		b1
	__	*	__
	a2		b2

	a1 * b2 + b1 * a2
	_________________
		a2 * b2
	*/

	//adding two raitios
	template<typename R1, typename R2>
	struct RatioAddImpl {
		private:
			static constexpr unsigned den = R1::den * R2::den;
			static constexpr unsigned num = R1::num * R2::den + R2::num * R1::den;
		public:
			typedef Ratio<num, den> Type;
	};

	//using declaration for convenient usage:
	template<typename R1, typename R2>
	using RatioAdd = typename RatioAddImpl<R1, R2>::Type;

	void test2() {
		using R1 = Ratio<1, 1000>;
		using R2 = Ratio<2, 3>;
		using RS = RatioAdd<R1, R2>;
		std::cout << RS::num << '/' << RS::den << '\n';//2003/3000
	}

	template<typename T, typename U = Ratio<1>>
	class Duration {
		public:
			using ValueType = T;
			using UnitType = typename U::Type;
		private:
			ValueType val;
		public:
			constexpr Duration(ValueType v = 0)
				:val(v) {

			}
			constexpr ValueType value() const {
				return val;
			}
	};

	//adding two durations where unit type might differ:
	template<typename T1, typename U1, typename T2, typename U2>
	auto constexpr operator+(Duration<T1, U1> const& lhs,
		Duration<T2, U2> const& rhs)
	{
		using VT = Ratio<1, RatioAdd<U1, U2>::den>;

		auto val = lhs.value() * VT::den / U1::den * U1::num +
					rhs.value() * VT::den / U2::den * U2::num;
		return Duration<decltype(val), VT>(val);
	}
}

namespace Note23dot2 {
	//primary template to compute sqrt(N)
	template<int N, int LO = 1, int HI = N>
	struct Sqrt {
		//compute the midpoint, rounded up
		static constexpr auto mid = (LO + HI + 1) / 2;

		//search a not too large value in a halved interval
		static constexpr auto value = (N < mid * mid) ? Sqrt<N, LO, mid - 1>::value
								: Sqrt<N, mid, HI>::value;
	};

	//partial specialization
	template<int N, int M>
	struct Sqrt<N, M, M> {
		static constexpr auto value = M;
	};
}

namespace Note23dot3dot1 {
	using namespace Note19dot7;
	//primary template for main recursive step
	template<int N, int LO = 1, int HI = N>
	struct Sqrt {
		//compute the midpoint, rounded up
		static constexpr auto mid = (LO + HI + 1) / 2;

		//search a not too large value in a havled interval
		using SubT = IfThenElse<(N * mid * mid),
					Sqrt<N, LO, mid - 1>,
					Sqrt<N, mid, HI>>;
		static constexpr auto value = SubT::value;
	};

	//partial specialization for end of recursion criterion
	template<int N, int S>
	struct Sqrt<N, S, S> {
		static constexpr auto value = S;
	};
}

namespace Note23dot6 {
	//primary template to compute 3 to the Nth
	template<int N>
	struct Pow3 {
		enum { value = 3 * Pow3<N - 1>::value };
	};

	//full specialization to end the recursion
	template<>
	struct Pow3<0> {
		enum { value = 1};
	};
}

namespace Note23dot6dot1 {
	template<int N>
	struct Pow3 {
		static int const value = 3 * Pow3<N - 1>::value;
	};

	template<>
	struct Pow3<0> {
		static int const value = 1;
	};
}

namespace Chapter23 {
	using namespace Note23dot1;
	using namespace Note23dot1dot4;
	void M_Test() {
		test();
		test2();
	}
}

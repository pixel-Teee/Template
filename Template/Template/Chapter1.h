#pragma once

#include <type_traits>

namespace Note1dot1dot {
	template<typename T>
	T max(T a, T b)
	{
		//T must could be copyable when return
		return b < a ? a : b;
	}

	template<typename T>
	T max2(T const& a, T const& b)
	{
		return b < a ? a : b;
	}

	template<typename T1, typename T2>
	T1 max3(T1 a, T2 b)
	{
		return b < a ? a : b;
	}

	template<typename T1, typename T2, typename RT>
	RT max4(T1 a, T2 b)
	{
		return b < a ? a : b;
	}

	template<typename T1, typename T2>
	auto max5(T1 a, T2 b)
	{
		return b < a ? a : b;
	}

	template<typename T1, typename T2>
	auto max6(T1 a, T2 b)->decltype(b < a ? a : b)
	{
		return b < a ? a : b;
	}

	template<typename T1, typename T2>
	auto max7(T1 a, T2 b)->typename std::decay<decltype(true ? a : b)>::type
	{
		return b < a ? a : b;
	}

	template<typename T1, typename T2>
	std::common_type_t<T1, T2> max8(T1 a, T2 b)
	{
		return b < a ? a : b;
	}

	template<typename T1, typename T2,
			typename RT = std::decay_t<decltype(true ? T1() : T2())>>
	RT max9(T1 a, T2 b)
	{
		return b < a ? a : b;
	}

	template<typename T1, typename T2>
	constexpr auto max10(T1 a, T2 b)
	{
		return b < a ? a : b;
	}

	//typename std::common_type<T1, T2>::type
	//std::decay
	//std::declval
	//std::ref std::cref

	void test()
	{
		int i = 42;
		std::cout << "max(7, i): " << max(7, i) << std::endl;

		std::cout << max4<int, double, double>(4, 7.2) << std::endl;

		std::cout << max5(3, 4) << std::endl;

		std::cout << max8(3, 4) << std::endl;

		std::cout << max10(4, 5) << std::endl;
	}
}

namespace Chapter1 {
	using namespace Note1dot1dot;
	void M_Test() {
		test();
	}
}
#pragma once

#include <iostream>
#include <type_traits>

namespace Note8dot1 {
	template<unsigned p, unsigned d> //p:number to check d:current divisor
	struct DoIsPrime {
		static constexpr bool value = (p % d != 0) && DoIsPrime<p, d - 1>::value;
	};

	template<unsigned p> //end recursion if divisor is 2
	struct DoIsPrime<p, 2>{
		static constexpr bool value = (p % 2 != 0);
	};

	template<unsigned p> //primary template
	struct IsPrime {
		static constexpr bool value = DoIsPrime<p, p / 2>::value;
	};

	//special cases
	template<>
	struct IsPrime<0> { static constexpr bool value = false;};
	template<>
	struct IsPrime<1> { static constexpr bool value = false;};
	template<>
	struct IsPrime<2> { static constexpr bool value = true; };
	template<>
	struct IsPrime<3> { static constexpr bool value = true; };

	//C++11
	constexpr bool doIsPrime(unsigned p, unsigned d)
	{
		return	d != 2 ? (p % d != 0) && doIsPrime(p, d - 1)
				:(p % 2 != 0);//end recusion if divisor is 2
	}

	constexpr bool isPrime(unsigned p)
	{
		return p < 4 ? !(p < 2) //1 is not a prime
						: doIsPrime(p, p / 2);//start recursion with divisor from p / 2
	}

	////c++14
	//constexpr bool isPrime(unsigned int p)
	//{
	//	for (unsigned int d = 2; d <= p / 2; ++d) {
	//		if(p % d != 0)
	//			return false;
	//	}
	//	return p > 1;
	//}

	void test() {
		std::cout << IsPrime<9>::value << std::endl;
	}
}

namespace Note8dot3 {
	using namespace Note8dot1;

	template<int SZ, bool = isPrime(SZ)>
	struct Helper
	{

	};

	template<int SZ>
	struct Helper<SZ, true>
	{

	};

	template<typename T, std::size_t SZ>
	long foo(std::array<T, SZ> const& coll)
	{
		Helper<SZ> h;//implementation depends on whether array has prime number as size
	}
}

namespace Note8dot4 {
	//number of elements in a raw array:
	template<typename T, unsigned N>
	std::size_t len(T(&)[N])
	{
		return N;
	}

	//number of elements for a type having size_type:
	template<typename T>
	typename T::size_type len(T const& t)
	{
		return t.size();
	}
}

namespace Noote8dot4dot1 {
	template<typename T, unsigned N>
	std::size_t len(T(&)[N])
	{
		return N;
	}

	template<typename T>
	typename T::size_type len(T const& t)
	{
		return t.size();
	}

	//SFINAE
	//worst match
	//fallback for all other types
	std::size_t len(...)
	{
		return 0;
	}
}

namespace Note8dot4dot1 {
	class thread {
		public:
			template<typename F, typename... Args,
			typename = std::enable_if_t<!std::is_same_v<std::decay_t<F>, thread>>>
			explicit thread(F&& f, Args&&... args);
	};
}

namespace Note8dot4dot2 {
	template<typename T>
	typename T::size_type len(T const& t)
	{
		return t.size();
	}

	template<typename T>
	auto len(T const& t) -> decltype((void)(t.size()), T::size_type())
	{
		return t.size();
	}
}

namespace Note8dot5 {
	template<typename T>
	void foo(T t)
	{
		//T is complete type
		if constexpr (std::is_integral_v<T>) {
			if (t > 0) {
				foo(t - 1);
			}
		}	
		else
		{

		}
	}
}

namespace Chapter8 {
	using namespace Note8dot1;
	void M_Test() {
		test();
	}
}
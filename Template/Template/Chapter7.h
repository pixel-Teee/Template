#pragma once

#include <string>
#include <type_traits>

namespace Note7dot1 {
	template<typename T>
	void printV(T arg) {

	}

	void test() {
		//std::string returnString();
		std::string s = "hi";

		printV(s);//copy constructor
		printV(std::string(s));//copying usually optimized away(if not, move constructor)
		//printV(returnString());//copying usually optimized away(if not, move constructor)
		printV(std::move(s));//move constructor
	}
}

namespace Note7dot2 {
	template<typename T>
	void printR(T const& arg) {

	}

	//template<typename T>
	//void outR(T& arg) {
	//	if (std::is_array<T>::value) {
	//		std::count << "got array of " << std::extent<T>::value << " elems\n";
	//	}
	//}
	
	template<typename T>
	void outR(T& arg) {
		static_assert(!std::is_const<T>::value, "out parameter of foo<T>(T&) is const");
		//
	}

	/*template<typename T,
			typename = std::enable_if_t<!std::is_const<T>::value>>
	void outR(T& arg) {

	}*/

	template<typename T>
	void passR(T&& arg)
	{
		T x;//for passed lvalues, x is a reference, which requires an initializer
	}

	void test2() {
		//std::string returnString();
		std::string s = "hi";

		printR(s);//no copy
		printR(std::string("hi"));//no copy
		//printR(returnString());//no copy
		printR(std::move(s));//no copy
	}
}

namespace Note7dot3 {
	void printString(std::string const& s) {
		std::cout << s << '\n';
	}

	template<typename T>
	void printT(T arg) {
		printString(arg);
	}
	
	void test3() {
		std::string s = "hello";
		printT(s);
		printT(std::cref(s));
	}
}

namespace Note7dot4 {
	template<typename T, std::size_t L1, std::size_t L2>
	void foo(T(&arg)[L1], T(&arg2)[L2])
	{
		T* pa = arg1;//decay arg1
		T* pb = arg2;//decay arg2
		//....
	}

	template<typename T, typename = std::enable_if_t<std::is_array_v<T>>>
	void foo(T&& arg1, T&& arg2)
	{

	}
}

namespace Note7dot5 {
	template<typename T>
	typename std::remove_reference<T>::type retV(T p)
	{
		return T{};
	}

	template<typename T>
	typename std::decay<T>::type retV(T p)
	{
		return T{};
	}

	//C++14
	template<typename T>
	auto retV(T p)
	{
		return T{};
	}
}

namespace Chapter7 {
	using namespace Note7dot2;
	void M_Test() {
		test2();
	}
}
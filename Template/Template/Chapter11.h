#pragma once

#include <iostream>
#include <vector>
#include <utility>
#include <functional>

namespace Note11dot1dot1 {
	template<typename Iter, typename Callable>
	void foreach(Iter current, Iter end, Callable op)
	{
		while (current != end)
		{
			op(*current);
			++current;
		}
	}

	void func(int i)
	{
		std::cout << "func() called for: " << i << '\n';
	}

	class FuncObj {
		public:
			void operator() (int i) const {
				std::cout << "FuncObj::op() called for: " << i << '\n';
			}
	};
	
	void test()
	{
		std::vector<int> primes = {2, 3, 5, 7, 11, 13, 17, 19};

		foreach(primes.begin(), primes.end(),
				func);//decays to pointer

		foreach(primes.begin(), primes.end(),
				&func);

		foreach(primes.begin(), primes.end(),
			FuncObj());

		foreach(primes.begin(), primes.end(),
			[](int i) {
				std::cout << "lambda called for: " << i << '\n';
			});
	}
}

namespace Note11dot1dot2 {
	class MyClass {
		public:
			void memfunc(int i) const {
				std::cout << "MyClass::memfunc() called for: " << i << '\n';
			}
	};

	template<typename Iter, typename Callable, typename...Args>
	void foreach(Iter current, Iter end, Callable op, Args const&... args)
	{
		while (current != end)
		{
			std::invoke(op,
						args...,
						*current
			);

			++current;
		}
	}

	void test2() {
		std::vector<int> primes = {2, 3, 5, 7, 11, 13, 17, 19};

		//-value to first parameter, vector element to second parameter
		foreach(primes.begin(), primes.end(),
				[](std::string const& prefix, int i){
					std::cout << prefix << i << '\n';
				}, "- value: ");

		MyClass obj;
		foreach(primes.begin(), primes.end(),
				&MyClass::memfunc,
				obj);
	}
}

namespace Note11dot1dot3 {
	template<typename Callable, typename... Args>
	decltype(auto) call(Callable&& op, Args&&...args)
	{
		if constexpr (std::is_same_v<std::invoke_result_t<Callable, Args...>, void>)
		{
			//return type is void
			std::invoke(std::forward<Callable>(op),
				std::forward<Args>(args)...);

			return;
		}
		else
		{
			decltype(auto) ret{std::invoke(std::forward<Callable>(op),
											std::forward<Args>(args)...)};
			return ret;
		}
	}

	void test3() {

	}
}

namespace Note11dot1dot4 {
	template<typename T>
	class C {
		//ensure T is not void(ignoring const or volatile)
		static_assert(!std::is_same_v<std::remove_cv_t<T>, void>,
						"invalid instantiation of Class C for void type");

		public:
			template<typename V>
			void f(V&& v) {
				if constexpr (std::is_reference_v<T>) {
					//T is reference
				}
				if constexpr (std::is_convertible_v<std::decay_t<V>, T>)
				{
					//v is convertible to T
				}
				if constexpr (std::has_virtual_destructor_v<V>) 
				{
					//v has virtual destructor
				}
			}
	};
}

namespace Note11dot2dot3 {
	template<typename T1, typename T2,
			typename RT = std::decay<decltype(true ? std::declval<T1>() : std::declval<T2>())>>
	RT max(T1 a, T2 b)
	{
		return b < a ? a : b;
	}
}

namespace Note11dot3 {
	//template<typename T>
	//void foo(T x)
	//{
	//	//temporaries value
	//	set(get(x));
	//}

	template<typename T>
	void foo(T x)
	{
		auto&& val = get(x);

		set(std::forward<decltype(val)>(val));
	}
}

namespace Note11dot4 {
	template<typename T>
	void tmplParamIsReference(T) {
		std::cout << "T is reference: " << std::is_reference_v<T> << '\n';
	}

	void test4()
	{
		std::cout << std::boolalpha;

		int i;
		int& r = i;
		tmplParamIsReference(i); //false
		tmplParamIsReference(r);	//false
		tmplParamIsReference<int&>(i);//true
		tmplParamIsReference<int&>(r);//true
	}

	template<typename T, T Z = T{}>
	class RefMem {
		private:
			T zero;
		public:
			RefMem() : zero{ Z } {

			}
	};

	int null = 0;

	void test5() {
		//RefMem<int> rm1, rm2;
		//rm1 = rm2; //OK

		//RefMem<int&> rm3;//invalid default value for N
		//RefMem<int&, 0> rm4;//invalid default value for N

		//extern int null;
		//RefMem<int&, null> rm5, rm6;
		//rm5 = rm6;//ERROR£ºoperator = is deleted due to reference member
	}

	template<typename T, int& SZ>
	class Arr {
		private:
			std::vector<T> elems;
		public:
			Arr() : elems(SZ) {

			}
			void print() const {
				for (int i = 0; i < SZ; ++i) {
					std::cout << elems[i] << ' ';
				}
			}
	};

	int size = 10;

	void test6() {
		//Arr<int&, size> y;//std::vector could not owner the reference type
		//Arr<int, size> x;//initializes internal vector with 10 elements
		//x.print();//OK
		//size += 100;//OOPS: modifes SZ in Arr<>
		//x.print();//run-time ERROR: invalid memry access
	}

	//template<typename T1, typename T2>
	//struct pair {
	//	T1 first;
	//	T2 second;

	//	//default copy/move constructors are OK even with references
	//	pair(pair const&) = default;
	//	pari(pair&&) = default;

	//	//but assignment operator have to be defined to be available with references:
	//	pair& operator=(pair const& p);
	//	pair& operator=(pair&& p) noexcept(...);
	//};

	template<typename T>
	class optional {
		static_assert(!std::is_reference<T>::value,
						"Invalid instantiation of optional<T> for references");
	};
}

namespace Note11dot5 {
	template<typename T>
	class Cont {
		private:
			T* elems;
		public:
			//...
			template<typename D = T>
			typename std::conditional<std::is_move_constructible<D>::value,
										T&&,
										T&>::type
			foo();
	};

	struct Node {
		std::string value;
		Cont<Node> next;//only possible if Cont accepts incomplete types
	};
}

namespace Chapter11 {
	using namespace Note11dot1dot1;
	using namespace Note11dot1dot2;
	using namespace Note11dot4;
	void M_Test() {
		test();
		test2();
		test4();
		test5();
	}
}
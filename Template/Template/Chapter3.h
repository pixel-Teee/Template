#pragma once

#include <assert.h>
#include <iostream>
#include <algorithm>
#include <array>
#include <utility>
#include <type_traits>

namespace Note3dot1{
	
	//Maxsize describe the size of the internal array of stack elements
	template<typename T, std::size_t Maxsize = 100>
	class Stack {
		private:
			std::array<T, Maxsize> elems;
			std::size_t numElems;
		public:
			Stack();
			void push(T const& elem);
			void pop();
			T const& top() const;
			bool empty() const {
				return numElems == 0;
			}
			std::size_t size() const {
				return numElems;
			}
	};

	template<typename T, std::size_t Maxsize>
	Stack<T, Maxsize>::Stack()
		:numElems(0)
	{
		//nothing else to do
	}

	template<typename T, std::size_t Maxsize>
	void Stack<T, Maxsize>::push(T const& elem)
	{
		//run assert
		assert(numElems < Maxsize);
		elems[numElems] = elem;
		++numElems;
	}

	template<typename T, std::size_t Maxsize>
	void Stack<T, Maxsize>::pop()
	{
		assert(!elems.empty());
		--numElems;
	}

	template<typename T, std::size_t Maxsize>
	T const& Stack<T, Maxsize>::top() const
	{
		assert(!elems.empty());
		return elems[numElems - 1];
	}

	void test()
	{
		Stack<int, 20> int20Stack;

		int20Stack.push(40);

		std::cout << int20Stack.top() << std::endl;
	}
}

namespace Note3dot2 {
	template<int Val, typename T>
	T addValue(T x)
	{
		return x + Val;
	}

	template<auto Val, typename T = decltype(Val)>
	T foo();

	template<typename T, T Val = T{}>
	T bar();

	void test2()
	{
		std::array<int, 5> a{1, 2, 3, 4, 5};
		std::array<int, 5> b;
		std::transform(a.begin(), a.end(),
						b.begin(), addValue<5, int>);
		for (size_t i = 0; i < b.size(); ++i)
		{
			std::cout << b[i] << " ";
		}
		std::cout << std::endl;
	}
}

namespace Note3dot4 {

	//previous is size_t Maxsize
	template<typename T, auto Maxsize>
	class Stack {
		public:
			//there 
			using size_type = decltype(Maxsize);
		private:
			std::array<T, Maxsize> elems;
			size_type numElems;
		public:
			Stack();
			void push(T const& elem);
			void pop();
			T const& top() const;
			bool empty() const {
				return numElems == 0;
			}
			size_type size() const {
				return numElems;
			}
	};

	//constructor
	template<typename T, auto Maxsize>
	Stack<T, Maxsize>::Stack()
		:numElems(0)
	{
		//nothing else to do
	}

	template<typename T, auto Maxsize>
	void Stack<T, Maxsize>::push(T const& elem)
	{
		assert(numElems < Maxsize);
		elems[numElems] = elem;
		++numElems;
	}

	void test3() {

		Stack<int, 20u> int20Stack;
		Stack<std::string, 40> stringStack;

		auto size1 = int20Stack.size();
		auto size2 = stringStack.size();

		if (!std::is_same_v<decltype(size1), decltype(size2)>) {
			std::cout << "size types differ" << '\n';
		}
	}
}

namespace Chapter3 {
	using namespace Note3dot1;
	using namespace Note3dot2;
	using namespace Note3dot4;
	void M_Test() {
		test();
		test2();
		test3();
	}
}
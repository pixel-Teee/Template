#pragma once

#include <assert.h>
#include <iostream>
#include <vector>
#include <queue>

namespace Note2dot1 {
	template<typename T>
	class Stack;
	template<typename T>
	std::ostream& operator<<(std::ostream&, Stack<T> const&);

	template<typename T>
	class Stack {
		private:
			std::vector<T> elems;//elements

		public:
			void push(T const& elem);//push element
			T pop();//pop element
			T const& top() const;//return top element
			bool empty() const {//return whether the stack is empty
				return elems.empty();
			}

			void printOn(std::ostream& strm) const {
				for (T const& elem : elems) {
					strm << elem << ' ';
				}
			}

			/*
			friend std::ostream& operator<<(std::ostream& strm, Stack<T> const& s) {
				s.printOn(strm);
				return strm;
			}
			*/
			/*
			template<typename U>
			friend std::ostream& operator<<(std::ostream&, Stack<U> const&);
			*/
			friend std::ostream& operator<< <T> (std::ostream&, Stack<T> const&);
	};

	template<typename T>
	std::ostream& operator<<(std::ostream& strm, Stack<T> const& s)
	{
		s.printOn(strm);
		return strm;
	}

	template<typename T>
	void Stack<T>::push(T const& elem)
	{
		elems.push_back(elem);//append copy of passed elem
	}

	template<typename T>
	T Stack<T>::pop()
	{
		//rum-time assert
		assert(!elems.empty());
		T elem = elems.back();
		elems.pop_back();
		return elem;
	}

	template<typename T>
	T const& Stack<T>::top() const
	{
		assert(!elems.empty());
		return elems.back();
	}

	//deduction guides
	Stack(char const*)->Stack<std::string>;

	//specialize
	template<>
	class Stack<std::string> {
		private:
			std::deque<std::string> elems;

		public:
			void push(std::string const&);
			void pop();
			std::string const& top() const;
			bool empty() const {
				return elems.empty();
			}
	};

	void Stack<std::string>::push(std::string const& elem)
	{
		elems.push_back(elem);
	}

	void Stack<std::string>::pop()
	{
		assert(!elems.empty());
		elems.pop_back();
	}

	std::string const& Stack<std::string>::top() const
	{
		assert(!elems.empty());
		return elems.back();
	}

	//partial specialization
	template<typename T>
	class Stack<T*> {
		private:
			std::vector<T*> elems;

		public:
			void push(T*);
			T* pop();
			T* top() const;
			bool empty() const{
				return elems.empty();
			}
	};

	template<typename T>
	void Stack<T*>::push(T* elem)
	{
		elems.push_back(elem);
	}

	template<typename T>
	T* Stack<T*>::pop()
	{
		assert(!elems.empty());
		T* p = elems.back();
		elems.pop_back();
		return p;
	}

	template<typename T>
	T* Stack<T*>::top() const
	{
		assert(!elems.empty());
		return elems.back();
	}

	void test()
	{
		Stack<int> intStack;
		intStack.push(5);

		Stack<std::string> s;
		s.push("QAQ");
		s.push("I love you!");

		//std::cout << intStack.top() << std::endl;

		std::cout << intStack << std::endl;

		Stack<int*> ptrStack;
		ptrStack.push(new int{42});
		std::cout << *ptrStack.top() << '\n';
		delete ptrStack.pop();
	}
}

namespace Note2dot7 {
	template<typename T, typename Cont = std::vector<T>>
	class Stack {
		private:
			Cont elems;
		public:
			void push(T const& elem);
			void pop();
			T const& top() const;
			bool empty() const {
				return elems.empty();
			}
	};

	template<typename T, typename Cont>
	void Stack<T, Cont>::push(T const& elem)
	{
		elems.push_back(elem);
	}

	template<typename T, typename Cont>
	void Stack<T, Cont>::pop()
	{
		assert(!elems.empty());
		elems.pop_back();
	}

	template<typename T, typename Cont>
	T const& Stack<T, Cont>::top() const
	{
		assert(!elems.empty());
		return elems.back();
	}

	void test2()
	{
		Stack<double, std::deque<double>> dbStack;
		dbStack.push(3.4);

		std::cout << dbStack.top() << std::endl;
	}
}

namespace Note2dot10 {
	template<typename T>
	struct ValueWithComment {
		T value;
		std::string comment;
	};

	//deduction guide
	ValueWithComment(char const*, char const*)->ValueWithComment<std::string>;

	void test3()
	{
		ValueWithComment<int> vc;
		vc.value = 42;
		vc.comment = "initial value";

		ValueWithComment v2 = {"hello", "initialize value"};
		
	}
}

namespace Chapter2 {
	using namespace Note2dot1;
	using namespace Note2dot7;
	void M_Test() {
		test();
		test2();
	}
}
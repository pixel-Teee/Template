#pragma once

#include <iterator>
#include <vector>
#include <deque>
#include <bitset>

namespace Note5dot1 {
	template<typename T>
	void printcoll(T const& coll)
	{
		typename T::const_iterator pos;
		typename T::const_iterator end(coll.end());

		for (pos = coll.begin(); pos != end; ++pos) {
			std::cout << *pos << ' ';
		}
		std::cout << '\n';
	}

	/*
	class stlcontainter{
		public:
			using iterator = ...;//iterator for read/write access
			using const_iterator = ...;//iterator for read access
	}
	*/
}

namespace Note5dot4 {
	//solve raw arrays or string iterals
	template<typename T, int N, int M>
	bool less(T(&a)[N], T(&b)[M])
	{
		for (int i = 0; i < N && i < M; ++i)
		{
			if(a[i] < b[i]) return true;
			if(b[i] < a[i]) return false;
		}
		return N < M;
	}
	//for string literals(and other char arrays)
	template<int N, int M>
	bool less(char const(&a)[N], char const(&b)[M])
	{
		for (int i = 0; i < N && i < M; ++i)
		{
			if (a[i] < b[i]) return true;
			if (b[i] < a[i]) return false;
		}
		return N < M;
	}

	//class template 
	template<typename T>
	struct MyClass;//primary template

	//partial specialization for arrays of known bounds
	template<typename T, std::size_t SZ>
	struct MyClass<T[SZ]>
	{
		static void print(){ std::cout << "print() for T[" << SZ << "]\n"; }
	};

	//partial spec, for references to arrays of known bounds
	template<typename T, std::size_t SZ>
	struct MyClass<T(&)[SZ]>
	{
		static void print() { std::cout << "print() for T(&)[" << SZ << "]\n"; }
	};

	//partial specialization for arrays of unknown bounds
	template<typename T>
	struct MyClass<T[]>
	{
		static void print(){std::cout << "print() for T[]\n"; }
	};

	//partial spec. for references to arrays of unknown bounds
	template<typename T>
	struct MyClass<T(&)[]>
	{
		static void print(){std::cout << "print() for T(&)[]\n"; }
	};

	//partial specialization for pointers
	template<typename T>
	struct MyClass<T*>
	{
		static void print(){std::cout << "print() for T*\n"; }
	};

	template<typename T1, typename T2, typename T3>
	void foo(int a1[7], int a2[],//pointers by language rules
		int(&a3)[42], int(&x0)[],//reference to array of known bound and unknown reference
		T1 x1,//passing by value decays
		T2& x2, T3&& x3)//passing by reference
	{
		MyClass<decltype(a1)>::print(); //MyClass<T*>
		MyClass<decltype(a2)>::print(); //MyClass<T*>
		MyClass<decltype(a3)>::print(); //MyClass<T(&)[sz]>
		MyClass<decltype(x0)>::print(); //MyClass<T(&)[]>
		MyClass<decltype(x1)>::print(); //MyClass<T*>
		MyClass<decltype(x2)>::print(); //MyClass<T(&)[]>
		MyClass<decltype(x3)>::print(); //MyClass<T(&)[]>
	}

	//define for forward declare
	int x[] = {0, 8, 15};

	void test()
	{
		int a[42];
		MyClass<decltype(a)>::print();//MyClass<T[sz]>

		extern int x[];//forward declare array
		MyClass<decltype(x)>::print();//use MyClass<T[]>

		foo(a, a, a, x, x, x, x);
	}


}

namespace Note5dot5 {
	template<typename T>
	class Stack {
		private:
			std::deque<T> elems;//elements
		public:
			void push(T const&);//push element
			void pop();//pop element
			T const& top() const;//return top element
			bool empty() const {	//return whether the stack is empty
				return elems.empty();
			}

			//assign stack of elements of type T2
			template<typename T2>
			Stack& operator=(Stack<T2> const&);

			//to get access to private member of Stack<T2> for any type T2
			template<typename> friend class Stack;
	};

	////assigment operator
	//template<typename T>
	//template<typename T2>
	//Stack<T>& Stack<T>::operator=(Stack<T2> const& op2)
	//{
	//	Stack<T2> tmp(op2);	//create a copy of the assigned stack

	//	elems.clear();	//remove existing elements
	//	while (!tmp.empty()) {
	//		elems.push_front(tmp.top())
	//		tmp.pop();
	//	}
	//	return *this;
	//}

	template<typename T>
	template<typename T2>
	Stack<T>& Stack<T>::operator=(Stack<T2> const& op2)
	{
		elems.clear();

		elems.insert(elems.begin(),
					op2.elems.begin(),
					op2.elems.end());

		return *this;
	}
}

namespace Note5dot5dot1 {
	//add template typename container type
	template<typename T, typename Cont = std::deque<T>>
	class Stack {
		private:
			Cont elems;//elements

		public:
			void push(T const&);
			void pop();
			T const& top() const;
			bool empty() const {
				return elems.empty();
			}

			template<typename T2, typename Cont2>
			Stack& operator=(Stack<T2, Cont2> const&);

			template<typename, typename> friend class Stack;
	};

	template<typename T, typename Cont>
	template<typename T2, typename Cont2>
	Stack<T, Cont>&
		Stack<T, Cont>::operator=(Stack<T2, Cont2> const& op2)
	{
		elems.clear();
		elems.insert(elems.begin(),
					op2.elems.begin(),
					op2.elems.end());
		return *this;
	}
}

namespace Note5dot5dot2 {
	class BoolString {
		private:
			std::string value;
		public:
			BoolString(std::string const& s)
				:value(s) {

			}
			template<typename T = std::string>
			T get() const {
				return value;
			}
	};

	//full specialization
	template<>
	inline bool BoolString::get<bool>() const {
		return value == "true" || value == "1" || value == "on";
	}
}

namespace Note5dot5dot3 {
	template<unsigned long N>
	void printBitset(std::bitset<N> const& bs)
	{
		std::cout << bs.template to_string<char, std::char_traits<char>, 
					std::allocator<char>>();
	}
}

namespace Note5dot6 {
	//variable template
	template<typename T = long double>
	constexpr T pi{3.1415926535897932385};

	void test2()
	{
		std::cout << pi<double> << '\n';
		//std::cout << pi<int> << '\n';//error
		std::cout << pi<float> << '\n';
	}

	//variable templates can also be parameterizaed by nontype parameter
	template<int N>
		std::array<int, N> arr{};//array with N elements, zero-initialized

	template<auto N>
	constexpr decltype(N) dval = N;//type of dval depends on passed value


	void test3() {
		std::cout << dval<'c'> << '\n';//dval has value'c' of type char
		arr<10>[0] = 42;//set first element of global arr
		for (std::size_t i = 0; i < arr<10>.size(); ++i) {
			std::cout << arr<10>[i] << '\n';//uses values set in arr
		}
	}

	//the application of variable template
	template<typename T>
	class MyClass {
		public:
			static constexpr int max = 1000;
	};

	/*template<typename T>
	class numeric_limits {
		public:
			static constexpr bool is_signed = false;
	};

	template<typename T>
	constexpr bool isSigned = std::numeric_limits<T>::is_signed;*/
}

namespace Note5dot7 {
	//default template type argument form std::deque<T> to std::deque
	template<typename T, template<typename> class Cont = std::deque>
	class Stack {
		private:
			//Cont's type paramter depend on first template type
			//so it don't need to specify interal element type
			Cont<T> elems;

		public:
			void push(T const&);
			void pop();
			T const& top() const;
			bool empty() const {
				return elems.empty();
			}
	};
}

namespace Note5dot8 {
	//complete version of Stack
	template<typename T,
			template<typename Elem,
					typename = std::allocator<Elem>>
			class Cont = std::deque>
	class Stack {
		private:
			Cont<T> elems;

		public:
			void push(T const&);
			void pop();
			T const& top() const;
			bool empty() const {
				return elems.empty();
			}

			//assign stack of elements of type T2
			template<typename T2,
					template<typename Elem2,
					typename = std::allocator<Elem2>
					>class Cont2>
			Stack<T, Cont>& operator=(Stack<T2, Cont2> const&);

			//to get access to private members of any Stack with elements of type T2
			template<typename, template<typename, typename>class>
			friend class Stack;
	};

	template<typename T, template<typename, typename> class Cont>
	void Stack<T, Cont>::push(T const& elem)
	{
		elems.push_back(elem);
	}

	template<typename T, template<typename, typename> class Cont>
	void Stack<T, Cont>::pop()
	{
		//run-time assert
		assert(!elems.empty());
		elems.pop_back();
	}

	template<typename T, template<typename, typename> class Cont>
	T const& Stack<T, Cont>::top() const
	{
		assert(!elems.empty());
		return elems.back();
	}

	//template member function
	template<typename T, template<typename, typename> class Cont>
	template<typename T2, template<typename, typename> class Cont2>
	Stack<T, Cont>&
		Stack<T, Cont>::operator=(Stack<T2, Cont2> const& op2)
	{
		elems.clear();
		elems.insert(elems.begin(),
					op2.elems.begin(),
					op2.elems.end());
		return *this;
	}

	void test4()
	{
		Stack<int> iStack;
		iStack.push(1);
		iStack.push(2);

		std::cout << "iStack.top(): " << iStack.top() << '\n';
	}
}

namespace Chapter5 {
	
	using namespace Note5dot1;
	using namespace Note5dot4;
	using namespace Note5dot6;
	using namespace Note5dot8;
	void M_Test() {
		std::vector<int> v{1, 2, 3, 4, 5};

		printcoll(v);

		test();
		test2();
		test4();
	}
}

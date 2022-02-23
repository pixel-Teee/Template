#pragma once

#include <iostream>
#include <type_traits>

namespace Note6dot1{

	class X {

	};

	void g(X&) {
		std::cout << "g() for variable\n";
	}

	void g(X const&) {
		std::cout << "g() for constant\n";
	}

	void g(X&&) {
		std::cout << "g() for movable object\n";
	}

	//let f() forward argument val to g()
	void f(X& val) {
		g(val); //call g(X&)
	}

	void f(X const& val) {
		g(val);//call g(X const&)
	}

	void f(X&& val) {
		g(std::move(val));//cal g(X&&)
	}

	template<typename T>
	void f(T&& val) {
		g(std::forward<T>(val));//prefect forward val to g()
	}

	void test() {
		X v;
		X const c;

		f(v);//f(X&)
		f(c);//f(X const&)
		f(X());//cal f(X&&)
		f(std::move(v));//call f(X&&)
	}
}

namespace Note6dot2 {
	/*
	class Person {
		private:
			std::string name;
		public:
			//constructor for passed initial name:
			explicit Person(std::string const& n) : name(n) {
				std::cout << "copy string-CONSTR for '" << name << "'\n";
			}

			explicit Person(std::string&& n) : name(std::move(n)) {
				std::cout << "moving string-CONSTR for '" << name << "'\n";
			}

			//copy and move constructor
			Person(Person const& p) : name(p.name) {
				std::cout << "COPY-CONSTR Person '" << name << "'\n";
			}

			Person(Person&& p) : name(std::move(p.name)) {
				std::cout << "MOVE-CONSTR Person '" << name << "'\n";
			}
	};
	*/

	class Person {
		private:
			std::string name;
		public:
			template<typename STR>
			explicit Person(STR&& n) : name(std::forward<STR>(n)) {
				std::cout << "TMPL-CONSTR for '" << name << "'\n";
			}

			//copy and move constructor
			//lvalue const
			Person(Person const& p) : name(p.name) {
				std::cout << "COPY-CONSTR Person '" << name << "'\n";
			}

			Person(Person&& p) : name(std::move(p.name)) {
				std::cout << "MOVE-CONSTR Person '" << name << "'\n";
			}
	};
	void test2()
	{
		/*
		std::string s = "sname";

		Person p1(s);//Person(std::string const& n)
		Person p2("tmp");//Person(std::string&& n)
		Person p3(p1);//Person(Person const& p)
		Person p4(std::move(p1));//Person(Person&& p)
		*/
		std::string s = "sname";
		Person p1(s);//call Person(STR&& n)
		//Person p2("tmp");//const char[4], call Person(STR&& n)

		//Person p3(p1);//error
	}
}

namespace Note6dot3 {
	template<typename T>
	typename std::enable_if<(sizeof(T) > 4)>::type
	foo() {

	}
	
	template<typename T,
			typename = std::enable_if_t<(sizeof(T) > 4)>>
	void foo() {

	}

	template<typename T>
	using EnableIfSizeGreater4 = std::enable_if_t<(sizeof(T) > 4)>;

	/*
	template<typename T,
			typename = EnableIfSizeGreater4<T>>
		void foo()
	{

	}
	*/
}

namespace Note6dot4 {
	//define our constraint
	template<typename T>
	using EnableIfString = std::enable_if_t<
							std::is_convertible_v<T, std::string>>;

	class Person {
	private:
		std::string name;
	public:
		template<typename STR,
				typename = EnableIfString<STR>
				>
		explicit Person(STR&& n) : name(std::forward<STR>(n)) {
			std::cout << "TMPL-CONSTR for '" << name << "'\n";
		}

		//copy and move constructor
		//lvalue const
		Person(Person const& p) : name(p.name) {
			std::cout << "COPY-CONSTR Person '" << name << "'\n";
		}

		Person(Person&& p) : name(std::move(p.name)) {
			std::cout << "MOVE-CONSTR Person '" << name << "'\n";
		}
	};

	void test3()
	{
		Person p1("I Love You");
		Person p2(p1);
	}

	//intergal: check the U is the complete type
	template<typename T>
	class C {
		public:
			C(C const volatile&) = delete;

		template<typename U, typename = std::enable_if_t<!std::integral<U>::value>>
		C(<U> const&) {
			std::cout << "tmpl copy constructor\n";
		}
	};
}

namespace Note6dot5 {

	template<typename T>
	concept ConvertiableToString = std::is_convertible_v<T, std::string>;
	class Person {
	private:
		std::string name;
	public:
		template<ConvertiableToString STR>
		//requires std::is_convertible_v<STR, std::string>
		//requires ConvertiableToString<STR>
			Person(STR&& n) : name(std::forward<STR>(n)) {

		}
	};
}

namespace Chapter6 {
	using namespace Note6dot1;
	using namespace Note6dot2;
	using namespace Note6dot4;
	void M_Test() {
		//test2();
		test3();
	}

	template<typename T>
	using EnableIfString = std::enable_if_t<
		std::is_constructible_v<std::string, T>>;
}
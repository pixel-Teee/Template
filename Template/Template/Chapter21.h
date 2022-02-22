#pragma once

#include <iostream>

namespace Note21dot1 {

	//a empty class requires one bit at run-time
	//because it need to compute distance between pointers/addresses
	class EmptyClass {
		
	};

	//---------test ebco---------
	class Empty {
		using Int = int;
	};

	class EmptyToo : public Empty {

	};

	class EmptyThree : public EmptyToo {

	};

	//constraint of the ebco
	//two base empty need offset at 0 and 1
	class NonEmpty : public Empty, public EmptyToo {

	};

	void test()
	{
		std::cout << "sizeof(Empty):      " << sizeof(Empty) << '\n';
		std::cout << "sizeof(EmptyToo):   " << sizeof(EmptyToo) << '\n';
		std::cout << "sizeof(EmptyThree): " << sizeof(EmptyThree) << '\n';
		std::cout << "sizeof(NonEmpty):   " << sizeof(NonEmpty) << '\n';
	}

	/*
	template<typename T1, typename T2>
	class MyClass {
		private:
			T1 a;
			T2 b;
	};
	*/

	//also have their problem
	//t1 and t2 could not substituted with nonclasstype or union type
	//substituted with same type are also have problem
	//t1 or t2 will final
	template<typename T1, typename T2>
	class MyClass : private T1, private T2 {

	};

	/*
	template<typename CustomClass>
	class Optimizable {
		private:
			CustomClass info;//might be empty
			void* storage;
	};
	*/

	//base class type and member type
	template<typename Base, typename Member>
	class BaseMemberPair : private Base {
		private:
			Member mem;
		public:
			//constructor
			BaseMemberPair(Base const& b, Member const& m)
				:Base(b), mem(m) {

			}

			//access base class data via base()
			Base const& base() const {
				return static_cast<Base const&>(*this);
			}

			Base& base() {
				return static_cast<Base&>(*this);
			}

			//access member data via member()
			Member const& member() const {
				return this->mem;
			}

			Member& member() {
				return this->mem;
			}
	};

	template<typename CustomClass>
	class Optimizable {
		private:
			//base class type and member type
			//if you know custom class is class type and we have member is not empty
			//we can merger them
			BaseMemberPair<CustomClass, void*> info_and_storage;
	};
	
	class Apple {
		using Int = int;
	};

	void test2()
	{
		//only 8 bit, one pointer size, else might be 16 bit, because memory layout
		std::cout << "sizeof(Optimizable<Apple>): " << sizeof(Optimizable<Apple>) << '\n';
	}
}
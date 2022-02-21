#pragma once

#include <iostream>

namespace Note21dot1 {

	//a empty class requires one bit at run-time
	//because it need to compute distance between pointers/addresses
	class EmptyClass {
		
	};

	//---------Test EBCO---------
	class Empty {
		using Int = int;
	};

	class EmptyToo : public Empty {

	};

	class EmptyThree : public EmptyToo {

	};

	void test()
	{
		std::cout << "sizeof(Empty):      " << sizeof(Empty) << '\n';
		std::cout << "sizeof(EmptyToo):   " << sizeof(EmptyToo) << '\n';
		std::cout << "sizeof(EmptyThree): " << sizeof(EmptyThree) << '\n';
	}
}
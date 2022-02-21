#pragma once

namespace Note21dot1 {
	//---------EBCO---------
	//------空基类优化------
	class Empty {
		using Int = int;
	};

	class EmptyToo : public Empty {

	};

	class EmptyThree : public Empty {

	};

	void test()
	{
		std::cout << "sizeof(Empty):	  " << sizeof(Empty) << '\n';
		std::cout << "sizeof(EmptyToo):	  " << sizeof(EmptyToo) << '\n';
		std::cout << "sizeof(EmptyThree): " << sizeof(EmptyThree) << '\n';
	}
}

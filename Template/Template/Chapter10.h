#pragma once

namespace Note10dot5 {
	template<typename T, int N>
	class ArrayInClass {
		public:
			T array[N];
	};

	template<typename T>
	class Dozen {
		public:
			ArrayInClass<T, 12> contents;
	};

	void test() {
		Dozen<int> d;
	}
}

namespace Chapter10 {
	using namespace Note10dot5;
	void M_Test() {
		test();
	}
}
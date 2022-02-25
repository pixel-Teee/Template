#pragma once

#include <iostream>
#include <cstdlib>
#include <memory>
#include <vector>

namespace Note18dot2 {
	//coord
	class Coord {
		private:
			int x, y;
		public:
			Coord(int i1, int i2) : x(i1), y(i2) {

			}
			friend Coord operator-(Coord const& c1, Coord const& c2) {
				return Coord(c1.x - c2.x, c1.y - c2.y);
			}
			Coord abs() {
				return Coord(std::abs(x), std::abs(y));
			}
	};

	//static polymorphism
	template<typename GeoObj>
	void myDraw(GeoObj const& obj)
	{
		obj.draw();
	}

	class Circle {
		public:
			void draw() const;
			Coord center_of_gravity() const;
	};

	class Line {
		public:
			void draw() const;
			Coord center_of_gravity() const;
	};

	void test() {
		//std::vector<GeoObj*> coll;
		//std::vector<Line> coll;
	}
}

namespace Note18dot4 {
	//coord
	class Coord {
	private:
		int x, y;
	public:
		Coord(int i1, int i2) : x(i1), y(i2) {

		}
		friend Coord operator-(Coord const& c1, Coord const& c2) {
			return Coord(c1.x - c2.x, c1.y - c2.y);
		}
		Coord abs() {
			return Coord(std::abs(x), std::abs(y));
		}
	};

	//Line and Circle
	class Line {
		public:
			void draw() const {
				std::cout << "draw line" << std::endl;
			}
			Coord center_of_gravity() const {
				return Coord(1, 1);
			}
	};

	class Circle {
		public:
			void draw() const {
				std::cout << "draw circle" << std::endl;
			}
			Coord center_of_gravity() const {
				return Coord(1, 1);
			}
	};

	//concept requires these interface
	template<typename T>
	concept GeoObj = requires(T x) {
		{x.draw()}->std::same_as<void>;
		{x.center_of_gravity()}->std::same_as<Coord>;
	};

	template<typename T>
	requires GeoObj<T>
		void myDraw(T const& obj)
	{
		obj.draw();
	}

	template<typename T1, typename T2>
	requires GeoObj<T1> && GeoObj<T2>
		Coord distance(T1 const& x1, T2 const& x2)
	{
		Coord c = x1.center_of_gravity() - x2.center_of_gravity();
		return c.abs();
	}

	template<typename T>
	requires GeoObj<T>
		void drawElems(std::vector<T> const& elems)
	{
		for (std::size_t i = 0; i < elems.size(); ++i) {
			elems[i].draw();
		}
	}

	void test() {
		Line L;
		Circle C;
		myDraw(L);
		myDraw(C);
		//std::cout << distance(L, C) << std::endl;
	}

	//template<typename Iterator>
	//Iterator max_element(Iterator beg, Iterator end)
	//{
	//	//...
	//}

	//template<typename T>
	//class vector {
	//	public:
	//		using const_iterator = ...;

	//		const_iterator begin() const;
	//		const_iterator end() const;
	//};
}

namespace Note18dot6 {
	template<typename T>
	void printMax(T const& coll) {
		auto pos = std::max_element(coll.begin(), coll.end());

		if (pos != coll.end())
		{
			std::cout << *pos << '\n';
		}
		else
		{
			std::cout << "empty" << '\n';
		}
	}
	void test2() {
		std::vector<int> v{1, 2, 3, 4, 5};

		printMax(v);
	}
}

namespace Chapter18 {
	using namespace Note18dot4;
	using namespace Note18dot6;
	void M_Test() {
		test();
		test2();
	}
}

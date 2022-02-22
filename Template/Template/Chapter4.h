#pragma once

#include <iostream>
#include <unordered_set>

namespace Note4dot1dot1{
	
	void print() {

	}

	//template parameter pack
	template<typename T, typename... Types>
	//function parameter pack
	//type expand
	void print(T firstArg, Types... args) {
		std::cout << firstArg << '\n';

		//pack expand
		print(args...);
	}

	template<typename T>
	void printa(T arg) {
		std::cout << arg << '\n';
	}

	template<typename T, typename... Types>
	void printa(T firstArg, Types... args) {
		printa(firstArg);
		print(args...);
	}

	//error
	/*
	template<typename T, typename... Types>
	void print(T firstArg, Types... args)
	{
		std::cout << firstArg << '\n';
		if (sizeof...(args) > 0) {
			print(args...);
		}
	}
	*/

	void test()
	{
		printa("a", "b", "c", "d");
	}
}

//fold expressions

namespace Note4dot2 {
	template<typename... T>
	auto foldSum(T... s) {
		return (... + s);//((s1 + s2) + s3)
	}

	void test2() {
		int sum = foldSum(1, 2, 3, 4, 5);
		
		std::cout << sum << std::endl;
	}

	//traverse a path in a binary tree using operator ->*
	struct Node {
		int value;
		Node* left;
		Node* right;
		Node(int i = 0) : value(i), left(nullptr), right(nullptr) {

		}
	};

	auto left = &Node::left;
	auto right = &Node::right;

	//traverse tree, using fold expression
	template<typename T, typename... TP>
	Node* traverse(T np, TP... paths) {
		return (np->*...->*paths);
		//np->*paths1->*paths2
	}

	template<typename T>
	class AddSpace
	{
		private:
			T const& ref;
		public:
			AddSpace(T const& r) : ref(r) {

			}
			friend std::ostream& operator<<(std::ostream& os, AddSpace<T> s) {
				return os << s.ref << ' ';//output passed argument and a space
			}
	};

	//use fold expressions on print
	template<typename... Types>
	void print(Types const&... args)
	{
		(std::cout << ... << AddSpace(args)) << '\n';
	}

	void test3() {
		Node* root = new Node{0};
		root->left = new Node{1};
		root->left->right = new Node{2};

		Node* node = traverse(root, left, right);

		print('A', 'C', 'M');
	}
}

namespace Note4dot4dot1 {
	using namespace Note4dot2;
	template<typename... T>
	void printDoubled(T const&... args) {
		print(args + args...);
	}

	//C++17
	template<typename T1, typename... TN>
	constexpr bool isHomogeneous(T1, TN...)
	{
		return (std::is_same<T1, TN>::value && ...);
	}

	void test4()
	{
		printDoubled(7.5, std::string("Hello"));
	}
}

namespace Note4dot4dot2 {
	using namespace Note4dot2;
	//container type and variadic index
	template<typename C, typename... Idx>
	void printElems(C const& coll, Idx... idx)
	{
		print(coll[idx]...);
	}

	//declare nontype parameter
	template<std::size_t... Idx, typename C>
	void printIdx(C const& coll)
	{
		print(coll[Idx]...);
	}

	//type for arbitrary number of indices
	template<std::size_t...>
	struct Indices {

	};

	template<typename T, std::size_t... Idx>
	void printByIdx(T t, Indices<Idx...>) {
		print(std::get<Idx>(t)...);
	}

	void test5() {

		std::vector<int> v{1, 2, 3, 4};
		printElems(v, 2, 0, 3);

		printIdx<2, 0, 3>(v);

		std::array<std::string, 5> arr = {"Hello", "Non", "QAQ", "I Love You"};
		printByIdx(arr, Indices<3, 2, 1>());
	}
}

namespace Note4dot4dot5 {
	class Customer {
		private:
			std::string name;
		public:
			Customer(std::string const& n) : name(n) {}
			std::string getName() const { return name; }
	};

	struct CustomerEq {
		bool operator()(Customer const& c1, Customer const& c2) const {
			return c1.getName() == c2.getName();
		}
	};

	//define hash, return size_t
	struct CustomerHash {
		std::size_t operator()(Customer const& c) const {
			return std::hash<std::string>()(c.getName());
		}
	};

	//define class that combines operator() for variadic base classes:
	template<typename... Bases>
	struct Overloader : Bases...
	{
		using Bases::operator()...;//C++17
	};

	void test6()
	{
		//combine hasher and equality for customers in one type
		using CustomerOP = Overloader<CustomerHash, CustomerEq>;

		std::unordered_set<Customer, CustomerHash, CustomerEq> coll1;
		std::unordered_set<Customer, CustomerOP, CustomerOP> coll1;
	}
}

namespace Chapter4 {
	using namespace Note4dot1dot1;
	using namespace Note4dot2;
	using namespace Note4dot4dot1;
	using namespace Note4dot4dot2;
	void M_Test() {
		test();
		test2();
		test3();
		test4();
		test5();
	}
}

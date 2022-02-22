#pragma once

#include <type_traits>

namespace Note25dot1dot1 {
	template<typename... Types>
	class Tuple;

	//recursive case:
	template<typename Head, typename... Tail>
	class Tuple<Head, Tail...>
	{
	private:
		Head head;
		Tuple<Tail...> tail;

	public:
		//constructors
		Tuple() {

		}
		Tuple(Head const& head, Tuple<Tail...> const& tail)
			:head(head), tail(tail) {

		}

		//from independent value constructor
		Tuple(Head const& head, Tail const&... tail)
			:head(head), tail(tail...) {

		}

		//wish to use move constructor
		template<typename VHead, typename... VTail,
				typename = std::enable_if_t<sizeof...(VTail) == sizeof...(Tail)>>
		Tuple(VHead&& vhead, VTail&&... vtail)
			: head(std::forward<VHead>(vhead)),
			tail(std::forward<VTail>(vtail)...) {

		}

		//constructor a tuple from another tuple
		template<typename VHead, typename... VTail,
				typename = std::enable_if_t<sizeof...(VTail) == sizeof...(Tail)>>
		Tuple(Tuple<VHead, VTail...> const& other)
		:head(other.getHead()), tail(other.getTail()){}

		Head& getHead(){ return head; }
		Head const& getHead() const { return head;}
		Tuple<Tail...>& getTail(){ return tail; }
		Tuple<Tail...> const& getTail() const { return tail;}
	};

	//basis case:
	template<>
	class Tuple<> {
		//no storage required
	};

	//extra the required element
	//recursive case:
	template<unsigned N>
	struct TupleGet {
		template<typename Head, typename... Tail>
		static auto apply(Tuple<Head, Tail...> const& t) {
			return TupleGet<N-1>::apply(t.getTail());
		}
	};

	//basis case:
	template<>
	struct TupleGet<0> {
		template<typename Head, typename... Tail>
		static Head const& apply(Tuple<Head, Tail...> const& t) {
			return t.getHead();
		}
	};

	template<unsigned N, typename... Types>
	auto get(Tuple<Types...> const& t) {
		return TupleGet<N>::apply(t);
	}

	//decay, decay array and literal to raw pointer
	template<typename... Types>
	auto makeTuple(Types&&... elems)
	{
		return Tuple<std::decay_t<Types>...>(std::forward<Types>(elems)...);
	}

	void test()
	{
		Tuple<int, double, std::string> t(17, 3.14, "I love you!");
		double const& a = get<1>(t);
		std::cout << a << std::endl;

		std::string str = "AQA";
		Tuple<int, std::string> t2(3, std::move(str));

		Tuple<int, double, std::string> t3(t);

		auto t4 = makeTuple(1, 2, "I love you!");

		int b = get<1>(t4);
		std::cout << b << std::endl;
	}
}

//tuple comparsion
namespace Note25dot2 {
	using namespace Note25dot1dot1;

	bool operator==(Tuple<> const&, Tuple<> const&)
	{
		//empty
		return true;
	}

	//recursive case:
	template<typename Head1, typename... Tail1,
			typename Head2, typename... Tail2,
			typename = std::enable_if<sizeof...(Tail1) == sizeof...(Tail2)>>
		bool operator==(Tuple<Head1, Tail1...> const& lhs,
			Tuple<Head2, Tail2...> const& rhs)
	{
		return lhs.getHead() == rhs.getHead()
				&& lhs.getTail() == rhs.getTail();
	}

	void test()
	{
		Tuple<std::string, int, int> t1("QAQ", 1, 1);
		Tuple<std::string, int, int> t2("I Love You", 2, 2);

		auto&& t3 = makeTuple(std::string("QAQ"), 1, 1);

		std::cout << (t1 == t2) << std::endl;
		std::cout << (t1 == t3) << std::endl;
	}

	//output
	void printTuple(std::ostream& strm, Tuple<> const&, bool isFirst = true)
	{
		strm << (isFirst ? '(' : ')');
	}

	template<typename Head, typename...Tail>
	void printTuple(std::ostream& strm, Tuple<Head, Tail...> const& t,
		bool isFirst = true)
	{
		strm << (isFirst ? "(" : ", ");
		strm << t.getHead();
		printTuple(strm, t.getTail(), false);
	}

	template<typename... Types>
	std::ostream& operator<<(std::ostream& strm, Tuple<Types...> const& t)
	{
		printTuple(strm, t);
		return strm;
	}

	void testOutput()
	{
		Tuple<int, double, std::string> a(3, 3.14, "I love you");
		std::cout << a << std::endl;
	}
}

//tuple algorithms
namespace Note25dot3 {
	using namespace Note25dot2;

	//use some partial specializations, we can turn Tuple into a full-featured typelist
}

namespace Note25dot5dot1 {
	/*
	template<typename Head, typename... Tail>
	class Tuple<Head, Tail...> : private Tuple<Tail...>
	{
		private:
			Head head;
		public:
			Head& getHead() { return head; }
			Head const& getHead(){ return head; }
			Tuple<Tail...>& getTail(){ return *this; }
			Tuple<Tail...> const& getTail(){ return *this; }
	};
	*/
}
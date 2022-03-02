#pragma once

#include <iostream>
#include <string>
#include <type_traits>

#include "Chapter24.h"

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

	//------extra the required element

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
namespace Note25dot2dot1 {
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

	void test2()
	{
		Tuple<std::string, int, int> t1("QAQ", 1, 1);
		Tuple<std::string, int, int> t2("I Love You", 2, 2);

		auto&& t3 = makeTuple(std::string("QAQ"), 1, 1);

		std::cout << (t1 == t2) << std::endl;
		std::cout << (t1 == t3) << std::endl;
	}

	//------Output------

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
	using namespace Note25dot2dot1;

	//use some partial specializations, we can turn Tuple into a full-featured typelist

	template<typename List>
	class IsEmpty
	{
	public:
		static constexpr bool value = false;
	};

	template<>
	struct IsEmpty<Tuple<>> {
		static constexpr bool value = true;
	};

	template<typename List>
	class FrontT;

	//extra from element:
	template<typename Head, typename... Tail>
	class FrontT<Tuple<Head, Tail...>> {
		public:
			using Type = Head;
	};

	template<typename List>
	using Front = typename FrontT<List>::Type;

	//removing one element:
	template<typename List>
	class PopFrontT;

	template<typename Head, typename... Tail>
	class PopFrontT<Tuple<Head, Tail...>> {
	public:
		using Type = Tuple<Tail...>;
	};

	template<typename List>
	using PopFront = typename PopFrontT<List>::Type;

	template<typename List, typename NewElement>
	class PushFrontT;

	//add element to the front:
	template<typename... Types, typename Element>
	class PushFrontT<Tuple<Types...>, Element> {
		public:
			using Type = Tuple<Element, Types...>;
	};

	template<typename List, typename Element>
	class PushBackT;

	//add element to the back:
	template<typename... Types, typename Element>
	class PushBackT<Tuple<Types...>, Element> {
		public:
			using Type = Tuple<Types..., Element>;
	};

	void test3() {
		Tuple<int, double, std::string> t1(17, 3.14, "Hello World!");

		using T2 = PopFrontT<PushBackT<decltype(t1), bool>::Type>::Type;

		T2 t2(get<1>(t1), get<2>(t1), true);

		std::cout << t2 << std::endl;
	}

	//adding to and removing from a tuple
	template<typename... Types, typename V>
	PushFrontT<Tuple<Types...>, V>
		pushFront(Tuple<Types...> const& tuple, V const& value)
	{
		return PushFrontT<Tuple<Types...>, V>(value, tuple);
	}

	//basis case
	template<typename V>
	Tuple<V> pushBack(Tuple<> const&, V const& value)
	{
		return Tuple<V>(value);
	}

	//recursive case
	template<typename Head, typename... Tail, typename V>
	Tuple<Head, Tail..., V>
	pushBack(Tuple<Head, Tail...> const& tuple, V const& value)
	{
		return Tuple<Head, Tail..., V>(tuple.getHead(),
									pushBack(tuple.getTail(), value));
	}


	template<typename... Types>
	PopFrontT<Tuple<Types...>>::Type
	popFront(Tuple<Types...> const& tuple)
	{
		return tuple.getTail();
	}

	template<typename List, bool Empty = IsEmpty<List>::value>
	class ReverseT;

	template<typename List>
	using Reverse = typename ReverseT<List>::Type;

	template<typename List>
	class ReverseT<List, true>
	{
	public:
		using Type = List;
	};

	//recursive case:
	template<typename List>
	class ReverseT<List, false>
		: public PushBackT<Reverse<PopFront<List>>, Front<List>>
	{

	};

	//Tuple<> reverse(Tuple<> const& t)
	//{
	//	return t;
	//}

	////recursive case
	//template<typename Head, typename... Tail>
	//Reverse<Tuple<Head, Tail...>> reverse(Tuple<Head, Tail...> const& t)
	//{
	//	return pushBack(reverse(t.getTail()), t.getHead());
	//}

	template<int N>
	struct CopyCounter
	{
		inline static unsigned numCopies = 0;
		inline static unsigned moveCopies = 0;
		CopyCounter() {

		}
		CopyCounter(CopyCounter const&) {
			++numCopies;
		}
		CopyCounter(CopyCounter&&) {
			++moveCopies;
		}
	};

	//------Index Lists------

	template<typename T, T... Values>
	struct Valuelist {

	};

	template<typename T, T Value>
	struct CTValue
	{
		static constexpr T value = Value;
	};

	template<typename T, T... Values>
	struct IsEmpty<Valuelist<T, Values...>> {
		static constexpr bool value = sizeof...(Values) == 0;
	};

	template<typename Valuelist>
	struct FrontT;

	template<typename T, T Head, T... Tail>
	struct FrontT<Valuelist<T, Head, Tail...>> {
		using Type = CTValue<T, Head>;
		static constexpr T value = Head;
	};

	template<typename Valuelist, typename CTValue>
	struct PushFrontT;

	template<typename T, T... Values, T New>
	struct PushFrontT<Valuelist<T, Values...>, CTValue<T, New>> {
		using Type = Valuelist<T, New, Values...>;
	};

	template<typename ValueList>
	struct PopFrontT;

	template<typename T, T Head, T... Tail>
	struct PopFrontT<Valuelist<T, Head, Tail...>> {
		using Type = Valuelist<T, Tail...>;
	};

	template<typename ValueList>
	using PopFront = typename PopFrontT<ValueList>::Type;

	template<typename T, T... Values, T New>
	struct PushBackT<Valuelist<T, Values...>, CTValue<T, New>> {
		using Type = Valuelist<T, Values..., New>;
	};

	//recursive case
	template<unsigned N, typename Result = Valuelist<unsigned>>
	struct MakeIndexListT
		: MakeIndexListT<N - 1, typename PushFrontT<Result, CTValue<unsigned, N - 1>>::Type>
	{

	};

	//basis case
	template<typename Result>
	struct MakeIndexListT<0, Result>
	{
		using Type = Result;
	};

	template<unsigned N>
	using MakeIndexList = typename MakeIndexListT<N>::Type;

	template<typename... Elements, unsigned... Indices>
	auto reverseImpl(Tuple<Elements...> const& t, Valuelist<unsigned, Indices...>)
	{
		return makeTuple(get<Indices>(t)...);
	}

	template<typename... Elements>
	auto reverse(Tuple<Elements...> const& t)
	{
		return reverseImpl(t, Reverse<MakeIndexList<sizeof...(Elements)>>());
	}

	void copycountertest() {
		Tuple<CopyCounter<0>, CopyCounter<1>, CopyCounter<2>,
				CopyCounter<3>, CopyCounter<4>> copies;
		//auto reversed = reverse(copies);
		/*auto reversed = makeTuple(get<4>(copies), get<3>(copies),
							get<2>(copies), get<1>(copies), 
							get<0>(copies));*/

		using MyIndexList = Reverse<MakeIndexList<5>>;

		reverse(copies);

		std::cout << "0: " << CopyCounter<0>::numCopies << " copies\n";
		std::cout << "1: " << CopyCounter<1>::numCopies << " copies\n";
		std::cout << "2: " << CopyCounter<2>::numCopies << " copies\n";
		std::cout << "3: " << CopyCounter<3>::numCopies << " copies\n";
		std::cout << "4: " << CopyCounter<4>::numCopies << " copies\n";
	}

	void test4() {
		Tuple<int, double, std::string> t1(17, 3.14, "Hello World");
		auto t2 = popFront(pushBack(t1, true));

		std::cout << std::boolalpha << t2 << '\n';

		Tuple<int, double, std::string> t3(17, 3.14, "I Love You!");
		reverse(t3);

		std::cout << t3 << '\n';

		copycountertest();


		
	}

	//yield T when using member Type:
	template<typename T>
	struct IdentityT {
		using Type = T;
	};

	//if cond is true, return second type, else return third type
	template<bool COND, typename TrueType, typename FalseType>
	struct IfThenElseT {
		using Type = TrueType;
	};

	//partial specialization: false yields third argument
	template<typename TrueType, typename FalseType>
	struct IfThenElseT<false, TrueType, FalseType>
	{
		using Type = FalseType;
	};

	template<bool COND, typename TrueType, typename FalseType>
	using IfThenElse = typename IfThenElseT<COND, TrueType, FalseType>::Type;

	//shuffle and select
	template<typename... Elements, unsigned... Indices>
	auto select(Tuple<Elements...> const& t,
		Valuelist<unsigned, Indices...>)
	{
		return makeTuple(get<Indices>(t)...);
	}

	template<unsigned I, unsigned N, typename IndexList = Valuelist<unsigned>>
	class ReplicatedIndexListT;

	template<unsigned I, unsigned N, unsigned... Indices>
	class ReplicatedIndexListT<I, N, Valuelist<unsigned, Indices...>>
		: public ReplicatedIndexListT<I, N - 1, Valuelist<unsigned, Indices..., I>> {

	};

	template<unsigned I, unsigned... Indices>
	class ReplicatedIndexListT<I, 0, Valuelist<unsigned, Indices...>> {
		public:
			using Type = Valuelist<unsigned, Indices...>;
	};

	template<unsigned I, unsigned N>
	using ReplicatedIndexList = typename ReplicatedIndexListT<I, N>::Type;

	template<unsigned I, unsigned N, typename... Elements>
	auto splat(Tuple<Elements...> const& t)
	{
		return select(t, ReplicatedIndexList<I, N>());
	}

	template<typename List, typename Element,
		template<typename T, typename U> class Compare,
		bool = IsEmpty<List>::value>
		class InsertSortedT;

	//recursive case:
	template<typename List, typename Element,
		template<typename T, typename U> class Compare>
	class InsertSortedT<List, Element, Compare, false>
	{
		using NewTail =
			typename IfThenElse<Compare<Element, Front<List>>::value,
			IdentityT<List>,
			InsertSortedT<PopFront<List>, Element, Compare>>::Type;
		using NewHead = IfThenElse<Compare<Element, Front<List>>::value,
			Element,
			Front<List>>;
	public:
		using Type = typename PushFrontT<NewTail, NewHead>::Type;
	};

	//basis case:
	template<typename List, typename Element,
		template<typename T, typename U> class Compare>
	class InsertSortedT<List, Element, Compare, true>
		: public PushFrontT<List, Element>
	{

	};

	//------InsertSort------

	template<typename List, typename Element,
		template<typename T, typename U> class Compare>
	using InsertSorted = typename InsertSortedT<List, Element, Compare>::Type;

	template<typename List,
		template<typename T, typename U> class Compare,
		bool = IsEmpty<List>::value>
		class InsertionSortT;

	template<typename List,
		template<typename T, typename U> class Compare>
	using InsertionSort = typename InsertionSortT<List, Compare>::Type;

	//recursive case(insert first element into sorted list):
	template<typename List,
		template<typename T, typename U> class Compare>
	class InsertionSortT<List, Compare, false>
		: public InsertSortedT<InsertionSort<PopFront<List>, Compare>,
		Front<List>, Compare>
	{

	};

	//basis case
	template<typename List,
		template<typename T, typename U> class Compare>
	class InsertionSortT<List, Compare, true>
	{
	public:
		using Type = List;
	};

	//NthElementT
	template<typename List, unsigned N>
	class NthElementT : public NthElementT <PopFront<List>, N - 1>
	{

	};

	//basis case:
	template<typename List>
	class NthElementT<List, 0> : public FrontT<List>
	{

	};

	template<typename List, unsigned N>
	using NthElement = typename NthElementT<List, N>::Type;

	//metafunction wrapper that compares the elements in a tuple
	template<typename List, template<typename T, typename U> class F>
	class MetafunOfNthElementT {
		public:
			template<typename T, typename U> class Apply;

			template<unsigned N, unsigned M>
			class Apply<CTValue<unsigned, M>, CTValue<unsigned, N>>
			: public F<NthElement<List, M>, NthElement<List, N>> {};
	};

	//sort a tuple based on comparing the element types
	template<template<typename T, typename U> class Compare,
			typename... Elements>
		auto sort(Tuple<Elements...> const& t)
	{
		return select(t,
			InsertionSort<MakeIndexList<sizeof...(Elements)>, 
						MetafunOfNthElementT<Tuple<Elements...>, Compare>::template Apply>());
	}

	template<typename F, typename... Elements, unsigned... Indices>
	auto UnPackImpl(F f, Tuple<Elements...> const& t, Valuelist<unsigned, Indices...>)->decltype(f(get<Indices>(t)...))
	{
		return f(get<Indices>(t)...);
	}

	template<typename F, typename... Elements, unsigned N = sizeof...(Elements)>
	auto UnPack(F f, Tuple<Elements...> const& t)->decltype(UnPackImpl(f, t, MakeIndexList<N>()))
	{
		return UnPackImpl(f, t, MakeIndexList<N>());
	}

	template<typename T>
	void print(T&& arg)
	{
		std::cout << arg << '\n';
	}

	template<typename T, typename... Types>
	void print(T&& firstArg, Types&&... args)
	{
		print(firstArg);
		print(args...);
	}

	template<typename... Args>
	void Output(Args&&... args)
	{
		print(args...);
	}

	void test5() {
		Tuple<std::string, char const*, int, char> t("PI", "is roughly", 3, '\n');

		//UnPack(print, t);

		UnPack(Output<std::string, char const*, int, char>, t);
	}
}

namespace Chapter25 {
	using namespace Note25dot3;

	void M_Test() {
		test3();
		test4();
		test5();
	}
}

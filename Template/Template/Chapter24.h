#pragma once

#include <type_traits>

#include "Chapter19.h"

namespace Note24dot1 {
	template<typename... Elements>
	class Typelist
	{

	};

	template<typename List>
	class FrontT;

	template<typename Head, typename... Tail>
	class FrontT<Typelist<Head, Tail...>>
	{
		public:
			using Type = Head;
	};

	template<typename List>
	using Front = typename FrontT<List>::Type;

	template<typename List>
	class PopFrontT;

	template<typename Head, typename... Tail>
	class PopFrontT<Typelist<Head, Tail...>> {
		public:
			using Type = Typelist<Tail...>;
	};

	template<typename List>
	using PopFront = typename PopFrontT<List>::Type;

	template<typename List, typename NewElement>
	class PushFrontT;

	template<typename... Elements, typename NewElement>
	class PushFrontT<Typelist<Elements...>, NewElement> {
		public:
			using Type = Typelist<NewElement, Elements...>;
	};

	template<typename List, typename NewElement>
	using PushFront = typename PushFrontT<List, NewElement>::Type;
}

namespace Note24dot2 {
	using namespace Note24dot1;
	//recursive case:
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
}

namespace Note24dot2dot2 {
	using namespace Note24dot2;
	using namespace Note19dot7;

	template<typename List>
	class IsEmpty
	{
	public:
		static constexpr bool value = false;
	};

	template<>
	class IsEmpty<Typelist<>>
	{
	public:
		static constexpr bool value = true;
	};

	template<typename List, bool Empty = IsEmpty<List>::value>
	class LargestTypeT;

	//recursive case:
	template<typename List>
	class LargestTypeT<List, false>
	{
		private:
			using Contender = Front<List>;
			using Best = typename LargestTypeT<PopFront<List>>::Type;
			//using First = Front<List>;
			//using Rest = typename LargestTypeT<PopFront<List>>::Type;
		public:
			//using Type = IfThenElse<(sizeof(First) >= sizeof(Rest)), First, Rest>;
			using Type = IfThenElse<(sizeof(Contender) >= sizeof(Best)), Contender, Best>;
	};

	//basis case:
	template<typename List>
	class LargestTypeT<List, true>
	{
		public:
			using Type = char;//minimum type
	};

	template<typename List>
	using LargestType = typename LargestTypeT<List>::Type;
}

namespace Note24dot2dot3 {
	using namespace Note24dot2dot2;
	/*template<typename List, typename NewElement>
	class PushBackT;

	template<typename... Elements, typename NewElement>
	class PushBackT<Typelist<Elements...>, NewElement>
	{
		public:
			using Type = Typelist<Elements..., NewElement>;
	};

	template<typename List, typename NewElement>
	using PushBack = typename PushBackT<List, NewElement>::Type;*/

	//generic push back
	template<typename List, typename NewElement, bool = IsEmpty<List>::value>
	class PushBackRecT;

	//recursive case:
	template<typename List, typename NewElement>
	class PushBackRecT<List, NewElement, false>
	{
		using Head = Front<List>;
		using Tail = PopFront<List>;
		using NewTail = typename PushBackRecT<Tail, NewElement>::Type;

		public:
			using Type = PushFront<Head, NewTail>;
	};

	//basis case:
	template<typename List, typename NewElement>
	class PushBackRecT<List, NewElement, true>
	{
		public:
			using Type = PushFront<List, NewElement>;
	};

	//generic push-back operation:
	template<typename List, typename NewElement>
	class PushBackT : public PushBackRecT<List, NewElement>{};

	template<typename List, typename NewElement>
	using PushBack = typename PushBackT<List, NewElement>::Type;
}

namespace Note24dot2dot4 {
	using namespace Note24dot2dot3;

	template<typename List, bool Empty = IsEmpty<List>::value>
	class ReverseT;

	template<typename List>
	using Reverse = typename ReverseT<List>::Type;

	//recursive case:
	template<typename List>
	class ReverseT<List, false>
	:public PushBackT<Reverse<PopFront<List>>, Front<List>>{};

	//basis case:
	template<typename List>
	class ReverseT<List, true>
	{
		public:
			using Type = List;
	};

	template<typename List>
	class PopBackT
	{
		public:
			using Type = Reverse<PopFront<Reverse<List>>>;
	};

	template<typename List>
	using PopBack = typename PopBackT<List>::Type;

	//------Transforming a Typelist
	template<typename T>
	struct AddConstT
	{
		using Type = T const;
	};

	template<typename T>
	using AddConst = typename AddConstT<T>::Type;

	template<typename List, template<typename T> class MetaFun,
			bool Empty = IsEmpty<List>::value>
	class TransformT;

	//recursive case:
	template<typename List, template<typename T> class MetaFun>
	class TransformT<List, MetaFun, false>
		: public PushFrontT<typename TransformT<PopFront<List>, MetaFun>::Type,
			typename MetaFun<Front<List>>::Type>
	{	
		
	};

	template<typename List, template<typename T> class MetaFun>
	class TransformT<List, MetaFun, true>
	{
		public:
			using Type = List;
	};

	template<typename List, template<typename T> class MetaFun>
	using Transform = typename TransformT<List, MetaFun>::Type;

	template<typename List,
			template<typename X, typename Y> class F,
			typename I,
			bool = IsEmpty<List>::value>
	class AccumulateT;

	//recursive case:
	template<typename List,
			template<typename X, typename Y> class F,
			typename I>
	class AccumulateT<List, F, I, false>
		:public AccumulateT<PopFront<List>, F,
		typename F<I, Front<List>>::Type>
	{

	};

	//basis case:
	template<typename List,
			template<typename X, typename Y> class F,
			typename I>
	class AccumulateT<List, F, I, true>
	{
		public:
			using Type = I;
	};

	template<typename List,
			template<typename X, typename Y> class F,
			typename I>
	using Accumulate = typename AccumulateT<List, F, I>::Type;

	//LargeType
	template<typename T, typename U>
	class LargerTypeT
		: public IfThenElseT<sizeof(T) >= sizeof(U), T, U>
	{

	};

	template<typename Typelist, bool = IsEmpty<Typelist>::value>
	class LargestTypeAccT;

	template<typename Typelist>
	class LargestTypeAccT<Typelist, false>
	: public AccumulateT<PopFront<Typelist>, LargerTypeT,
		Front<Typelist>>
	{

	};

	template<typename Typelist>
	class LargestTypeAccT<Typelist, true>
	{

	};

	template<typename Typelist>
	using LargestTypeAcc = typename LargestTypeAccT<Typelist>::Type;
}	

namespace Note24dot2dot6 {
	//using namespace Note19dot7;
	//using namespace Note24dot2dot2;
	using namespace Note24dot2dot4;
	template<typename T, typename U>
	class LargerTypeT
		: public IfThenElse<sizeof(T) >= sizeof(U), T, U>
	{

	};

	template<typename Typelist, bool = IsEmpty<Typelist>::value>
	class LargestTypeAccT;

	template<typename Typelist>
	class LargestTypeAccT<Typelist, false>
	: public AccumulateT<PopFront<Typelist>, LargerTypeT,
		FrontT<Typelist>>
	{

	};

	template<typename Typelist>
	class LargestTypeAccT<Typelist, true>
	{

	};

	template<typename Typelist>
	using LargestTypeAcc = typename LargestTypeAccT<Typelist>::Type;
}

namespace Note24dot2dot7 {
	using namespace Note24dot2dot6;

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
			using Type = PushFront<NewTail, NewHead>;
	};

	//basis case:
	template<typename List, typename Element,
			template<typename T, typename U> class Compare>
	class InsertSortedT<List, Element, Compare, true>
		: public PushFrontT<List, Element>
	{

	};

	//------------

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

	template<typename T, typename U>
	struct SmallerThanT {
		static constexpr bool value = sizeof(T) < sizeof(U);
	};

	void test() {
		using Types = Typelist<int, char, short, double>;

		//second paramter is metafunction
		using ST = InsertionSort<Types, SmallerThanT>;

		std::cout << std::is_same<ST, Typelist<char, short, int, double>>::value
		<< '\n';
	}
}

namespace Note24dot3 {
	using namespace Note24dot2dot2;
	using namespace Note24dot2dot4;

	template<typename T, T Value>
	struct CTValue
	{
		static constexpr T value = Value;
	};

	template<typename T, typename U>
	struct MultiplyT;

	template<typename T, T Value1, T Value2>
	struct MultiplyT<CTValue<T, Value1>, CTValue<T, Value2>> {
		public:
			using Type = CTValue<T, Value1 * Value2>;
	};

	template<typename T, typename U>
	using Multiply = typename MultiplyT<T, U>::Type;

	void test2() {
		using Primes = Typelist<CTValue<int, 2>, CTValue<int, 3>,
						CTValue<int, 5>, CTValue<int, 7>,
						CTValue<int, 11>>;

		std::cout << Accumulate<Primes, MultiplyT, CTValue<int, 1>>::value;
	}

	template<typename T, T... Values>
	struct Valuelist {

	};

	/*template<typename T, T... Values>
	struct IsEmpty<Valuelist<T, Values...>> {
		static constexpr bool value = sizeof...(Values) == 0;
	};

	template<typename T, T Head, T... Tail>
	struct FrontT<Valuelist<T, Head, Tail...>> {
		using Type = CTValue<T, Head>;
		static constexpr T value = Head;
	};
	*/
	template<typename ValueList>
	struct PopFrontT;

	template<typename T, T Head, T... Tail>
	struct PopFrontT<Valuelist<T, Head, Tail...>> {
		using Type = Valuelist<T, Tail...>;
	};

	template<typename Valuelist, typename CTValue>
	struct PushFrontT;

	template<typename T, T... Values, T New>
	struct PushFrontT<Valuelist<T, Values...>, CTValue<T, New>> {
		using Type = Valuelist<T, New, Values...>;
	};

	/*
	template<typename T, T... Values, T New>
	struct PushBackT<Valuelist<T, Values...>, CTValue<T, New>> {
		using Type = Valuelist<T, Values..., New>;
	};*/
}

namespace Note24dot4 {
	using namespace Note24dot2dot4;
	using namespace Note24dot3;
	template<typename... Elements, template<typename T> class MetaFun>
	class TransformT<Typelist<Elements...>, MetaFun, false>
	{
		public:
			using Type = Typelist<typename MetaFun<Elements>::Type...>;
	};

	template<typename Types, typename Indices>
	class SelectT;

	template<typename Types, unsigned... Indices>
	class SelectT<Types, Valuelist<unsigned, Indices...>>
	{
		public:
			using Type = Typelist<NthElement<Types, Indices>...>;
	};

	template<typename Types, typename Indices>
	using Select = typename SelectT<Types, Indices>::Type;
}

namespace Note24dot5 {
	//using namespace Note24dot2dot7;
	class Nil{};//empty class

	template<typename HeadT, typename TailT = Nil>
	class Cons {
		public:
			using Head = HeadT;
			using Tail = TailT;
	};

	using TwoShort = Cons<short, Cons<unsigned short>>;

	//extract the first element
	template<typename List>
	class FrontT {
		public:
			using Type = typename List::Head;
	};

	template<typename List>
	using Front = typename FrontT<List>::Type;

	template<typename List, typename Element>
	class PushFrontT {
		public:
			using Type = Cons<Element, List>;
	};

	template<typename List, typename Element>
	using PushFront = typename PushFrontT<List, Element>::Type;

	//removing first element
	template<typename List>
	class PopFrontT {
		public:
			using Type = typename List::Tail;
	};

	template<typename List>
	using PopFront = typename PopFrontT<List>::Type;

	template<typename List>
	struct IsEmpty
	{
		static constexpr bool value = false;
	};

	template<>
	struct IsEmpty<Nil> {
		static constexpr bool value = true;
	};

	template<typename T, typename U>
	struct SmallerThanT {
		static constexpr bool value = sizeof(T) < sizeof(U);
	};

	void test4() {
		/*using ConsList = Cons<int, Cons<char, Cons<short, Cons<double>>>>;

		using SortedTypes = Note24dot2dot7::InsertionSort<ConsList, SmallerThanT>;
		using Expected = Cons<char, Cons<short, Cons<int, Cons<double>>>>;

		std::cout << std::is_same<SortedTypes, Expected>::value << '\n';*/
	}
}

namespace Chapter24 {
	using namespace Note24dot2dot7;
	using namespace Note24dot3;
	using namespace Note24dot5;
	void M_Test() {
		test();
		test2();
		//test3();
		test4();
	}
}

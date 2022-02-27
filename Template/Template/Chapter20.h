#pragma once

#include <type_traits>
#include <iterator>
#include <map>

#include "Chapter19.h"

namespace Note20dot1 {
	using namespace Note19dot5;
	////algorithm specialization
	//template<typename T>
	//void swap(T& x, T& y)
	//{
	//	T tmp(x);
	//	x = y;
	//	y = tmp;
	//}

	//template<typename T>
	//void swap(Array<T>& x, Array<T>& y)
	//{
	//	swap(x.ptr, y.ptr);
	//	swap(x.len, y.len);
	//}

	//template<typename InputIterator, typename Distance>
	//void advanceIter(InputIterator& x, Distance n)
	//{
	//	while (n > 0) //linerar time
	//	{
	//		++x;
	//		--n;
	//	}
	//}

	//template<typename RandomAccessIterator, typename Distance>
	//void advanceIter(RandomAccessIterator& x, Distance n)
	//{
	//	x += n;//constant time
	//}

	//template<typename Iterator, typename Distance>
	//void advanceIterImpl(Iterator& x, Distance n, std::input_iterator_tag)
	//{
	//	while (n > 0)
	//	{
	//		++x;
	//		--n;
	//	}
	//}

	//template<typename Iterator, typename Distance>
	//void advanceIterImpl(Iterator& x, Distance n, std::random_access_iterator_tag)
	//{
	//	x += n;//constant time
	//}

	//template<typename Iterator, typename Distance>
	//void advanceIter(Iterator& x, Distance n)
	//{
	//	advanceIterImpl(x, n, 
	//	typename std::iterator_traits<Iterator>::iterator_category());
	//}

	/*struct input_iterator_tag{};
	struct output_iterator_tag{};
	struct forward_iterator_tag : public input_iterator_tag{};
	struct bidirectional_iterator_tag : public forward_iterator_tag{};
	struct random_access_iterator_tag : public bidirectional_iterator_tag{};*/

	template<bool, typename T = void>
	struct EnableIfT {

	};

	template<typename T>
	struct EnableIfT<true, T> {
		using Type = T;
	};

	template<bool Cond, typename T = void>
	using EnableIf = typename EnableIfT<Cond, T>::Type;

	template<typename Iterator>
	constexpr bool IsRandomAccessIteator = 
	IsConvertibleT<
		typename std::iterator_traits<Iterator>::iterator_category,
		std::random_access_iterator_tag>::Type;

	/*template<typename Iterator, typename Distance>
	EnableIf<IsRandomAccessIteator<Iterator>>
		advanceIter(Iterator& x, Distance n)
	{
		x += n;
	}*/

	template<typename Iterator, typename Distance>
	EnableIf<!IsRandomAccessIteator<Iterator>>
		advanceIter(Iterator& x, Distance n)
	{
		while (n > 0)
		{
			++x;
			--n;
		}
	}

	//implementation for random access iterators
	template<typename Iterator, typename Distance>
	EnableIf<IsRandomAccessIteator<Iterator>>
	advanceIter(Iterator& x, Distance n) {
		x += n;//constant time
	}

	template<typename Iterator>
	constexpr bool IsBidirectionalIterator = 
	IsConvertibleT<typename std::iterator_traits<Iterator>::iterator_category,
	std::bidirectional_iterator_tag>;

	//implementation for bidirectional iterators:
	template<typename Iterator, typename Distance>
	EnableIf<IsBidirectionalIterator<Iterator> &&
			!IsRandomAccessIteator<Iterator>>
		advanceIter(Iterator& x, Distance n)
	{
		if (n > 0)
		{
			for(; n > 0; ++x, --n){}
		}
		else
		{
			for(; n < 0; --x, ++n){}
		}
	}

	//implementation for all other iterators:
	template<typename Iterator, typename Distance>
	EnableIf<!IsBidirectionalIterator<Iterator>>
		advanceIter(Iterator& x, Distance n)
	{
		if (n < 0) {
			throw "advanceIter(): invalid iterator category for negative n";
		}
		while (n > 0) {
			++x;
			--n;
		}
	}

	template<typename Iterator>
	constexpr bool IsInputIterator = 
		IsConvertibleT<
			typename std::iterator_traits<Iterator>::iterator_category,
			std::input_iterator_tag>::value;

	template<typename T>
	class Container {
		public:
			//constructor from an input iterator sequence:
			template<typename Iterator,
					typename = EnableIfT<IsInputIterator<Iterator>::Type>>
			Container(Iterator first, Iterator second);

			//constructor from an input iterator sequence:
			template<typename Iterator,
					typename = EnableIfT<IsRandomAccessIteator<Iterator>>::Type,
					typename = int>
			Container(Iterator first, Iterator second);//ERROR

			//convert to a container so long as the value types are convertible:
			template<typename U, typename = EnableIfT<IsConvertibleT<T, U>::value>::Type>
			operator Container<U>() const;
	};

	template<typename Iterator, typename Distance>
	void advanceIter(Iterator& x, Distance n) {
		if constexpr (IsRandomAccessIteator<Iterator>) {
			x += n;
		}
		else if constexpr (IsBidirectionalIterator<Iterator>) {
			if (n > 0)
			{
				for(; n > 0; ++x, --n){}
			}
			else {
				for(; n < 0; --x, ++n){}
			}
		}
		else {
			if (n < 0)
			{

			}
			while (n > 0)
			{
				++x;
				--n;
			}
		}
	}

	//template<typename T>
	//class Container {
	//	public:
	//		//construct from an input iterator sequence:
	//		template<typename Iterator>
	//		requires IsInputIterator<Iterator>
	//		Container(Iterator first, Iterator last);

	//		//construct from a random access iterator sequence:
	//		template<typename Iterator>
	//		requires IsRandomAccessIteator<Iterator>
	//		Container(Iterator first, Iterator last);

	//		//convert to a container so long as the value types are convertible:
	//		template<typename U>
	//		requires IsConvertible<T, U>
	//		operator Container<U>() const;
	//};

	/*using namespace Note19dot6dot2;

	template<typename Key, typename Value, typename = void>
	class Dictionary
	{

	};

	template<typename Key, typename Value>
	class Dictionary<Key, Value, typename EnableIfT<HasLessT<Key, Key>::value>::Type>
	{
		private:
			std::map<Key, Value> data;
		public:
			Value& operator[](Key const& key) {
				return data[key];
			}
	};*/

	//construct a set of match() overloads for the types in Types...
	template<typename... Types>
	struct MatchOverloads;

	//basis case:nothing matched
	template<>
	struct MatchOverloads<> {
		static void match(...);
	};

	//recursive case:introduce a new match() overload:
	template<typename T1, typename... Rest>
	struct MatchOverloads<T1, Rest...> : public MatchOverloads<Rest...> {
		static T1 match(T1);					//introduce overload for T1
		using MatchOverloads<Rest...>::match;//collect overloads from bases
	};

	//find the best match for T in Types...
	template<typename T, typename... Types>
	struct BestMatchInSetT {
		using Type = decltype(MatchOverloads<Types...>::match(std::declval<T>()));
	};

	template<typename T, typename... Types>
	using BestMatchInSet = typename BestMatchInSetT<T, Types...>::Type;

	//primary template
	template<typename Iterator,
			typename Tag = 
			BestMatchInSet<
			typename std::iterator_traits<Iterator>::iterator_category,
			std::input_iterator_tag,
			std::bidirectional_iterator_tag,
			std::random_access_iterator_tag>>
	class Advance;

	//general, liner-time implementation for input iterators:
	template<typename Iterator>
	class Advance<Iterator, std::input_iterator_tag>
	{
		public:
			using DifferenceType = typename std::iterator_traits<Iterator>::difference_type;

			void operator()(Iterator& x, DifferenceType n) const
			{
				while (n > 0) {
					++x;
					--n;
				}
			}
	};

	//bidirectional, linear-time algorithm for bidirectional iterators:
	template<typename Iterator>
	class Advance<Iterator, std::bidirectional_iterator_tag>
	{
		public:
			using DifferenceType = typename std::iterator_traits<Iterator>::difference_type;

			void operator()(Iterator& x, DifferenceType n) const
			{
				if (n > 0) {
					while (n > 0) {
						++x;
						--n;
					}
				}
				else
				{
					while (n < 0) {
						--x;
						++n;
					}
				}
			}
	};

	void test() {
		//Dictionary
	}
}

namespace Note20dot5 {
	using namespace Note20dot1;

	template<typename T>
	class IsContextualBoolT {
	private:
		template<typename T> struct Identity;

		template<typename U> static std::true_type
		test(Identity<decltype(std::declval<U>() ? 0 : 1)>*);

		template<typename U> static std::false_type
			test(...);

	public:
		static constexpr bool value = decltype(test<T>(nullptr))::value;
	};

	template<typename T>
	constexpr bool IsContextualBool = IsContextualBoolT<T>::value;

	template<typename T1, typename T2>
	class HasLess {
		template<typename T> struct Identity;

		template<typename U1, typename U2> static std::true_type
		test(Identity<decltype(std::declval<U1>() < std::declval<U2>())>*);

		//SFINAE
		template<typename U1, typename U2> static std::false_type
		test(...);
		public:
			static constexpr bool value = decltype(test<T1, T2>(nullptr))::value;
	};
	//primary template
	template<typename T1, typename T2, bool HasLess>
	class LessResultImpl {
		public:
			using Type = decltype(std::declval<T1>() < std::declval<T2>());
	};
	//partial specialization
	template<typename T1, typename T2>
	class LessResultImpl<T1, T2, false>{

	};

	template<typename T1, typename T2>
	class LessResultT : public LessResultImpl<T1, T2, HasLess<T1, T2>::value> {

	};

	template<typename T1, typename T2>
	using LessResult = typename LessResultT<T1, T2>::Type;

	template<typename T>
	EnableIf<IsContextualBool<LessResult<T const&, T const&>>, T const&>
		min(T const& x, T const& y)
	{
		if(y < x) return y;
		return x;
	}

	//------
	struct X1{};
	bool operator<(X1 const&, X1 const&){ return true; }

	struct X2{};
	bool operator<(X2, X2){ return true; }

	struct X3{};
	bool operator<(X3&, X3&){ return true; }

	struct X4{};

	struct BoolConvertible {
		operator bool() const { return true; } //implicit conversion to bool
	};

	struct X5{};
	BoolConvertible operator<(X5 const&, X5 const&)
	{
		return BoolConvertible();
	}

	struct NotBoolConvertible { //no conversion to bool

	};

	struct X6{};
	NotBoolConvertible operator<(X6 const&, X6 const&) {
		return NotBoolConvertible();
	}

	struct BoolLike {
		explicit operator bool() const { return true; } //explicit conversion to bool
	};

	struct X7{};
	BoolLike operator<(X7 const&, X7 const&){ return BoolLike(); }

	

	void test2() {
		min(X1(), X1());
		min(X2(), X2());
		//min(X3(), X3());
		//min(X4(), X4());
		min(X5(), X5());
		//min(X6(), X6());
		min(X7(), X7());
	}
}


namespace Chapter20 {
	using namespace Note20dot5;

	void M_Test() {
		test2();
	}
}

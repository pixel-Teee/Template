#pragma once

#include <iostream>
#include <utility>

namespace Note19dot1dot1 {
	template<typename T>
	struct AccumulationTraits;

	template<>
	struct AccumulationTraits<char> {
		using AccT = int;
	};
	template<>
	struct AccumulationTraits<short> {
		using AccT = int;
	};
	template<>
	struct AccumulationTraits<int> {
		using AccT = long;
	};
	template<>
	struct AccumulationTraits<unsigned int> {
		using AccT = unsigned long;
	};
	template<>
	struct AccumulationTraits<float> {
		using AccT = double;
	};

	template<typename T>
	auto accum(T const* beg, T const* end)
	{
		//return type is traits of the element type
		using AccT = typename AccumulationTraits<T>::AccT;
		AccT total{};
		while (beg != end) {
			total += *beg;
			++beg;
		}
		return total;
	}

	void test() {
		int num[] = {1, 2, 3, 4, 5};

		std::cout << "the average value of the integer values is "
				  << accum(num, num + 5) / 5
				  << '\n';

		//create array of character values
		char name[] = "templates";
		int length = sizeof(name) - 1;

		//print average character value
		std::cout << "the average value of the characters in \""
					<< name << "\" is"
					<< accum(name, name + length) / length
					<< '\n';
	}

	
}

namespace Note19dot1dot3 {
	//new trait provides an zero element as a constant that can be evaluated at compile time
	template<typename T>
	struct AccumulationTraits;

	template<>
	struct AccumulationTraits<char> {
		using AccT = int;
		static AccT const zero = 0;
	};

	template<>
	struct AccumulationTraits<short> {
		using AccT = int;
		static AccT const zero = 0;
	};

	template<>
	struct AccumulationTraits<int> {
		using AccT = long;
		static AccT const zero = 0;
	};

	template<typename T>
	auto accum(T const* beg, T const* end)
	{
		using AccT = typename AccumulationTraits<T>::AccT;
		AccT total = AccumulationTraits<T>::zero;//init total by trait value

		while (beg != end) {
			total += *beg;
			++beg;
		}
		return total;
	}

	template<>
	struct AccumulationTraits<float> {
		using AccT = float;
		static constexpr float zero = 0.0f;
	};
}

namespace Note19dot1dot4 {
	template<typename T>
	struct AccumulationTraits;

	template<>
	struct AccumulationTraits<char>{
		using AccT = int;
		static constexpr AccT zero() {
			return 0;
		}
	};

	template<>
	struct AccumulationTraits<short> {
		using AccT = int;
		static constexpr AccT zero() {
			return 0;
		}
	};

	template<>
	struct AccumulationTraits<int> {
		using AccT = long;
		static constexpr AccT zero() {
			return 0;
		}
	};

	template<>
	struct AccumulationTraits<unsigned int> {
		using AccT = unsigned long;
		static constexpr AccT zero() {
			return 0;
		}
	};

	template<>
	struct AccumulationTraits<float> {
		using AccT = double;
		static constexpr AccT zero() {
			return 0;
		}
	};

	template<typename T, typename AT = AccumulationTraits<T>>
	auto accum(T const* beg, T const* end)
	{
		typename AT::AccT total = AT::zero();
		while (beg != end)
		{
			total += *beg;
			++beg;
		}
		return total;
	}

	/*class BigInt {
		BigInt(long, long);
	};

	template<>
	struct AccumulationTraits<BigInt> {
		using AccT = BigInt;
		static BigInt zero() {
			return BigInt{0};
		}
	};*/

	//AccT total = AccumulationTraits<T>::zero();
}

namespace Note19dot2 {
	using namespace Note19dot1dot4;

	//policy class 
	class SumPolicy {
		public:
			template<typename T1, typename T2>
			static void accumulate(T1& total, T2 const& value) {
				total += value;
			}
	};

	class MultPolicy {
		public:
			template<typename T1, typename T2>
			static void accumulate(T1& total, T2 const& value) {
				total *= value;
			}
	};

	template<typename T,
			//typename Policy = SumPolicy,
			typename Policy = SumPolicy,
			typename Traits = AccumulationTraits<T>>
		auto accum(T const* beg, T const* end)
	{
		using AccT = typename Traits::AccT;
		AccT total = Traits::zero();
		while (beg != end)
		{
			Policy::accumulate(total, *beg);
			++beg;
		}
		return total;
	}

	//template<typename Iter>
	//auto accum(Iter start, Iter end)
	//{
	//	//value type
	//	using VT = typename std::iterator_traits<Iter>::value_type;

	//	VT total{};
	//	while (start != end)
	//	{
	//		total += *start;
	//		++start;
	//	}
	//	return total;
	//}

	void test2()
	{
		int num[] = {1, 2, 3, 4, 5};

		std::cout << "the product of the integer values is "
					<< accum<int, MultPolicy>(num, num + 5)
					<< '\n';
	}
}

namespace Note19dot3 {
	//Type Functions
	template<typename T>
	struct TypeSize {
		static std::size_t const value = sizeof(T);
	};

	//Element Types
	template<typename T>
	struct ElementT;//primary template

	template<typename T>
	struct ElementT<std::vector<T>> {
		using Type = T;//partial specialization for std::vector
	};

	template<typename T>
	struct ElementT<std::list<T>> {
		using Type = T;//partial specialization for std::list
	};

	template<typename T, std::size_t N>
	struct ElementT<T[N]> {
		using Type = T;//known bounds
	};

	template<typename T>
	struct ElementT<T[]> {
		using Type = T;
	};

	template<typename T>
	void printElementType(T const& c)
	{
		std::cout << "Container of "
			<< typeid(typename ElementT<T>::Type).name()
			<< " elements.\n";
	}

	template<typename C>
	struct ElementT {
		using Type = typename C::value_type;
	};

	//template<typename T1, typename T2, ...>
	//class X {
	//	public:
	//		using ... = T1;
	//		using ... = T2;
	//};

	void test3() {
		/*std::cout << "TypeSize<int>::value = "
					<< TypeSize<int>::value << '\n';*/

		std::vector<bool> s;
		printElementType(s);
		int arr[42];
		printElementType(arr);
	}

	/*template<typename T, typename C>
	T sumOfElements(C const& c);

	template<typename C>
	typename ElementT<C>::Type sumOfElements(C const& c);*/

	/*template<typename T>
	using ElementType = typename ElementT<T>::Type;

	template<typename C>
	ElementT<C> sumOfElements(C const& c);*/
}

namespace Note19dot3dot2 {
	template<typename T>
	struct RemoveReferenceT {
		using Type = T;
	};

	template<typename T>
	struct RemoveReferenceT<T&> {
		using Type = T;
	};

	template<typename T>
	struct RemoveReferenceT<T&&> {
		using Type = T;
	};

	template<typename T>
	using RemoveReference = typename RemoveReferenceT<T>::Type;

	//Add Reference

	template<typename T>
	struct AddLValueReferenceT {
		using Type = T&;
	};

	template<typename T>
	using AddLValueReference = typename AddLValueReferenceT<T>::Type;

	template<typename T>
	struct AddRValueReferenceT {
		using Type = T&&;
	};

	template<typename T>
	using AddRValueReference = typename AddRValueReferenceT<T>::Type;

	////lighter weight
	//template<typename T>
	//using AddLValueReferenceT = T&;

	//template<typename T>
	//using AddRValueReferenceT = T&&;

	template<>
	struct AddLValueReferenceT<void> {
		using Type = void;
	};

	template<>
	struct AddLValueReferenceT<void const> {
		using Type = void const;
	};

	template<>
	struct AddLValueReferenceT<void volatile> {
		using Type = void volatile;
	};

	template<>
	struct AddLValueReferenceT<void const volatile> {
		using Type = void const volatile;
	};

	//removing qualifiers
	template<typename T>
	struct RemoveConstT {
		using Type = T;
	};

	template<typename T>
	struct RemoveConstT<T const> {
		using Type = T;
	};

	template<typename T>
	using RemoveConst = typename RemoveConstT<T>::type;

	//removing volatile
	template<typename T>
	struct RemoveVolatileT {
		using Type = T;
	};

	template<typename T>
	struct RemoveVolatileT<T volatile> {
		using Type = T;
	};

	template<typename T>
	using RemoveVolatile = typename RemoveVolatileT<T>::Type;

	//transformation traits can be composed

	template<typename T>
	struct RemoveCVT : RemoveConstT<typename RemoveVolatileT<T>::Type> {

	};

	template<typename T>
	using RemoveCV = typename RemoveCVT<T>::Type;

	//decay
	template<typename T>
	struct DecayT : RemoveCVT<T> {

	};

	template<typename T>
	struct DecayT<T[]> {
		using Type = T*;
	};

	template<typename T, std::size_t N>
	struct DecayT<T[N]> {
		using Type = T*;
	};

	//function-to-pointer decay
	template<typename R, typename... Args>
	struct DecayT<R(Args...)> {
		using Type = R(*)(Args...);
	};

	template<typename R, typename... Args>
	struct DecayT<R(Args..., ...)> {
		using Type = R(*)(Args..., ...);
	};

	template<typename T>
	void f(T)
	{

	}

	template<typename A>
	void printParameterType(void (*)(A))
	{
		std::cout << "Parameter type: " << typeid(A).name() << '\n';
		std::cout << "- is int:       " << std::is_same<A, int>::value << '\n';
		std::cout << "- is const:     " << std::is_const<A>::value << "\n";
		std::cout << "- is pointer:   " << std::is_pointer<A>::value << '\n';
	}

	template<typename T>
	void printDecayedType()
	{
		using A = typename DecayT<T>::Type;

		std::cout << "Parameter type: " << typeid(A).name() << '\n';
		std::cout << "- is int:       " << std::is_same<A, int>::value << '\n';
		std::cout << "- is const:     " << std::is_const<A>::value << '\n';
		std::cout << "- is pointer:   " << std::is_pointer<A>::value << '\n';
	}

	void test4() {
		printParameterType(&f<int>);
		printParameterType(&f<int const>);
		printParameterType(&f<int [7]>);
		printParameterType(&f<int(int)>);

		//print decay

		printDecayedType<int>();
		printDecayedType<int const>();
		printDecayedType<int [7]>();
		printDecayedType<int(int)>();
	}
}

namespace Note19dot3dot3 {
	template<bool val>
	struct BoolConstant {
		using Type = BoolConstant<val>;
		static constexpr bool value = val;
	};
	using TrueType = BoolConstant<true>;
	using FalseType = BoolConstant<false>;

	template<typename T1, typename T2>
	struct IsSameT : FalseType
	{
		//static constexpr bool value = false;
	};

	template<typename T>
	struct IsSameT<T, T> : TrueType
	{
		//static constexpr bool value = true;
	};

	template<typename T1, typename T2>
	constexpr bool isSame = IsSameT<T1, T2>::value;

	template<typename T>
	void fooImpl(T, TrueType)
	{
		std::cout << "fooImpl(T, true) for int called\n";
	}

	template<typename T>
	void fooImpl(T, FalseType)
	{
		std::cout << "fooImpl(T, false) for other type called\n";
	}

	template<typename T>
	void foo(T t)
	{
		fooImpl(t, IsSameT<T, int>{});//choose impl depending on whether T is int
	}

	void test5() {
		foo(42);
		foo(7.7);
	}
}

namespace Note19dot3dot4 {
	using namespace Note19dot3dot2;
	template<typename T1, typename T2>
	struct PlusResultT {
		using Type = decltype(std::declval<T1>() + std::declval<T2>());
	};

	template<typename T1, typename T2>
	using PlusResult = typename PlusResultT<T1, T2>::Type;

	/*template<typename T1, typename T2>
	Array<RemoveCV<RemoveReference<PlusResult<T1, T2>>>
	operator + (Array<T1> const&, Array<T2> const&);*/
}

namespace Note19dot4 {
	using namespace Note19dot3dot3;
	//SFINAE-Based Traits

	//template<typename T>
	//struct IsDefaultConstructibleT {
	//	private:
	//		//test() trying substitute call of a default constructor for T passed as U:
	//		template<typename U, typename = decltype(U())>
	//		static char test(void*);

	//		//test() callback
	//		template<typename>
	//		static long test(...);
	//	public:
	//		static constexpr bool value 
	//		= IsSameT<decltype(test<T>(nullptr)), char>::value;
	//};

	template<typename T>
	struct IsDefaultConstructibleHelper {
	private:
		//test() trying substitute call of a default constructor for T passed as U:
		template<typename U, typename = decltype(U())>
		static std::true_type test(void*);

		//test() callback
		template<typename>
		static std::false_type test(...);
	public:
		using Type = decltype(test<T>(nullptr));
	};

	template<typename T>
	struct IsDefaultConstructibleT : IsDefaultConstructibleHelper<T>::Type {

	};

	//template<typename T>
	//struct IsDefaultConstructibleT {
	//	private:
	//		//ERROR
	//		template<typename, typename = decltype(T())>
	//		static char test(void*);

	//		template<typename>
	//		static long test(...);
	//	public:
	//		static constexpr bool value
	//		= IsSameT<decltype(test<T>(nullptr)), char>::value;
	//};

	void test6() {
		//IsDefaultConstructibleT <int>::value;//yields true

		//struct S {
		//	S() = delete;
		//};
		//IsDefaultConstructibleT<S>::value;//yields false

		struct S {
			S() = delete;
		};


		std::cout << IsDefaultConstructibleT<S>::value << std::endl;
		std::cout << IsDefaultConstructibleT<int>::value << std::endl;
	}
	

}

namespace Note19dot4dot2 {
	template<typename...> using VoidT = void;

	//primary template
	template<typename, typename = VoidT<>>
	struct IsDefaultConstructibleT : std::false_type
	{


	};

	//partial specialization
	template<typename T>
	struct IsDefaultConstructibleT<T, VoidT<decltype(T())>> : std::true_type
	{

	};
}

namespace Note19dot4dot3 {
	//helper: checking validity of f(args...) for F f and Args... args
	template<typename F, typename... Args,
			typename = decltype(std::declval<F>()(std::declval<Args&&>()...))>
	std::true_type isValidImpl(void());

	//fallback if helper SINAE'd out
	template<typename F, typename... Args>
	std::false_type isValidImpl(...);

	//define a lambda that takes a lambda f and returns whether calling f with args is valid
	inline constexpr
		auto isValid = [](auto f) {
						return [](auto&&... args) {
							return decltype(
								isValidImpl<decltype(f),
								decltype(args)&&...>
								(nullptr)){};
						};
	};

	//helper template to represent a type as a value
	template<typename T>
	struct TypeT {
		using Type = T;
	};

	//helper to wrap a type as a value
	template<typename T>
	constexpr auto type = TypeT<T>{};

	//helper to unwrap a wrapped type in unevaluated contexts
	template<typename T>
	T valueT(TypeT<T>);//no definition needed

	constexpr auto isDefaultConstructible
		= isValid(
			[](auto x)->decltype((void)decltype(valueT(x))()) {}
	);

	template<typename T>
	using isDefaultConstructibleT
		= decltype(isDefaultConstructible(std::declval<T>()));

	void test7() {
		

		constexpr auto hasFirst
		= isValid([](auto x)->decltype((void)valueT(x).first){});

		//
	}
}

namespace Chapter19 {
	using namespace Note19dot2;
	using namespace Note19dot3;
	using namespace Note19dot3dot2;
	using namespace Note19dot4;
	void M_Test() {
		test2();
		test3();
		test4();
		test6();
	}
}
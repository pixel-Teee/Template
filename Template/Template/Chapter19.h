#pragma once

#include <iostream>
#include <utility>
#include <type_traits>

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

namespace Note19dot4dot4 {

	/*template<typename T1, typename T2>
	struct PlusResultT {
		using Type = decltype(std::declval<T1>() + std::declval<T2>());
	};

	template<typename T1, typename T2>
	using PlusResult = typename PlusResultT<T1, T2>::Type;*/

	//primary template
	template<typename, typename, typename = std::void_t<>>
	struct HasPlusT : std::false_type
	{

	};

	//partial specialization(may be SFINAE)
	template<typename T1, typename T2>
	struct HasPlusT<T1, T2, std::void_t<decltype(std::declval<T1>() + std::declval<T2>())>> : std::false_type
	{

	};

	template<typename T1, typename T2, bool = HasPlusT<T1, T2>::value>
	struct PlusResultT {
		using Type = decltype(std::declval<T1>() + std::declval<T2>());
	};

	template<typename T1, typename T2>
	struct PlusResultT<T1, T2, false> {

	};

	/*template<typename T>
	struct Array {

	};*/

	/*template<typename C, bool = HasMemberT_value_type<C>::value>
	struct ElementT {
		using Type = typename C::value_type;
	};

	template<typename C>
	struct ElementT<C, false> {

	};*/

	//template<typename T1, typename T2>
	//Array<typename PlusResultT<T1, T2>::Type>
	//operator + (Array<T1> const&, Array<T2>& const&);

	////overload
	//Array<A> operator+(Array<A> const& arrayA, Array<B> const& arrayB);

	void test7() {
		
	}
}

namespace Note19dot5 {

	/*template<typename FROM, typename TO, bool = IsVoidT<TO>::Value ||
										IsValidT<TO>::Value ||
										IsFunctionT<TO>::Value>
		struct IsConvertibleHelper {
		using Type = std::integral_constant<bool,
											IsVoidT<TO>::Type
											&& IsVoidT<FROM>::value>;
		;*/

	template<typename FROM, typename TO>
	struct IsConvertibleHelper{
		private:
			static void aux(TO);

			//
			template<typename F, typename = decltype(aux(std::declval<F>()))>
			static std::true_type test(void*);

			//fallback
			template<typename>
			static std::false_type test(...);
		public:
			
			//test return type is true_type or false_type
			//true_type or false_type have member value
			using Type = decltype(test<FROM>(nullptr));
	};

	template<typename FROM, typename TO>
	struct IsConvertibleT : IsConvertibleHelper<FROM, TO>::Type {

	};

	void test11() {
		std::cout << IsConvertibleT<int, int>::value << std::endl;
		std::cout << IsConvertibleT<int, std::string>::value << std::endl;
		std::cout << IsConvertibleT<char const*, std::string>::value << std::endl;
		std::cout << IsConvertibleT<std::string, char const*>::value << std::endl;
	}
}

namespace Note19dot6 {
	using namespace Note19dot3dot2;
	//helper to ignore any number of template parameters
	template<typename...> using VoidT = void;

	//primary template
	template<typename, typename = VoidT<>>
	struct HasSizeTypeT : std::false_type
	{

	};

	//partial specialization
	//may be SFINAE away
	template<typename T>
	struct HasSizeTypeT<T, VoidT<typename RemoveReferenceT<T>::Type>> : std::true_type
	{

	};

	struct CX {
		using size_type = std::size_t;
	};

	void test8() {
		std::cout << HasSizeTypeT<int>::value << std::endl;
		
		std::cout << HasSizeTypeT<CX>::value << std::endl;
	}
}

namespace Note19dot6dot2 {
	#define DEFINE_HAS_TYPE(MemType)				\
	template<typename, typename = std::void_t<>>	\
	struct HasTypeT_##MemType						\
	: std::false_type{};							\
	template<typename T>							\
	struct HasTypeT_##MemType<T, std::void_t<typename T::MemType>> \
	: std::true_type{};

	DEFINE_HAS_TYPE(value_type);
	DEFINE_HAS_TYPE(char_type);

	#define DEFINE_HAS_MEMBER(Member) \
	template<typename, typename = std::void_t<>> \
	struct HasMemberT_##Member					\
	: std::false_type{};\
	template<typename T>\
	struct HasMemberT_##Member<T, std::void_t<decltype(&T::Member)>> \
	: std::true_type{};

	DEFINE_HAS_MEMBER(size);
	DEFINE_HAS_MEMBER(first);

	//Detecting Member Functions

	//primary template:
	template<typename, typename = std::void_t<>>
	struct HasBeginT : std::false_type {

	};

	//partial specialization(may be SFINAE'd away)
	template<typename T>
	struct HasBeginT<T, std::void_t<decltype(std::declval<T>().begin())>>
	:std::true_type {

	};

	//primary template:
	template<typename, typename, typename = std::void_t<>>
	struct HasLessT : std::false_type
	{

	};

	//partial specialization
	template<typename T1, typename T2>
	struct HasLessT<T1, T2, std::void_t<decltype(std::declval<T1>() < std::declval<T2>())>>
	: std::true_type
	{

	};

	//primary template:
	template<typename, typename = std::void_t<>>
	struct HasVariousT : std::false_type
	{

	};

	//partial specialization
	template<typename T>
	struct HasVariousT<T, std::void_t<decltype(std::declval<T>().begin()),
		typename T::difference_type,
		typename T::iterator>>
		: std::true_type
	{

	};

	void test9() {
		std::cout << "int::value_type: "
					<< HasTypeT_value_type<int>::value << '\n';

		std::cout << "std::vector<int>::value_type: "
					<< HasTypeT_value_type<std::vector<int>>::value << '\n';

		std::cout << "int::size: "
					<< HasMemberT_size<int>::value << '\n';

		std::cout << "std::vector<int>::size: "
					<< HasMemberT_size<std::vector<int>>::value << '\n';
	}

	using namespace Note19dot4dot3;

	void test10() {
		/*constexpr auto hasFirst 
		= isValid([](auto x)-> decltype((void)valueT(x).first){});*/

		constexpr auto hasSizeType
		= isValid([](auto x)->typename decltype(valueT(x))::size_type{});

		struct CX {
			using size_type = std::size_t;
		};

		std::cout << "hasSizeType: " << hasSizeType(type<CX>) << '\n';

		if constexpr (!hasSizeType(type<int>)) {
			std::cout << "int has no size_type\n";
		}

		//define to check for < 
		constexpr auto hasLess
		= isValid([](auto x, auto y)-> decltype(valueT(x) < valueT(y)){});

		std::cout << hasLess(42, type<char>) << std::endl;//yields true

		//std::cout << "hasFirst: " << hasFirst(type<std::pair<int, int>>) << '\n';
	}
}

namespace Note19dot7 {
	//If-Then-Else

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

	template<auto N>
	struct SmallestIntT {
		using Type = 
			typename IfThenElseT<N <= std::numeric_limits<char>::max(), char,
				typename IfThenElseT<N <= std::numeric_limits<short>::max(), short,
					typename IfThenElseT<N <= std::numeric_limits<int>::max(), int,
						typename IfThenElseT<N <= std::numeric_limits<long>::max(), long,
							typename IfThenElseT<N <= std::numeric_limits<long long>::max(),
							long long,
							void
							>::Type
						>::Type
					>::Type
				>::Type
			>::Type;
	};

	//yield T when using member Type:
	template<typename T>
	struct IdentityT {
		using Type = T;
	};

	//to make unsigned after IfThenElse was evaluated:
	template<typename T>
	struct MakeUnsignedT {
		using Type = typename std::make_unsigned<T>::type;
	};

	template<typename T>
	struct UnsignedT {
		using Type = typename IfThenElse<std::is_integral<T>::value
								&& !std::is_same<T, bool>::value,
								MakeUnsignedT<T>,
								IdentityT<T>>::Type;
	};

	void test12() {
		//SmallestIntT<156161561616165>::Type q;
		//std::cout << typeid(q).name() << std::endl;

		UnsignedT<int>::Type a = 4U;
		std::cout << a << std::endl;
	}
}

namespace Note19dot7dot2 {
	/*template<typename T>
	struct IsNothrowMoveConstructibleT
		:std::bool_constant<noexcept(T(std::declval<T>()))>
	{

	};*/

	//primary template
	template<typename T, typename = std::void_t<>>
	struct IsNothrowMoveConstructibleT : std::false_type
	{

	};

	//partial specialization
	template<typename T>
	struct IsNothrowMoveConstructibleT<T, std::void_t<decltype(T(std::declval<T>()))>>
		: std::bool_constant<noexcept(T(std::declval<T>()))>
	{

	};
}

namespace Note19dot8 {
	
	//primary template: in general T is not a fundamental type
	template<typename T>
	struct IsFundaT : std::false_type {

	};

	//marco to specializa for fundamental types
	#define MK_FUNDA_TYPE(T)		\
		template<> struct IsFundaT<T> : std::true_type{ \
		};\

	MK_FUNDA_TYPE(void)
	MK_FUNDA_TYPE(bool)

	MK_FUNDA_TYPE(signed char)
	MK_FUNDA_TYPE(unsigned char)

	MK_FUNDA_TYPE(wchar_t)
	MK_FUNDA_TYPE(char16_t)
	MK_FUNDA_TYPE(char32_t)

	MK_FUNDA_TYPE(signed short)
	MK_FUNDA_TYPE(unsigned short)
	MK_FUNDA_TYPE(signed int)
	MK_FUNDA_TYPE(unsigned int)
	MK_FUNDA_TYPE(signed long)
	MK_FUNDA_TYPE(unsigned long)
	MK_FUNDA_TYPE(signed long long)
	MK_FUNDA_TYPE(unsigned long long)

	MK_FUNDA_TYPE(float)
	MK_FUNDA_TYPE(double)
	MK_FUNDA_TYPE(long double)

	MK_FUNDA_TYPE(std::nullptr_t)

	template<typename T>
	void test(T const&)
	{
		if (IsFundaT<T>::value)
		{
			std::cout << "T is a fundamental type" << '\n';
		}
		else
		{
			std::cout << "T is not a fundamental type" << '\n';
		}
	}

	void test13() {
		test(7);
		test("hello");
	}
}

namespace Note19dot8do2 {
	using namespace Note19dot7;
	using namespace Note19dot8;
	//pointers
	template<typename T>
	struct IsPointerT : std::false_type {

	};

	template<typename T>
	struct IsPointerT<T*> : std::true_type {
		using BaseT = T;
	};

	//references
	template<typename T>
	struct IsLValueReferenceT : std::false_type {

	};

	template<typename T>
	struct IsLValueReferenceT<T&> : std::true_type {
		using BaseT = T;
	};

	//rvalue reference
	template<typename T>
	struct IsRValueReferenceT : std::false_type {

	};

	template<typename T>
	struct IsRValueReferenceT<T&&> : std::true_type {
		using BaseT = T;
	};

	template<typename T>
	class IsReferenceT : public IfThenElseT<
						IsLValueReferenceT<T>::value,
						IsLValueReferenceT<T>,
						IsRValueReferenceT<T>
	>::Type {

	};

	//arrays
	template<typename T>
	struct IsArrayT : std::false_type { //primary template

	};

	template<typename T, std::size_t N> //partial specialization for arrays
	struct IsArrayT<T[N]> : std::true_type {
		using BaseT = T;
		static constexpr std::size_t size = N;
	};

	template<typename T>
	struct IsArrayT<T[]> : std::false_type {//unbound arrays
		using BaseT = T;
		static constexpr std::size_t size = 0;
	};

	//pointer to members
	template<typename T>
	struct IsPointerToMemberT : std::false_type {

	};

	//C: class name
	//partial specialization
	template<typename T, typename C>
	struct IsPointerToMemberT<T C::*>: std::true_type {
		using MemberT = T;
		using ClassT = C;
	};

	//identifying function types
	template<typename T>
	struct IsFunctionT : std::false_type { //primary template: no function

	};

	template<typename R, typename... Params>
	struct IsFunctionT<R(Params...)> : std::true_type {
		using Type = R;
		//using ParamsT = Typelist<Params...>;
		static constexpr bool variadic = false;
	};

	template<typename R, typename... Params>
	struct IsFunctionT<R(Params..., ...)> : std::true_type {
		using Type = R;
		//using ParamsT = Typelist<Params...>;
		static constexpr bool variadic = true;
	};

	//determining class types
	template<typename T, typename = std::void_t<>>
	struct IsClassT : std::false_type {
		//primary template
	};

	template<typename T>
	struct IsClassT<T, std::void_t<int T::*>> //calsses can have pointer-to-member
	: std::true_type {

	};

	//determining enumeration types
	template<typename T>
	struct IsEnumT {
		static constexpr bool value = !IsFundaT<T>::value &&
									!IsPointerT<T>::value &&
									!IsReferenceT<T>::value &&
									!IsArrayT<T>::value &&
									!IsPointerT<T>::value &&
									!IsFunctionT<T>::value &&
									!IsClassT<T>::value;
	};

	template<typename T>
	struct RParam {
		using Type = typename IfThenElse<sizeof(T) <= 2 * sizeof(void*),
										T,
										T const&>::Type;
	};

	/*template<typename T>
	struct RParam<Array<T>>
	{
		using Type = Array<T> const&;
	};*/

	/*template<typename T>
	struct RParam {
		using Type
		= IfThenElse<(sizeof(T) <= 2 * sizeof(void*)
					&& std::is_trivially_copy_constructible<T>::value
					&& std::is_trivially_move_constructible<T>::value)
					T,
					T const&>;
	};*/

	class MyClass1{
		public:
			MyClass1() {

			}
			MyClass1(MyClass1 const&) {
				std::cout << "MyClass1 copy constructor called\n";
			}
	};

	class MyClass2 {
		public:
			MyClass2() {

			}
			MyClass2(MyClass2 const&) {
				std::cout << "MyClass2 copy constructor called\n";
			}
	};

	//pass MyClass2 objects with RParam<> by value
	template<>
	class RParam<MyClass2> {
		public:
			using Type = MyClass2;
	};

	////function that allows parameter passing by value or by reference
	//template<typename T1, typename T2>
	//void foo(typename RParam<T1>::Type p1,
	//	typename RParam<T2>::Type p2)
	//{
	//	//...
	//}

	template<typename T1, typename T2>
	void foo_core(typename RParam<T1>::Type p1,
		typename RParam<T2>::Type p2)
	{

	}

	//wrapper to avoid explicit template parameter passing
	template<typename T1, typename T2>
	void foo(T1&& p1, T2&& p2)
	{
		foo_core<T1, T2>(std::forward<T1>(p1), std::forward<T2>(p2));
	}
}

namespace Chapter19 {
	/*using namespace Note19dot2;
	using namespace Note19dot3;
	using namespace Note19dot3dot2;
	using namespace Note19dot4;*/
	using namespace Note19dot5;
	using namespace Note19dot6;
	using namespace Note19dot6dot2;
	using namespace Note19dot7;
	using namespace Note19dot8;
	void M_Test() {
		/*test2();
		test3();
		test4();
		test6();*/
		test7();
		test8();
		test9();
		test10();
		test12();
		test13();
	}
}
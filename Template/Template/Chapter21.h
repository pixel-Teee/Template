#pragma once

#include <iostream>
#include <algorithm>
#include <iterator>
#include <type_traits>

namespace Note21dot1 {

	//a empty class requires one bit at run-time
	//because it need to compute distance between pointers/addresses
	class EmptyClass {
		
	};

	//---------test ebco---------
	class Empty {
		using Int = int;
	};

	class EmptyToo : public Empty {

	};

	class EmptyThree : public EmptyToo {

	};

	//constraint of the ebco
	//two base empty need offset at 0 and 1
	class NonEmpty : public Empty, public EmptyToo {

	};

	void test()
	{
		std::cout << "sizeof(Empty):      " << sizeof(Empty) << '\n';
		std::cout << "sizeof(EmptyToo):   " << sizeof(EmptyToo) << '\n';
		std::cout << "sizeof(EmptyThree): " << sizeof(EmptyThree) << '\n';
		std::cout << "sizeof(NonEmpty):   " << sizeof(NonEmpty) << '\n';
	}

	/*
	template<typename T1, typename T2>
	class MyClass {
		private:
			T1 a;
			T2 b;
	};
	*/

	//also have their problem
	//t1 and t2 could not substituted with nonclasstype or union type
	//substituted with same type are also have problem
	//t1 or t2 will final
	template<typename T1, typename T2>
	class MyClass : private T1, private T2 {

	};

	/*
	template<typename CustomClass>
	class Optimizable {
		private:
			CustomClass info;//might be empty
			void* storage;
	};
	*/

	//base class type and member type
	template<typename Base, typename Member>
	class BaseMemberPair : private Base {
		private:
			Member mem;
		public:
			//constructor
			BaseMemberPair(Base const& b, Member const& m)
				:Base(b), mem(m) {

			}

			//access base class data via base()
			Base const& base() const {
				return static_cast<Base const&>(*this);
			}

			Base& base() {
				return static_cast<Base&>(*this);
			}

			//access member data via member()
			Member const& member() const {
				return this->mem;
			}

			Member& member() {
				return this->mem;
			}
	};

	template<typename CustomClass>
	class Optimizable {
		private:
			//base class type and member type
			//if you know custom class is class type and we have member is not empty
			//we can merger them
			BaseMemberPair<CustomClass, void*> info_and_storage;
	};
	
	class Apple {
		using Int = int;
	};

	void test2()
	{
		//only 8 bit, one pointer size, else might be 16 bit, because memory layout
		std::cout << "sizeof(Optimizable<Apple>): " << sizeof(Optimizable<Apple>) << '\n';
	}
}

namespace Note21dot2 {
	template<typename Derived>
	class CuriousBase {
		//...
	};

	template<typename T>
	class CuriousTemplate : public CuriousBase<CuriousTemplate<T>> {

	};

	template<typename CountedType>
	class ObjectCounter {
		private:
			inline static std::size_t count = 0;//number of existing objects
		protected:
			//default constructor
			ObjectCounter() {
				++count;
			}

			//copy constrcutor
			ObjectCounter(ObjectCounter<CountedType> const&) {
				++count;
			}

			//move constructor
			ObjectCounter(ObjectCounter<CountedType>&&) {
				++count;
			}

			//destructor
			~ObjectCounter() {
				--count;
			}

		public:
			//return number of existing objects:
			static std::size_t live() {
				return count;
			}
	};

	template<typename CharT>
	class MyString : public ObjectCounter<MyString<CharT>> {

	};

	void test2() {
		MyString<char> s1, s2;

		std::cout << "num of MyString<char>:	"
				<< MyString<char>::live() << '\n';
	}

	template<typename T>
	class Array {
		static bool areEqual(Array<T> const& a, Array<T> const& b);

		public:
			friend bool operator==(Array<T> const& a, Array<T> const& b) {
				return areEqual(a, b);
			}
	};

	class S {

	};

	template<typename T>
	class Wrapper {
		private:
			T object;
		public:
			Wrapper(T obj) : object(obj) {//implicit conversion from T to Wrapper<T>

			}
			friend void foo(Wrapper<T> const&) {

			}
	};

	void test3() {
		S s;
		Wrapper<S> w(s);
		foo(w);//Wrapper<S> is a class associated with w
		//foo(s);//Wrapper<S> is not associated with S
	}
}

namespace Note21dot2dot3 {
	template<typename Derived>
	class EqualityComparable
	{
		public:
			friend bool operator!=(Derived const& x1, Derived const& x2) {
				return !(x1 == x2);
			}
	};

	class X : public EqualityComparable<X>
	{
		public:
			friend bool operator==(X const& x1, X const& x2) {
				//
				return true;
			}
	};
	void test4() {
		X x1, x2;
		if (x1 != x2) {

		}
	}
}

namespace Note21dot2dot3 {
	template<typename Derived, typename Value, typename Category,
			typename Reference = Value&, typename Distance = std::ptrdiff_t>
	class IteratorFacade {
		public:
			using value_type = typename std::remove_const<Value>::type;
			using reference = Reference;
			using pointer = Value*;
			using difference_type = Distance;
			using iterator_category = Category;

			//input iterator interface
			reference operator *() const {
				return asDerived().dereference();
			}

			//pointer operator->() const {}
			Derived& operator++(){
				asDerived().increment();
				return asDerived();
			}
			Derived operator++(int){
				Derived result(asDerived());
				asDerived().increment();
				return result;
			}
			friend bool operator==(IteratorFacade const& lhs,
									IteratorFacade const& rhs)
			{	
				return lhs.asDerived().equals(rhs.asDerived());
			}

			//bidirectional iterator interface
			Derived& operator--(){ 
				asDerived().decrement();
				return asDerived();
			}
			Derived operator--(int)
			{
				Derived result(asDerived());
				asDerived().decrement();
				return result;
			}

			Derived& asDerived(){ return *static_cast<Derived*>(this); }
			Derived const& asDerived() const {
				return *static_cast<Derived const*>(this);
			}

			friend difference_type operator - (IteratorFacade const& lhs,
				IteratorFacade const& rhs)
			{
				//return lhs.asDerived() - rhs.asDerived();
				return lhs.asDerived() - rhs.asDerived();
			}
	};

	template<typename T>
	class ListNode
	{
		public:
			T value;
			ListNode<T>* next = nullptr;
			~ListNode(){ delete next; }
	};

	template<typename T>
	class ListNodeIterator : public IteratorFacade<ListNodeIterator<T>, T,
												std::forward_iterator_tag>
	{
		ListNode<T>* current = nullptr;
		public:
			T& dereference() const {
				return current->value;
			}
			void increment() {
				current = current->next;
			}
			bool equals(ListNodeIterator const& other) const {
				return current = other.current;
			}
			ListNodeIterator(ListNode<T>* current = nullptr) : current(current){}
	};

	class IteratorFacadeAccess
	{
		//only IteratorFacade can use theses definitions
		template<typename Derived, typename Value, typename Category,
		typename Reference, typename Distance>
		friend class IteratorFacade;

		//required of all iterators:
		template<typename Reference, typename Iterator>
		static Reference dereference(Iterator const& i) {
			return i.dereference();
		}

		//required of bidirectional iterators:
		template<typename Iterator>
		static void decrement(Iterator& i) {
			return i.decrement();
		}
	};

	void test5() {
		ListNode<int> Head;
		ListNode<int>* current = &Head;
		for (size_t i = 0; i < 10; ++i)
		{
			current->value = i;
			current->next = new ListNode<int>;
			current = current->next;
		}
		
		ListNodeIterator<int> iterator(&Head);

		for (size_t i = 0; i < 10; ++i)
		{
			std::cout << iterator.dereference() << std::endl;
			iterator.increment();
		}
	}

	struct Person {
		std::string firstName;
		std::string lastName;
		friend std::ostream& operator<<(std::ostream& strm, Person const& p) {
			return strm << p.lastName << ", " << p.firstName;
		}
	};

	template<typename Iterator, typename T>
	class ProjectionIterator
		: public IteratorFacade<ProjectionIterator<Iterator, T>,
			T,
			typename std::iterator_traits<Iterator>::iterator_category,
			T&,
			typename std::iterator_traits<Iterator>::difference_type>
	{
		using Base = typename std::iterator_traits<Iterator>::value_type;
		using Distance = typename std::iterator_traits<Iterator>::difference_type;

		Iterator iter;
		T Base::* member;

		friend class IteratorFacadeAccess;

		public:
			ProjectionIterator(Iterator iter, T Base::* member)
			:iter(iter), member(member){}

		T& dereference() const {
			return (*iter).*member;
		}

		void increment() {
			++iter;
		}

		bool equals(ProjectionIterator const& other) const {
			return iter == other.iter;
		}

		void decrement() {
			--iter;
		}

		friend Distance operator-(ProjectionIterator const& lhs,
			ProjectionIterator const& rhs) {
			return rhs.iter - lhs.iter;
		}
	};

	template<typename Iterator, typename Base, typename T>
	auto project(Iterator iter, T Base::* member) {
		return ProjectionIterator<Iterator, T>(iter, member);
	}

	void test6() {
		std::vector<Person> authors = {
			{"David", "Vandevoorde"},
			{"Nicolai", "Josuttis"},
			{"Douglas", "Gregor"}
		};

		std::copy(project(authors.begin(), &Person::firstName),
				project(authors.end(), &Person::firstName),
				std::ostream_iterator<std::string>(std::cout, "\n"));
	}
}

namespace Note21dot3 {
	//class Point {
	//	public:
	//		double x, y;
	//		//P
	//		Point() : x(0.0), y(0.0){}
	//		Point(double x, double y) : x(x), y(y){}
	//};

	//template<typename P>
	//class Polygon
	//{
	//	private:
	//		std::vector<P> points;
	//	public:
	//		//public operations
	//};

	//class LabeledPoint : public Point
	//{
	//	public:
	//		std::string label;
	//		LabeledPoint() : Point(), label(""){}
	//		LabeledPoint(double x, double y) : Point(x, y), label(""){}
	//};

	template<typename... Mixins>
	class Point : public Mixins...
	{
		public:
			double x, y;
			Point() : Mixins()..., x(0.0), y(0.0){}
			Point(double x, double y) : Mixins()..., x(x), y(y){}
	};

	class Label {
		public:
			std::string label;
			Label() : label(""){}
	};

	using LabeledPoint = Point<Label>;

	class Color
	{
		public:
			unsigned char red = 0, green = 0, blue = 0;
	};

	using MyPoint = Point<Label, Color>;

	template<typename... Mixins>
	class Polygon
	{
	private:
		std::vector<Point<Mixins...>> points;
	public:
		//...
	};

	
}

namespace Note21dot3dot1 {
	template<typename... Mixins>
	class Point : public Mixins<Point>...
	{
	public:
		double x, y;
		Point() : Mixins<Point>()..., x(0.0), y(0.0){}
		Point(double x, double y) : Mixins<Point>()..., x(x), y(y){}
	};
}

namespace Note21dot3dot2 {
	class NotVirtual {

	};

	class Virtual {
		public:
			virtual void foo() {

			}
	};

	template<typename... Mixins>
	class Base : public Mixins... {
		public:
			void foo() {
				std::cout << "Base::foo()" << '\n';
			}
	};

	template<typename... Mixins>
	class Derived : public Base<Mixins...> {
		public:
			void foo() {
				std::cout << "Derived::foo()" << '\n';
			}
	};

	void test7() {

		Base<NotVirtual>* p1 = new Derived<NotVirtual>;
		p1->foo();//call Base::foo()

		Base<Virtual>* p2 = new Derived<Virtual>;
		p2->foo();//call Derived::foo()
	}
}

namespace Note21dot4 {
	/*template<typename Policy1 = DefaultPolicy1,
			typename Policy2 = DefaultPolicy2,
			typename Policy3 = DefaultPolicy3,
			typename Policy4 = DefaultPolicy4>
		class BreadSlicer {

	};*/

	class DefaultPolicies {
	public:
		/*using P1 = DefaultPolicy1;
		using P2 = DefaultPolicy2;
		using P3 = DefaultPolicy3;
		using P4 = DefaultPolicy4;*/
	};

	//
	template<typename Policy>
	class Policy1_is : virtual public DefaultPolicies {
	public:
		using P1 = Policy;
	};

	template<typename Policy>
	class Policy2_is : virtual public DefaultPolicies {
	public:
		using P2 = Policy;
	};

	template<typename Policy>
	class Policy3_is : virtual public DefaultPolicies {
	public:
		using P3 = Policy;
	};

	template<typename Policy>
	class Policy4_is : virtual public DefaultPolicies {
	public:
		using P4 = Policy;
	};

	//allow having even the same base class more than once
	template<typename Base, int D>
	class Discriminator : public Base {

	};

	//PolicySelector<A, B, C, D> creates A, B, C, D as base classes
	template<typename Setter1, typename Setter2,
			typename Setter3, typename Setter4>
	class PolicySelecter : public Discriminator<Setter1, 1>,
							public Discriminator<Setter2, 2>,
							public Discriminator<Setter3, 3>,
							public Discriminator<Setter4, 4> 
	{

	};

	
}

namespace Chapter21 {
	using namespace Note21dot2;
	using namespace Note21dot2dot3;
	using namespace Note21dot2dot3;
	//using namespace Note21dot2dot3;
	void M_Test() {
		test2();
		test4();
		test5();
		test6();
	}	
}
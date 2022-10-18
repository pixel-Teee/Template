#pragma once

namespace Note12do1
{
	//------definitions1------
	template<typename T> //a namespace scope class template
	class Data
	{
	public:
		static constexpr bool copyable = true;
	};

	template<typename T> //a namespace scope function template
	void log(T x)
	{
		//...
	}

	template<typename T> //a namespace scope variable template
	T zero = 0;

	template<typename T> //a namespace scope variable template
	bool dataCopyanble = Data<T>::copyable;

	template<typename T> //a namespace scope alias template
	using DataList = Data<T*>;
	//------definitions1------

	//------definitions2------
	class Collection
	{
	public:
		template<typename T> //an in-class member class template definition
		class Node
		{
			//...
		};

		template<typename T> //an in-class(and therefore implicitly inline)
		T* alloc() {		//member function template definition
			//...
		};

		template<typename T> //a member variable template(since C++14)
		static T zero = 0;

		template<typename T> //a member alias template
		using NodePtr = Node<T>*;
	};
	//------definitions2------
}

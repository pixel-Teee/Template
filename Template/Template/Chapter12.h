#pragma once

namespace Note12do1
{
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
}

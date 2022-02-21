#pragma once

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
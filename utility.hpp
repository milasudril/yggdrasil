//@	{"targets":[{"name": "utility.hpp", "type":"include"}]}

#ifndef DATA_STORE_UTILITY_HPP
#define DATA_STORE_UTILITY_HPP

#include <cstddef>

#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)

namespace DataStore
	{
	template<class T>
	struct Empty{};

	template<class ... Types>
    struct TypeSet
		{
		static constexpr size_t size()
			{return sizeof...(Types);}
		};

	template<size_t N, class T>
	struct GetType;

	template<size_t N, class Head, class ... Tail>
	struct GetType<N, TypeSet<Head, Tail...>>:GetType<N-1, TypeSet<Tail...>>
		{};

	template<class Head, class ... Tail>
	struct GetType<0, TypeSet<Head, Tail...>>
		{using type = Head;};

	template<class ... T>
	struct GetSize;

	template<class ... T>
	struct GetSize<TypeSet<T...>>
		{static constexpr const size_t value = sizeof...(T);};
	}

#endif

//@	{"targets":[{"name": "utility.hpp", "type":"include"}]}

#ifndef DATA_STORE_UTILITY_HPP
#define DATA_STORE_UTILITY_HPP

#include <cstddef>
#include <type_traits>

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



	template<class T, class = int>
	struct HasSize : std::false_type { };

	template<class T>
	struct HasSize<T, decltype(std::declval<T>().size(), 0)> : std::true_type { };

	template<class T, class = int>
	struct HasData : std::false_type { };

	template<class T>
	struct HasData<T, decltype(std::declval<T>().data(), 0)> : std::true_type { };

	template<class T, class Enable=void>
	struct IsSimpleArray : std::false_type{};

	template<class T>
	struct IsSimpleArray<T, std::enable_if_t<HasData<T>::value>>
		{
		static constexpr auto value =
		       HasSize<T>::value && HasData<T>::value
			&& std::is_arithmetic_v<std::decay_t<decltype(*std::declval<T>().data())>>;
		};

	}

#endif

//@	{"targets":[{"name": "utility.hpp", "type":"include"}]}

#ifndef DATA_STORE_UTILITY_HPP
#define DATA_STORE_UTILITY_HPP

#include <cstddef>
#include <type_traits>

#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)

namespace DataStore
	{
	template<class T, class = int>
	struct HasSize : std::false_type {};

	template<class T>
	struct HasSize<T, decltype(std::declval<T>().size(), 0)> : std::true_type {};

	template<class T, class = int>
	struct HasData : std::false_type {};

	template<class T>
	struct HasData<T, decltype(std::declval<T>().data(), 0)> : std::true_type {};

	template<class T, class Enable=void>
	struct IsSimpleArray : std::false_type{};

	template<class T>
	struct IsPod
		{
		static constexpr auto value = std::is_trivial_v<T> && std::is_standard_layout_v<T>;
		};

	template<class T>
	struct IsSimpleArray<T, std::enable_if_t<HasData<T>::value>>
		{
		static constexpr auto value =
			   HasSize<T>::value && HasData<T>::value
			&& IsPod<std::decay_t<decltype(*std::declval<T>().data())>>::value;
		};


	template<class T, class = int>
	struct HasBegin : std::false_type {};

	template<class T>
	struct HasBegin<T, decltype(std::declval<T>().begin(), 0)> : std::true_type {};

	template<class T, class = int>
	struct HasEnd : std::false_type {};

	template<class T>
	struct HasEnd<T, decltype(std::declval<T>().end(), 0)> : std::true_type {};

	template<class T, class U, class Enable = void>
	struct IsSequenceOf : std::false_type {};

	template<class T, class U>
	struct IsSequenceOf<T, U, std::enable_if_t<HasBegin<T>::value && HasEnd<T>::value >>
		{
		static constexpr auto value = std::is_same_v<std::decay_t<decltype(*std::declval<T>().begin())>, U>;
		};
	}

#endif

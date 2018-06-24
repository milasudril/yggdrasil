//@	{"targets":[{"name":"make_default.hpp","type":"include"}]}

#ifndef DATA_STORE_MAKE_DEFAULT_HPP
#define DATA_STORE_MAKE_DEFAULT_HPP

#include "strint/make_value.hpp"

#include <type_traits>

namespace DataStore
	{
	template<class T>
	std::enable_if_t<std::is_default_constructible_v<T>, T> make_default()
		{return T{};}

	template<class T>
	decltype(Strint::make_default<T>()) make_default()
		{return Strint::make_default<T>();}
	}

#endif

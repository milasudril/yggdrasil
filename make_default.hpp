#include "strint/make_value.hpp"


	template<class T>
	std::enable_if_t<std::is_default_constructible_v<T>, T> make_default()
		{return T{};}

	template<class T>
	decltype(Strint::make_default<T>()) make_default()
		{return Strint::make_default<T>();}

//@	{"targets":[{"name": "utility.hpp", "type":"include"}]}

#ifndef DATA_STORE_UTILITY_HPP
#define DATA_STORE_UTILITY_HPP

#include <cstddef>

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
	}

#endif

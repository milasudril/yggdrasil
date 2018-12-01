//@	{"targets":[{"name":"key_type_count_value_defs.hpp","type":"include"}]}

#ifndef DATA_STORE_KEY_TYPE_COUNT_VALUE_DEFS_HPP
#define DATA_STORE_KEY_TYPE_COUNT_VALUE_DEFS_HPP

#include "analib/inlinestring/inlinestring.hpp"

#include <cstdint>
#include <climits>

namespace DataStore
	{
	namespace KeyTypeCountValue
		{
		static_assert(CHAR_BIT == 8, "This library requires 8-bit chars");
		using KeyType = Analib::InlineString<char, 32>;
		using TypeId = uint8_t;
		using ArraySize = uint32_t;
		}
	}

#endif

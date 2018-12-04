//@	{"targets":[{"name":"key_type_count_value_defs.hpp","type":"include"}]}

#ifndef DATA_STORE_KEY_TYPE_COUNT_VALUE_DEFS_HPP
#define DATA_STORE_KEY_TYPE_COUNT_VALUE_DEFS_HPP

#include "analib/inlinestring/inlinestring.hpp"

#include <cstdint>
#include <climits>

#include <limits>

namespace DataStore
	{
	namespace KeyTypeCountValueDefs
		{
		static_assert(CHAR_BIT == 8, "This library requires 8-bit chars");
		using KeyType = Analib::InlineString<char, 32>;

		class TypeId
			{
			public:
				using value_type = uint8_t;

				template<size_t From>
				static constexpr TypeId create()
					{
					static_assert(From <= std::numeric_limits<value_type>::max());
					TypeId ret{From};
					return ret;
					}

				TypeId() = default;

				constexpr operator size_t() const
					{return m_value;}

				constexpr bool operator==(TypeId other) const
					{return m_value == other.m_value;}

				constexpr bool operator<(TypeId other) const
					{return m_value < other.m_value;}

			private:
				value_type m_value;
				explicit constexpr TypeId(size_t from) : m_value(from) {}
			};

		inline constexpr bool operator!=(TypeId a, TypeId b)
			{return !(a == b);}

		inline constexpr bool operator>(TypeId a, TypeId b)
			{return b < a;}

		inline constexpr bool operator>=(TypeId a, TypeId b)
			{return !(a < b);}

		inline constexpr bool operator<=(TypeId a, TypeId b)
			{return !(a > b);}

		using ArraySize = uint32_t;
		}
	}

#endif

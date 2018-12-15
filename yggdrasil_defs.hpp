//@	{"targets":[{"name":"yggdrasil_defs.hpp", "type":"include"}]}

#ifndef YGGDRASIL_DEFS_HPP
#define YGGDRASIL_DEFS_HPP

#include "analib/inlinestring/inlinestring.hpp"

#include <cstdint>
#include <cstddef>

namespace Yggdrasil
	{
	constexpr Analib::InlineString MagicNumber{"yggdrasil v 1.0"};
	constexpr uint32_t ByteOrderMarker{0x01020304};
	constexpr size_t KeySize{32};
	constexpr size_t ArrayElemCountSize{4};

	enum class TypeId
		{
		// Signed integer types
		 Int8
		,Int16
		,Int32
		,Int64

		// Unsigned integer types
		,Uint8
		,Uint16
		,Uint32
		,Uint64

		// Floating point types
		,Float8
		,Float16
		,Float32
		,Float64

		// Other types
		,String
		,Raw
		,Reserved0
		,Reserved1

		// Vector versions of arthmetic types
		,VecInt8
		,VecInt16
		,VecInt32
		,VecInt64

		,VecUint8
		,VecUint16
		,VecUint32
		,VecUint64

		,VecFloat8
		,VecFloat16
		,VecFloat32
		,VecFloat64

		,Reserved2
		,Reserved3
		,Reserved4
		,Compound

		// Arrays
		,ArrayInt8
		,ArrayInt16
		,ArrayInt32
		,ArrayInt64

		,ArrayUint8
		,ArrayUint16
		,ArrayUint32
		,ArrayUint64

		,ArrayFloat8
		,ArrayFloat16
		,ArrayFloat32
		,ArrayFloat64

		,ArrayString
		,ArrayRaw
		,ArrayReserved0
		,ArrayReserved1

		,ArrayVecInt8
		,ArrayVecInt16
		,ArrayVecInt32
		,ArrayVecInt64

		,ArrayVecUint8
		,ArrayVecUint16
		,ArrayVecUint32
		,ArrayVecUint64

		,ArrayVecFloat8
		,ArrayVecFloat16
		,ArrayVecFloat32
		,ArrayVecFloat64

		,ArrayReserved2
		,ArrayReserved3
		,ArrayReserved4
		,ArrayCompound
		,Size
		};

	static_assert(TypeId::Size == static_cast<TypeId>(64));
	}

#endif

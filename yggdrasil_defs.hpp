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
	constexpr size_t TypeIdSize{1};

	enum class TypeId
		{
		 VecInt8
		,VecUint8
		,VecFloat8
		,Reserved3
		,VecInt16
		,VecUint16
		,VecFloat16
		,Reserved7
		,VecInt32
		,VecUint32
		,VecFloat32
		,Reserved11
		,VecInt64
		,VecUint64
		,VecFloat64
		,Reserved15
		,Int8
		,Uint8
		,Float8
		,String
		,Int16
		,Uint16
		,Float16
		,Reserved23
		,Int32
		,Uint32
		,Float32
		,Reserved27
		,Int64
		,Uint64
		,Float64
		,Compound
		,ArrayVecInt8
		,ArrayVecUint8
		,ArrayVecFloat8
		,ArrayReserved3
		,ArrayVecInt16
		,ArrayVecUint16
		,ArrayVecFloat16
		,ArrayReserved7
		,ArrayVecInt32
		,ArrayVecUint32
		,ArrayVecFloat32
		,ArrayReserved11
		,ArrayVecInt64
		,ArrayVecUint64
		,ArrayVecFloat64
		,ArrayReserved15
		,ArrayInt8
		,ArrayUint8
		,ArrayFloat8
		,ArrayString
		,ArrayInt16
		,ArrayUint16
		,ArrayFloat16
		,ArrayReserved23
		,ArrayInt32
		,ArrayUint32
		,ArrayFloat32
		,ArrayReserved27
		,ArrayInt64
		,ArrayUint64
		,ArrayFloat64
		,ArrayCompound
		,Size
		};

	static_assert(TypeId::Size == static_cast<TypeId>(64));
	}

#endif

//@	{"targets":[{"name":"yggdrasil_defs.hpp", "type":"include"}]}

#ifndef YGGDRASIL_DEFS_HPP
#define YGGDRASIL_DEFS_HPP

#include "analib/inlinestring/inlinestring.hpp"

#include <cstdint>
#include <cstddef>
#include <cassert>

namespace Yggdrasil
	{
	constexpr Analib::InlineString MagicNumber{"yggdrasil v 1.0"};
	constexpr uint32_t ByteOrderMarker{0x01020304};
	constexpr size_t KeySize{32};
	constexpr size_t ArrayElemCountSize{4};
	constexpr size_t TypeIdSize{1};

	enum class TypeIdValue : uint8_t
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

	enum class TypeCategory : uint8_t {Integer, UnsignedInteger, Float, String, Object};
	enum class ElementSize : uint8_t {ONE, TWO, FOUR, EIGHT};

	class TypeAttribute
		{
		static constexpr uint8_t Vector = 1;
		static constexpr uint8_t Array = 2;
		};

	class TypeId
		{
		public:
			constexpr TypeId(TypeIdValue val) : m_value{static_cast<uint8_t>(val)}{}
			constexpr TypeId& attributes(TypeAttribute);

			constexpr TypeId& size(ElementSize size)
				{
				assert(category()!=TypeCategory::String && category()!=TypeCategory::Object);
				m_value &= ~SIZE_MASK;
				m_value |= (static_cast<uint8_t>(size) << SIZE_SHIFT);
				return *this;
				}

			constexpr TypeId& category(TypeCategory cat)
				{
				if(cat==TypeCategory::String || cat==TypeCategory::Object)
					{
					category_field(0x3);
					size_field(cat == TypeCategory::String? 0x0 : 0x3);
					}
				else
					{category_field(static_cast<uint8_t>(cat));}
				return *this;
				}

			constexpr TypeCategory category() const
				{
				auto val = category_field();
				if(val == 0x3)
					{return size_field() == 0? TypeCategory::String : TypeCategory::Object;}
				else
					{return static_cast<}
				}

			constexpr ElementSize size()
				{return static_cast<ElementSize>( (m_value&SIZE_MASK)>>SIZE_SHIFT );}

			constexpr TypeIdValue value() const
				{return static_cast<TypeIdValue>(m_id);}

		private:
			uint8_t m_value;

			static constexpr uint8_t ATTRIBUTE_MASK = 0x30;
			static constexpr uint8_t SizeMask = 0x0c;
			static constexpr uint8_t SizeShift = 2;
			static constexpr uint8_t CategoryMask = 0x03;
			static constexpr uint8_t CategoryShift =0;

			static_assert((ATTRIBUTE_MASK|SizeShift|CategoryMask) == 0x3f);

			constexpr void size_field(uint8_t val)
				{
				m_value&=~SizeMask;
				m_value|=(val << SizeShift);
				}

			constexpr uint8_t size_field() const
				{return (m_value&SizeMask) >> SizeShift;}

			constexpr void category_field(uint8_t val)
				{
				m_value&=~CategoryMask;
				m_value|=(val<< CategoryShift);
				}

			constexpr uint8_t category_field() const
				{return (m_value&CategoryMask) >> CategoryShift;}
		};

	constexpr inline bool operator==(TypeId a, TypeId b)
		{return a.value() == b.value();}

	constexpr inline bool operator!=(TypeId a, TypeId b)
		{return !(a==b);}

	}

#endif

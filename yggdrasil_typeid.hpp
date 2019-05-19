//@	{"targets":[{"name":"yggdrasil_typeid.hpp", "type":"include"}]}

#ifndef YGGDRASIL_TYPEID_HPP
#define YGGDRASIL_TYPEID_HPP

#include <cassert>
#include <cstdint>

namespace Yggdrasil
	{
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

	static_assert(TypeIdValue::Size == static_cast<TypeIdValue>(64));

	enum class TypeCategory : uint8_t {Integer, UnsignedInteger, Float, String, Compound};
	enum class ElementSize : uint8_t {One, Two, Four, Eight};

	struct TypeAttribute
		{
		static constexpr uint8_t Vector = 1;
		static constexpr uint8_t Array = 2;
		};

	class TypeId
		{
		public:

			// Allow implicit conversion here (fully compatible representation)
			// VecInt8 has code zero.
			constexpr TypeId(TypeIdValue val = TypeIdValue::VecInt8) : m_value{static_cast<uint8_t>(val)}{}

			constexpr TypeId& attributes(uint8_t attrs)
				{
				assert(!((attrs & TypeAttribute::Vector) && category_field()==0x3));
				attributes_field(attrs^TypeAttribute::Vector);
				return *this;
				}

			constexpr uint8_t attributes() const
 				{return attributes_field()^TypeAttribute::Vector;}

			constexpr TypeId& size(ElementSize size)
				{
				assert(category_field()!=0x3);
				size_field(static_cast<uint8_t>(size));
				return *this;
				}

			constexpr ElementSize size()
				{return static_cast<ElementSize>(size_field());}

			constexpr TypeId& category(TypeCategory cat)
				{
				if(cat==TypeCategory::String || cat==TypeCategory::Compound)
					{
					category_field(0x3);
					attributes_field(1);  // Cannot have vector of string or object
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
					{return size_field() == 0? TypeCategory::String : TypeCategory::Compound;}
				else
					{return static_cast<TypeCategory>(val);}
				}

			constexpr TypeIdValue value() const
				{return static_cast<TypeIdValue>(m_value);}

		private:
			uint8_t m_value;

			static constexpr uint8_t AttributeMask = 0x30;
			static constexpr uint8_t AttributeShift = 4;
			static constexpr uint8_t SizeMask = 0x0c;
			static constexpr uint8_t SizeShift = 2;
			static constexpr uint8_t CategoryMask = 0x03;
			static constexpr uint8_t CategoryShift = 0;

			static_assert((AttributeMask|SizeMask|CategoryMask) == 0x3f);

			constexpr uint8_t attributes_field() const
				{return (m_value&AttributeMask) >> AttributeShift;}

			constexpr void attributes_field(uint8_t val)
				{
				m_value&=~AttributeMask;
				m_value|=(val << AttributeShift);
				}

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

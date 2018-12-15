//@	{"targets":[{"name":"compound.hpp","type":"include"
//, "pkgconfig_libs":["IlmBase"]
//@}]}

#ifndef YGGDRASIL_COMPOUND_HPP
#define YGGDRASIL_COMPOUND_HPP

#include "basic_compound.hpp"
#include "key_type_count_value_defs.hpp"

#include "native_decoder.hpp"
#include "key_type_count_value_deserializer.hpp"
#include "native_encoder.hpp"
#include "key_type_count_value_serializer.hpp"


#include "analib/cxxstring/string.hpp"

#include <cstdint>
#include <cstddef>
#include <array>

namespace Yggdrasil
	{
	struct Minifloat
		{
		// Unimplemented
		};

	struct Half
		{
		// Unimplemented
		};

	namespace detail
		{
		template<class T>
		struct MakeVector
			{using type __attribute__((vector_size(4*sizeof(T)))) = T;};

		template<>
		struct MakeVector<Minifloat>
			{using type = Analib::Empty<Minifloat>;};

		template<>
		struct MakeVector<Half>
			{using type = Analib::Empty<Half>;};
		}

	template<class T>
	using vec4_t = typename detail::MakeVector<T>::type;

	using String = Analib::StringNoSso;
	using KeyType = DataStore::KeyTypeCountValueDefs::KeyType;
	using StatusCode = DataStore::KeyTypeCountValueDefs::StatusCode;



	template<class ExceptionPolicy>
	using Compound = DataStore::BasicCompound
		<
		 ExceptionPolicy
		,KeyType

		// Signed intger types
		,int8_t
		,int16_t
		,int32_t
		,int64_t

		// Unsigned integer types
		,uint8_t
		,uint16_t
		,uint32_t
		,uint64_t

		// Floats
		,Minifloat
		,Half
		,float
		,double

		// Vector versions of each type
		,vec4_t<int8_t>
		,vec4_t<int16_t>
		,vec4_t<int32_t>
		,vec4_t<int64_t>

		,vec4_t<uint8_t>
		,vec4_t<uint16_t>
		,vec4_t<uint32_t>
		,vec4_t<uint64_t>

		,vec4_t<Minifloat>
		,vec4_t<Half>
		,vec4_t<float>
		,vec4_t<double>

		,String
		,std::byte
		,Analib::Empty<std::integral_constant<int, 1>>
		,Analib::Empty<std::integral_constant<int, 2>>
		,Analib::Empty<std::integral_constant<int, 3>>
		,Analib::Empty<std::integral_constant<int, 4>>
		,Analib::Empty<std::integral_constant<int, 5>>
		>;

	static_assert(Compound<int>::SupportedTypes::size() == 64);

	constexpr Analib::InlineString MagicNumber{"yggdrasil v 1.0"};
	constexpr uint32_t ByteOrderMarker{0x01020304};

	template<class ExceptionPolicy, class Source>
	[[nodiscard]] StatusCode load(Compound<ExceptionPolicy>& compound, Source& source)
		{
		decltype(MagicNumber) magic_number{};
		if(unlikely(source.read(&magic_number, sizeof(magic_number) != sizeof(magic_number))))
			{return StatusCode::EndOfFile;}

		uint32_t byte_order_marker{};
		if(unlikely(source.read(&byte_order_marker, sizeof(byte_order_marker)) != sizeof(byte_order_marker)))
			{return StatusCode::EndOfFile;}

		if(likely(MagicNumber != magic_number))
			{return StatusCode::UnsupportedFileFormat;}

		if(unlikely(ByteOrderMarker != byte_order_marker))
			{return StatusCode::UnsupportedByteOrder;}

		return DataStore::load(compound, DataStore::KeyTypeCountValueDeserializer{DataStore::NativeDecoder{source}});
		}

	template<class ExceptionPolicy, class Sink>
	[[nodiscard]] bool store(Compound<ExceptionPolicy> const& compound, Sink& sink)
		{
		if(unlikely(sink.write(&MagicNumber, sizeof(MagicNumber))) != sizeof(MagicNumber))
			{return false;}
		if(unlikely(!sink.write(&ByteOrderMarker, sizeof(ByteOrderMarker))) != sizeof(ByteOrderMarker))
			{return false;}
		return DataStore::store(compound, DataStore::KeyTypeCountValueSerializer{DataStore::NativeEncoder{sink}});
		}

	}

#endif

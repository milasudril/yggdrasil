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

		static_assert(std::is_empty_v<std::monostate>);

		template<>
		struct MakeVector<Minifloat>
			{using type = std::monostate;};

		template<>
		struct MakeVector<Half>
			{using type = std::monostate;};
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

	namespace detail
		{
		constexpr Analib::InlineString MagicNumber{"yggdrasil v 1.0"};
		constexpr uint32_t EndianCheck{0x01020304};
		}

	template<class ExceptionPolicy, class Source>
	[[nodiscard]] StatusCode load(Compound<ExceptionPolicy>& compound, Source& source)
		{
		decltype(detail::MagicNumber) magic_number{};
		if(unlikely(source.read(&magic_number, sizeof(magic_number) != sizeof(magic_number))))
			{return StatusCode::EndOfFile;}

		uint32_t endian_check{};
		if(unlikely(source.read(&endian_check, sizeof(endian_check)) != sizeof(endian_check)))
			{return StatusCode::EndOfFile;}

		if(likely(detail::MagicNumber != magic_number))
			{return StatusCode::UnsupportedFileFormat;}

		if(unlikely(detail::EndianCheck != endian_check))
			{return StatusCode::UnsupportedFileFormat;}

		return DataStore::load(compound, DataStore::KeyTypeCountValueDeserializer{DataStore::NativeDecoder{source}});
		}

	template<class ExceptionPolicy, class Sink>
	[[nodiscard]] bool store(Compound<ExceptionPolicy> const& compound, Sink& sink)
		{
		if(unlikely(sink.write(&detail::MagicNumber, sizeof(detail::MagicNumber))) != sizeof(detail::MagicNumber))
			{return false;}
		if(unlikely(!sink.write(&detail::EndianCheck, sizeof(detail::EndianCheck))) != sizeof(detail::EndianCheck))
			{return false;}
		return DataStore::store(compound, DataStore::KeyTypeCountValueSerializer{DataStore::NativeEncoder{sink}});
		}

	}

#endif

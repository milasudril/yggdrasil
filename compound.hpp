//@	{"targets":[{"name":"compound.hpp","type":"include"}]}

#ifndef YGGDRASIL_COMPOUND_HPP
#define YGGDRASIL_COMPOUND_HPP

#include "yggdrasil_defs.hpp"

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
#ifndef YGGDRASIL_HAS_MINIFLOAT
	struct Minifloat
		{
		// Unimplemented
		};
#endif

#ifndef YGGDRASIL_HAS_HALF
	struct Half
		{
		// Unimplemented
		};
#endif

	namespace detail
		{
#ifdef __AVX__
		constexpr auto MaxNativeVectorSize = 8;
#else
		constexpr auto MaxNativeVectorSize = 4;
#endif
		template<class T, class Enable = void>
		struct MakeVector;

		template<class T>
		struct HalfSize;

		template<>
		struct HalfSize<double>
			{using type = float;};

		template<>
		struct HalfSize<int64_t>
			{using type = int32_t;};

		template<>
		struct HalfSize<uint64_t>
			{using type = uint32_t;};

		template<class T>
		struct MakeVector<T, std::enable_if_t< (sizeof(T) <= MaxNativeVectorSize) >>
			{using type __attribute__((vector_size(4*sizeof(T)))) = T;};

		template<class T>
		struct MakeVector<T, std::enable_if_t< (sizeof(T) > MaxNativeVectorSize) >>
			{
			// For now, disable support for over-sized vectors
			using type = Analib::Empty<T>;
			};

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

	template<size_t N>
	using Reserved = Analib::Empty<std::integral_constant<size_t, N>>;

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

		// Floating point types
		,Minifloat
		,Half
		,float
		,double

		// Other types
		,String
		,std::byte
		,Reserved<0>
		,Reserved<1>

		// Vector versions of arthmetic types
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

		// Other
		,Reserved<2>
		,Reserved<3>
		,Reserved<4>
		>;

	template<class ExceptionPolicy, class Source>
	[[nodiscard]] StatusCode load(Compound<ExceptionPolicy>& compound, Source& source)
		{
		decltype(MagicNumber) magic_number{};
		if(unlikely(source.read(&magic_number, sizeof(magic_number)) != sizeof(magic_number)))
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
		if(unlikely(sink.write(&ByteOrderMarker, sizeof(ByteOrderMarker))) != sizeof(ByteOrderMarker))
			{return false;}
		return DataStore::store(compound, DataStore::KeyTypeCountValueSerializer{DataStore::NativeEncoder{sink}});
		}

	}

#endif

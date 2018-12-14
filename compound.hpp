//@	{"targets":[{"name":"compound.hpp","type":"include"
//, "pkgconfig_libs":["IlmBase"]
//@}]}

#ifndef YGGDRASIL_COMPOUND_HPP
#define YGGDRASIL_COMPOUND_HPP

#include "basic_compound.hpp"
#include "key_type_count_value_defs.hpp"
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
		,std::monostate
		,std::monostate
		,std::monostate
		,std::monostate
		,std::monostate
		>;

	static_assert(Compound<int>::SupportedTypes::size() == 64);
	}

#endif

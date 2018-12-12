//@	{"targets":[{"name":"compound.hpp","type":"include"
//, "pkgconfig_libs":["IlmBase"]
//@}]}

#ifndef YGGDRASIL_COMPOUND_HPP
#define YGGDRASIL_COMPOUND_HPP

#include "basic_compound.hpp"
#include "key_type_count_value_defs.hpp"
#include "analib/cxxstring/string.hpp"

//#include <OpenEXR/half.h>
#include <cstdint>
#include <cstddef>
#include <array>

namespace Yggdrasil
	{
	namespace detail
		{
		template<class T>
		struct MakeVector
			{using type __attribute__((vector_size(4*sizeof(T)))) = T;};

	/*	template<>
		struct MakeVector<half>
			{using type = std::array<half, 4>;};*/
		}
	template<class T>
	using vec4_t = typename detail::MakeVector<T>::type;

	using String = Analib::StringNoSso;
	using KeyType = DataStore::KeyTypeCountValueDefs::KeyType;
	using StatusCode = DataStore::KeyTypeCountValueDefs::StatusCode;

	struct Half
		{
		// Unimplemented
		};

	template<class ExceptionPolicy>
	using Compound = DataStore::BasicCompound
		<
		 ExceptionPolicy
		,KeyType

		// Basic intger types
		,int8_t
		,uint8_t
		,int16_t
		,uint16_t
		,int32_t
		,uint32_t
		,int64_t
		,uint64_t

		// Floats
		,Half
		,float
		,double
    //
	//	// Vector types
		,vec4_t<int8_t>
		,vec4_t<uint8_t>
		,vec4_t<int16_t>
		,vec4_t<uint16_t>
		,vec4_t<int32_t>
		,vec4_t<uint32_t>
		,vec4_t<int64_t>
		,vec4_t<uint64_t>
	//	,vec4_t<half>
		,vec4_t<float>
		,vec4_t<double>

		,String
		,std::byte
		>;
	}

#endif

//@	{"targets":[{"name":"compound.hpp","type":"include"}]}
#ifndef YGGDRASIL_COMPOUND_HPP
#define YGGDRASIL_COMPOUND_HPP

#include "basic_compound.hpp"
#include "analib/cxxstring/string.hpp"
#include <cstdint>

namespace Yggdrasil
	{
	template<class T>
	using vec4_t __attribute__((vector_size(4*sizeof(T)))) = T ;

	using String = Analib::StringNoSso;


	template<class ExceptionPolicy>
	using Compound = DataStore::BasicCompound
		<
		 ExceptionPolicy

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
		//TODO (maybe decode will unpack to float: ,half
		,float
		,double


		,vec4_t<int32_t>
		,vec4_t<uint32_t>
		,vec4_t<float>
		// Skip vectorized double as it is not as widely supported as vectorized float
		,String
		>;

	enum class TypeId : intptr_t
		{
		 Unkonwn

		// Basic integer types
		,Int8
		,Uint8
		,Int16
		,Uint16
		,Int32
		,Uint32
		,Int64
		,Uint64

		// Floats
		// TODO:, Float16
		,Float32
		,Float64

		// Vector types
		,VecInt32
		,VecUint32
		,VecFloat32

		// Other
		, String
		, Compound


		// The same list for arrays
		,ArrayInt8
		,ArrayUint8
		,ArrayInt16
		,ArrayUint16
		,ArrayInt32
		,ArrayUint32
		,ArrayInt64
		,ArrayUint64

		// TODO:, Float16
		,ArrayFloat32
		,ArrayFloat64

		,ArrayVecInt32
		,ArrayVecUint32
		,ArrayVecFloat32

		,ArrayString
		,ArrayCompound
		};
	}

#endif

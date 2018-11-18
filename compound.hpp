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
		,int8_t
		,uint8_t
		,int16_t
		,uint16_t
		,int32_t
		,uint32_t
		,float
		,int64_t
		,uint64_t
		,double
		,vec4_t<int32_t>
		,vec4_t<uint32_t>
		,vec4_t<float>
		// Skip vectorized double as it is not as widely supported as vectorized float
		,String
		>;

	template<class T> struct ValueTag{};

	constexpr const char* getTypeName(ValueTag<int8_t>)
		{return "i08v";}
	constexpr const char* getTypeName(ValueTag<uint8_t>)
		{return "u08v";}

	constexpr const char* getTypeName(ValueTag<int16_t>)
		{return "i10v";}
	constexpr const char* getTypeName(ValueTag<uint16_t>)
		{return "u10v";}

	constexpr const char* getTypeName(ValueTag<int32_t>)
		{return "i20v";}
	constexpr const char* getTypeName(ValueTag<uint32_t>)
		{return "u20v";}

	constexpr const char* getTypeName(ValueTag<int64_t>)
		{return "i40v";}
	constexpr const char* getTypeName(ValueTag<uint64_t>)
		{return "u40v";}

	constexpr const char* getTypeName(ValueTag<float>)
		{return "f20v";}
	constexpr const char* getTypeName(ValueTag<double>)
		{return "f40v";}

	constexpr const char* getTypeName(ValueTag<vec4_t<int32_t>>)
		{return "I20v";}
	constexpr const char* getTypeName(ValueTag<vec4_t<uint32_t>>)
		{return "U20v";}
	constexpr const char* getTypeName(ValueTag<vec4_t<float>>)
		{return "F20v";}

	constexpr const char* getTypeName(ValueTag<String>)
		{return "strv";}

	template<class ExceptionPolicy>
	constexpr const char* getTypeName(ValueTag<Compound<ExceptionPolicy>>)
		{return "objv";}


	template<class T> struct ArrayTag
		{
		using tag = ValueTag<std::vector<T>>;
		};

	constexpr const char* getTypeName(ValueTag<std::vector<int8_t>>)
		{return "i08a";}
	constexpr const char* getTypeName(ValueTag<std::vector<uint8_t>>)
		{return "u08a";}

	constexpr const char* getTypeName(ValueTag<std::vector<int16_t>>)
		{return "i10a";}
	constexpr const char* getTypeName(ValueTag<std::vector<uint16_t>>)
		{return "u10a";}

	constexpr const char* getTypeName(ValueTag<std::vector<int32_t>>)
		{return "i20a";}
	constexpr const char* getTypeName(ValueTag<std::vector<uint32_t>>)
		{return "u20a";}

	constexpr const char* getTypeName(ValueTag<std::vector<int64_t>>)
		{return "i40a";}
	constexpr const char* getTypeName(ValueTag<std::vector<uint64_t>>)
		{return "u40a";}

	constexpr const char* getTypeName(ValueTag<std::vector<float>>)
		{return "f20a";}
	constexpr const char* getTypeName(ValueTag<std::vector<double>>)
		{return "f40a";}

	constexpr const char* getTypeName(ValueTag<std::vector<vec4_t<int32_t>>>)
		{return "I20a";}
	constexpr const char* getTypeName(ValueTag<std::vector<vec4_t<uint32_t>>>)
		{return "U20a";}
	constexpr const char* getTypeName(ValueTag<std::vector<vec4_t<float>>>)
		{return "F20a";}

	constexpr const char* getTypeName(ValueTag<std::vector<String>>)
		{return "stra";}

	template<class ExceptionPolicy>
	constexpr const char* getTypeName(ValueTag<std::vector<Compound<ExceptionPolicy>>>)
		{return "obja";}
	}

#endif

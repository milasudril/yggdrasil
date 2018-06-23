//@	{"targets":[{"name":"var","type":"include"}]}

#ifndef DATA_STORE_VAR_HPP
#define DATA_STORE_VAR_HPP

#include "strint/common_types.hpp"

#include <string>
#include <vector>
#include <variant>
#include <array>
#include <cstring>

namespace DataStore
	{
	class Compound;  // Predeclare Compound to avoid cyclic dependency between var_t and Compound

	template<class T>
	using Array = std::vector<T>;

	using Int8 = Strint::Int8;
	using Int16 = Strint::Int16;
	using Int32 = Strint::Int32;
	using Int64 = Strint::Int64;
	using UInt8 = Strint::UInt8;
	using UInt16 = Strint::UInt16;
	using UInt32 = Strint::UInt32;
	using UInt64 = Strint::UInt64;
	using Float = float;
	using Double = double;
	using String = std::string;

	using var_t = std::variant
		<
		 Int8
		,Int16
		,Int32
		,Int64
		,UInt8
		,UInt16
		,UInt32
		,UInt64
		,Float
		,Double
		,std::unique_ptr<String>
		,std::unique_ptr<Compound>

		,Array<Int8 >
		,Array<Int16>
		,Array<Int32>
		,Array<Int64>
		,Array<UInt8>
		,Array<UInt16>
		,Array<UInt32>
		,Array<UInt64>
		,Array<Float>
		,Array<Double>
		,Array<String>
		,Array<Compound>
		>;

	template<class T>
	std::enable_if_t<std::is_constructible_v<var_t, T>, var_t> make_var(T&& val)
		{return var_t{std::forward<T>(val)};}

	template<class T>
	std::enable_if_t<std::is_constructible_v<var_t, std::unique_ptr<std::remove_reference_t<T>>>, var_t>
	make_var(T&& val)
		{return var_t{std::make_unique<std::remove_reference_t<T>>(std::forward<T>(val))};}

	var_t make_var(const char* str)
		{return make_var(std::string(str));}


	template<class T>
	std::enable_if_t<std::is_constructible_v<var_t, T>, T const*>
	extract_val(var_t const* x)
		{return std::get_if<T>(x);}

	template<class T>
	std::enable_if_t<std::is_constructible_v<var_t, std::unique_ptr<T>>, T const*>
	extract_val(var_t const* x)
		{
		auto temp = std::get_if<std::unique_ptr<T>>(x);
		if(temp == nullptr)
			{return nullptr;}
		return temp->get();
		}

	namespace detail
		{
		template<class Visitor, class VariantType, int N = std::variant_size_v<VariantType>>
		struct VarTypeEnumerator
			{
			static void iterate(Visitor&& f)
				{
				using CurrentType = decltype(std::get<N-1>(VariantType{}));
				f.template visit<CurrentType>(std::forward<Visitor>(f));
				VarTypeEnumerator<Visitor, VariantType, N - 1>::iterate(std::forward<Visitor>(f));
				}
			};

		template<class Visitor, class VariantType>
		struct VarTypeEnumerator<Visitor, VariantType, 0>
			{
			static void iterate(Visitor&& f) { }
			};
		}

	template<class Visitor, class VariantType = var_t>
	void enumTypes(Visitor&& visitor)
		{
		detail::VarTypeEnumerator<Visitor, VariantType>::iterate(std::forward<Visitor>(visitor));
		}
	}
#endif

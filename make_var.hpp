//@	{"targets":[{"name":"var_hpp.hpp","type":"include"}]}

#ifndef DATA_STORE_MAKE_VAR_HPP
#define DATA_STORE_MAKE_VAR_HPP

#include <memory>
#include <type_traits>
#include <variant>

namespace DataStore
	{
	template<class VariantType, class T>
	std::enable_if_t<std::is_constructible_v<VariantType, T>, VariantType> make_var(T&& val)
		{return VariantType{std::forward<T>(val)};}

	template<class VariantType, class T>
	std::enable_if_t<std::is_constructible_v<VariantType, std::unique_ptr<std::remove_reference_t<T>>>, VariantType>
	make_var(T&& val)
		{return VariantType{std::make_unique<std::remove_reference_t<T>>(std::forward<T>(val))};}

	template<class T, class VariantType>
	std::enable_if_t<std::is_constructible_v<VariantType, T>, T const*>
	get_if(VariantType const* x)
		{return std::get_if<T>(x);}

	template<class T, class VariantType>
	std::enable_if_t<std::is_constructible_v<VariantType, std::unique_ptr<T>>, T const*>
	get_if(VariantType const* x)
		{
		auto temp = std::get_if<std::unique_ptr<T>>(x);
		if(temp == nullptr)
			{return nullptr;}
		return temp->get();
		}
	}
#endif

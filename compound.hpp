//@	{
//@	 "targets":[{"name": "compound.hpp", "type":"include"}]
//@	}

#ifndef DATA_STORE_COMPOUND_HPP
#define DATA_STORE_COMPOUND_HPP

#include "key_not_found_exception.hpp"
#include "type_mismatch_exception.hpp"

#include "strint/common_types.hpp"

#include <string>
#include <vector>
#include <variant>
#include <map>
#include <memory>

namespace DataStore
	{
	class Compound;
		
	using var_t = std::variant
		<
		 Strint::Int8
		,Strint::Int16
		,Strint::Int32
		,Strint::Int64
		,Strint::UInt8
		,Strint::UInt16
		,Strint::UInt32
		,Strint::UInt64
		,float
		,double
		,std::unique_ptr<std::string>
		,std::unique_ptr<Compound>
		
		,std::vector<Strint::Int8 >
		,std::vector<Strint::Int16>
		,std::vector<Strint::Int32>
		,std::vector<Strint::Int64>
		,std::vector<Strint::UInt8>
		,std::vector<Strint::UInt16>
		,std::vector<Strint::UInt32>
		,std::vector<Strint::UInt64>
		,std::vector<float>
		,std::vector<double>
		,std::vector<std::string>
		,std::vector<Compound>
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
	
	class Compound
		{
		public:
			Compound() : m_src_loc(std::make_unique<SourceLocation>())
				{}
			
			template<class KeyType, class Type>
			Compound& set(KeyType&& key, Type&& value) 
				{
				m_content.insert_or_assign(std::forward<KeyType>(key)
					,make_var(std::forward<Type>(value)));
				return *this;
				}
			
			template<class Type>
			Type const* getIf(std::string const& key) const noexcept
				{
				auto i = m_content.find(key);
				if(i == m_content.end())
					{return nullptr;}
				return extract_val<Type>(&i->second);
				}
				
			template<class Type>
			Type* getIf(std::string const& key) noexcept
				{return const_cast<Type*>(const_cast<Compound const*>(this)->getIf<Type>(key));}
				
			template<class Type>
			Type const& get(std::string const& key) const
				{
				auto i = m_content.find(key);
				if(i == m_content.end())
					{throw KeyNotFoundException(key, sourceLocation());}
				
				auto ret = extract_val<Type>(&i->second);
				if(ret == nullptr)
					{throw TypeMismatchException(key, "", sourceLocation());}

				return *ret;
				}
			
			template<class Type>
			Type& get(std::string const& key)
				{return const_cast<Type&>(const_cast<Compound const*>(this)->get<Type>(key));}

			void remove(const std::string& key)
				{m_content.erase(key);}
				
			auto begin() const
				{return m_content.begin();}
				
			auto end() const
				{return m_content.end();}
				
			auto begin()
				{return m_content.begin();}
				
			auto end()
				{return m_content.end();}
				
			auto size() const
				{return m_content.size();}
				
			SourceLocation const& sourceLocation() const noexcept
				{return *m_src_loc;}
				

		private:
			std::map<std::string, var_t> m_content;
			std::unique_ptr<SourceLocation> m_src_loc;
		};
	};

#endif


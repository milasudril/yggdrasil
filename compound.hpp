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
#include <cstring>
#include <array>

namespace DataStore
	{
	class Compound;
	
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
		

		
	template<class T>
	constexpr decltype(T::typeName()) getTypeName() noexcept
		{return T::typeName();}
	
	template<class T>
	constexpr std::enable_if_t<std::is_same_v<T, String>, char const*> getTypeName() noexcept
		{return "str";}
		
	template<class T>
	constexpr std::enable_if_t<std::is_same_v<T, Float>, char const*> getTypeName() noexcept
		{return "f20";}
		
	template<class T>
	constexpr std::enable_if_t<std::is_same_v<T, Double>, char const*> getTypeName() noexcept
		{return "f40";}
		
		
	
	namespace detail
		{
		// Helper function for concatenating two string sources into a std::array.
		template<class SrcA, class SrcB>
		constexpr auto strcat(SrcA const& a, SrcB const& b) noexcept
			{
			constexpr auto ptr_a = a();
			constexpr auto ptr_b = b();
			constexpr auto N_a = strlen(ptr_a);
			constexpr auto N_b = strlen(ptr_b);

			std::array<char, N_a + N_b + 1> ret{};
			auto ptr_src = ptr_a;
			auto ptr_dest = ret.begin();
			while(*ptr_src)
				{
				*ptr_dest = *ptr_src;
				++ptr_dest;
				++ptr_src;
				}
			
			ptr_src = ptr_b;
			while(*ptr_src)
				{
				*ptr_dest = *ptr_src;
				++ptr_dest;
				++ptr_src;
				}

			return ret;
			}
		}
	
	template<class T>
	struct ArrayTypeName
		{
		public:
			static constexpr char const* typeName() noexcept
				{return retval;}

		private:
			static constexpr auto val = detail::strcat([](){return getTypeName<T>();}
				, [](){return "a";});
			static constexpr auto retval = val.begin();
		};
		
	template<class T>
	constexpr decltype(getTypeName<typename T::value_type>()) getTypeName() noexcept
		{
		using Type = typename T::value_type;
		return ArrayTypeName<Type>::typeName();
		}

	
	class Compound
		{
		public:
			static constexpr auto invalidKey() noexcept
				{return std::variant_npos;}
			
			static constexpr char const* typeName() noexcept
				{return "obj";}
			
			Compound() : m_src_loc(std::make_unique<SourceLocation>())
				{}
			
			template<class KeyType, class Type>
			Compound& set(KeyType&& key, Type&& value) 
				{
				m_content.insert_or_assign(std::forward<KeyType>(key)
					,make_var(std::forward<Type>(value)));
				return *this;
				}
				

				
			var_t const* getIf(std::string const& key) const noexcept
				{
				auto i = m_content.find(key);
				if(i == m_content.end())
					{return nullptr;}
				return &i->second;
				}
			
			var_t* getIf(std::string const& key) noexcept
				{return const_cast<var_t*>(const_cast<Compound const*>(this)->getIf(key));}
				
			auto typeOfValue(std::string const& key) const noexcept
				{
				auto val = getIf(key);
				if(val == nullptr)
					{return std::variant_npos;}
				return val->index();
				}
			
			var_t const& get(std::string const& key) const
				{
				auto val = getIf(key);
				if(val == nullptr)
					{throw KeyNotFoundException(key, sourceLocation());}
				return *val;
				}
				
			var_t& get(std::string const& key)
				{return const_cast<var_t&>(const_cast<Compound const*>(this)->get(key));}
	
	

				
			template<class Type>
			Type const* getIf(std::string const& key) const noexcept
				{
				auto val = getIf(key);
				if(val == nullptr)
					{return nullptr;}
				return extract_val<Type>(val);
				}
				
			template<class Type>
			Type* getIf(std::string const& key) noexcept
				{return const_cast<Type*>(const_cast<Compound const*>(this)->getIf<Type>(key));}
				
			template<class Type>
			Type const& get(std::string const& key) const
				{
				auto& val = get(key);
				auto ret = extract_val<Type>(&val);
				if(ret == nullptr)
					{throw TypeMismatchException(key, getTypeName<Type>(), sourceLocation());}
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
				
			bool empty() const 
				{return size()==0;}
				
			SourceLocation const& sourceLocation() const noexcept
				{return *m_src_loc;}

		private:
			std::map<std::string, var_t> m_content;
			std::unique_ptr<SourceLocation> m_src_loc;
		};
	};

#endif


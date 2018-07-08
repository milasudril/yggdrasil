//@	{"targets":[{"name": "compound.hpp", "type":"include"}]}

#ifndef DATA_STORE_COMPOUND_HPP
#define DATA_STORE_COMPOUND_HPP

#include "key_not_found_exception.hpp"
#include "type_mismatch_exception.hpp"
#include "make_var.hpp"
#include "type_names.hpp"
#include "strint/common_types.hpp"

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <stack>
#include <string_view>

namespace DataStore
	{
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
	using Float32 = float;
	using Float64 = double;
	using String = std::string;
	
	class Compound;

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
		,Float32
		,Float64
		,std::unique_ptr<String>
		,std::unique_ptr<Compound>

		,Array<Int8>
		,Array<Int16>
		,Array<Int32>
		,Array<Int64>
		,Array<UInt8>
		,Array<UInt16>
		,Array<UInt32>
		,Array<UInt64>
		,Array<Float32>
		,Array<Float64>
		,Array<String>
		,Array<Compound>
		>;

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
			Compound& set(KeyType&& key, Type&& value) &
				{
				m_content.insert_or_assign(std::forward<KeyType>(key)
					,make_var<var_t>(std::forward<Type>(value)));
				return *this;
				}
				
			template<class KeyType, class Type>
			Compound&& set(KeyType&& key, Type&& value) &&
				{
				m_content.insert_or_assign(std::forward<KeyType>(key)
					,make_var<var_t>(std::forward<Type>(value)));
				return std::move(*this);
				}
				
			template<class KeyType, class Type>
			bool insert(KeyType&& key, Type&& value)
				{
				auto ip = m_content.insert(std::make_pair(std::forward<KeyType>(key)
					,make_var<var_t>(std::forward<Type>(value))));
				return ip.second;
				}



			var_t const* find(std::string_view key) const noexcept
				{
				auto i = m_content.find(key);
				if(i == m_content.end())
					{return nullptr;}
				return &i->second;
				}

			var_t* find(std::string_view key) noexcept
				{return const_cast<var_t*>(const_cast<Compound const*>(this)->find(key));}

			auto typeOfValue(std::string_view key) const noexcept
				{
				auto val = find(key);
				if(val == nullptr)
					{return std::variant_npos;}
				return val->index();
				}

			var_t const& get(std::string_view key) const
				{
				auto val = find(key);
				if(val == nullptr)
					{throw KeyNotFoundException(std::string{key}, sourceLocation());}
				return *val;
				}

			var_t& get(std::string_view key)
				{return const_cast<var_t&>(const_cast<Compound const*>(this)->get(key));}




			template<class Type>
			Type const* getIf(std::string_view key) const noexcept
				{
				auto val = find(key);
				if(val == nullptr)
					{return nullptr;}
				return DataStore::get_if<Type>(val);
				}

			template<class Type>
			Type* getIf(std::string_view key) noexcept
				{return const_cast<Type*>(const_cast<Compound const*>(this)->getIf<Type>(key));}

			template<class Type>
			inline Type const& get(std::string_view key) const;

			template<class Type>
			Type& get(std::string_view key)
				{return const_cast<Type&>(const_cast<Compound const*>(this)->get<Type>(key));}



			void remove(const std::string& key)
				{m_content.erase(key);}

			bool empty() const
				{return size()==0;}
				
			size_t size() const 
				{return m_content.size();}

			SourceLocation const& sourceLocation() const noexcept
				{return *m_src_loc;}
				
				
				
			template<template<class, class> class VisitorPolicy, class Visitor>
			void visitItems(Visitor&& visitor) const;
			
			auto begin() const
				{return m_content.begin();}
				
			auto end() const
				{return m_content.end();}
			
			
			
		private:
			std::map<std::string, var_t, std::less<>> m_content;
			std::unique_ptr<SourceLocation> m_src_loc;
		};

	template<class T>
	constexpr std::enable_if_t<std::is_same_v<T, std::unique_ptr<Compound>>, char const*> getTypeName() noexcept
		{return getTypeName<Compound>();}
		

	template<class Type>
	inline Type const& Compound::get(std::string_view key) const
		{
		auto& val = get(key);
		auto ret = DataStore::get_if<Type>(&val);
		if(ret == nullptr)
			{throw TypeMismatchException(std::string{key}, getTypeName<Type>(), sourceLocation());}
		return *ret;
		}
		
	template<class InputIterator, class Visitor>
	class ItemVisitor
		{
		public:
			ItemVisitor(InputIterator begin, InputIterator end, Visitor& visitor) : 
				m_begin(begin), m_end(end), r_visitor(visitor) {}
			
			template<class T>
			void operator()(T const& val)
				{r_visitor.visit(std::pair<std::string const&, T const&>{*r_key, val});}
				
			template<class T>
			void operator()(std::unique_ptr<T> const& val)
				{r_visitor.visit(std::pair<std::string const&, T const&>{*r_key, *val});}
			
			void visitItems()
				{
				std::for_each(m_begin, m_end, [this](auto&& item)
					{
					r_key = &item.first;
					std::visit(*this, item.second);
					});
				}
				
		private:
			InputIterator m_begin;
			InputIterator m_end;
			std::string const* r_key;
			Visitor& r_visitor;
		};
		
	template<class InputIterator, class Visitor>
	class RecursiveItemVisitor
		{
		public:
			RecursiveItemVisitor(InputIterator begin, InputIterator end, Visitor& visitor) : 
				r_visitor(visitor)
				{
				m_contexts.push(Context{begin, end});
				}
			
			template<class T>
			void operator()(T const& val)
				{r_visitor.visit(std::pair<std::string const&, T const&>{*r_key, val});}
				
			template<class T>
			void operator()(std::unique_ptr<T> const& val)
				{r_visitor.visit(std::pair<std::string const&, T const&>{*r_key, *val});}
			
			void operator()(std::unique_ptr<Compound> const& val)
				{
				r_visitor.compoundBegin(std::pair<std::string const&, Compound const&>{*r_key, *val});
				m_contexts.push(Context{val->begin(), val->end()});
				}

			void visitItems()
				{
				while(!m_contexts.empty())
					{
					auto& context = m_contexts.top();
					if(context.begin == context.end)
						{
						m_contexts.pop();
						if(!m_contexts.empty())
							{r_visitor.compoundEnd();}
						}
					else
						{
						r_key = &context.begin->first;
						std::visit(*this, context.begin->second);
						++context.begin;
						}
					}
				}
				
		private:
			struct Context
				{
				InputIterator begin;
				InputIterator end;
				};
			std::string const* r_key;
			Visitor& r_visitor;
			std::stack<Context> m_contexts;
		};
		
	template<template<class, class> class VisitorPolicy, class Visitor>
	void Compound::visitItems(Visitor&& visitor) const
		{			
		using std::begin;
		using std::end;
		VisitorPolicy<decltype(begin(m_content)), Visitor>{begin(m_content), end(m_content), visitor}.visitItems();
		}

	
	};

#endif


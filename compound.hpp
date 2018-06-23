//@	{"targets":[{"name": "compound.hpp", "type":"include"}]}

#ifndef DATA_STORE_COMPOUND_HPP
#define DATA_STORE_COMPOUND_HPP

#include "key_not_found_exception.hpp"
#include "type_mismatch_exception.hpp"
#include "var.hpp"
#include "type_names.hpp"

#include <map>
#include <memory>

namespace DataStore
	{
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

			template<class KeyType, class Type>
			bool insert(KeyType&& key, Type&& value)
				{
				auto ip = m_content.insert(std::make_pair(std::forward<KeyType>(key)
					,make_var(std::forward<Type>(value))));
				return ip->second;
				}



			var_t const* find(std::string const& key) const noexcept
				{
				auto i = m_content.find(key);
				if(i == m_content.end())
					{return nullptr;}
				return &i->second;
				}

			var_t* find(std::string const& key) noexcept
				{return const_cast<var_t*>(const_cast<Compound const*>(this)->find(key));}

			auto typeOfValue(std::string const& key) const noexcept
				{
				auto val = find(key);
				if(val == nullptr)
					{return std::variant_npos;}
				return val->index();
				}

			var_t const& get(std::string const& key) const
				{
				auto val = find(key);
				if(val == nullptr)
					{throw KeyNotFoundException(key, sourceLocation());}
				return *val;
				}

			var_t& get(std::string const& key)
				{return const_cast<var_t&>(const_cast<Compound const*>(this)->get(key));}




			template<class Type>
			Type const* getIf(std::string const& key) const noexcept
				{
				auto val = find(key);
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

	template<class T>
	constexpr std::enable_if_t<std::is_same_v<T, std::unique_ptr<Compound>>, char const*> getTypeName() noexcept
		{return getTypeName<Compound>();}
	};

#endif


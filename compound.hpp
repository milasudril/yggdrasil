//@	{"targets":[{"name": "compound.hpp", "type":"include"}]}

#ifndef DATA_STORE_COMPOUND_HPP
#define DATA_STORE_COMPOUND_HPP

#include <variant>
#include <map>
#include <memory>
#include <vector>
#include <type_traits>
#include <cassert>
#include <string>

namespace DataStore
	{
	template<class T, class Enable=void>
	class ValueWrapper
		{
		public:
			template<class... Args>
			ValueWrapper(Args&&... args) : m_value(std::make_unique<T>(std::forward<Args>(args)...))
				{}

			T& get() {return *m_value;}

			T const& get() const {return *m_value;}

		private:
			std::unique_ptr<T> m_value;
		};

	template<class T>
	class ValueWrapper<T, std::enable_if_t<(sizeof(std::variant<T>) <= 32)> >
		{
		public:
			template<class...Args>
			ValueWrapper(Args&&... args) : m_value(std::forward<Args>(args)...)
				{}

			T get() {return m_value;}

			T const& get() const {return m_value;}

		private:
			T m_value;
		};

	template<class ExceptionPolicy, class... Types>
	class Compound
		{
		public:
			Compound(): m_depth{0} {}

			using mapped_type = std::variant<
				ValueWrapper<Compound>
				, ValueWrapper<Types>...
				, ValueWrapper<std::vector<Compound>>
				, ValueWrapper<std::vector<Types>>...
				>;

			using key_type = std::string;


			template<class T>
			T& get(std::string_view key)
				{return const_cast<T&>(const_cast<Compound const*>(this)->get<T>(key));}

			template<class T, class ... Path>
			T& get(std::string_view head, Path ... path)
				{
				auto& next = get<Compound>(head);
				return next.template get<T>(path...);
				}


			template<class T>
			T const& get(std::string_view key) const;

			template<class T, class ... Path>
			T const& get(std::string_view head, Path ... path) const
				{
				auto const& next = get<Compound>(head);
				return next.template get<T>(path...);
				}


			bool exists(std::string_view key) const
				{return m_content.find(key)!=m_content.end();}

			template<class ... Path>
			bool exists(std::string_view head, Path ... path) const
				{
				auto const& next = get<Compound>(head);
				return next.exists(path...);
				}


			template<class T>
			Compound& insert(key_type&& key, T&& value)
				{return insert_impl(std::move(key), std::forward<T>(value));}

			Compound& insert(key_type&& key, Compound&& other)
				{
				auto depth_other = other.depth();
				insert_impl(std::move(key), std::move(other));
				// Compute tree new depth. We add one to other because its new parent will increase the depth by one.
				m_depth = std::max(depth_other + 1, m_depth);
				return *this;
				}

			template<class T>
			Compound& insertOrReplace(key_type&& key, T&& value)
				{
				m_content.insert_or_assign(std::move(key), ValueWrapper<T>{std::forward<T>(value)});
				return *this;
				}

			size_t childCount() const
				{return m_content.size();}

			size_t depth() const
				{return m_depth;}

		private:
			using MapType = std::map<key_type, mapped_type, std::less<>>;
			MapType m_content;
			size_t m_depth;

			template<class T>
			Compound& insert_impl(key_type&& key, T&& value);

		};

	template<class ExceptionPolicy, class... Types>
	template<class T>
	T const& Compound<ExceptionPolicy, Types...>::get(std::string_view key) const
		{
		auto i = m_content.find(key);
		if(i != m_content.end())  // Use this style to trigger warning about noreturn
			{
			auto x = std::get_if<ValueWrapper<T>>(&i->second);
			if( x != nullptr )
				{return x->get();}
			else
				{ExceptionPolicy::template keyValueHasWrongType<T>(key, i->second.index());}
			}
		else
			{ExceptionPolicy::keyNotFound(key);}
		}

	template<class ExceptionPolicy, class... Types>
	template<class T>
	Compound<ExceptionPolicy, Types...>& Compound<ExceptionPolicy, Types...>::insert_impl(key_type&& key, T&& value)
		{
		auto i = m_content.find(key);
		if(i == m_content.end())
			{
			m_content.insert(std::make_pair(std::move(key), ValueWrapper<T>{std::forward<T>(value)}));
			return *this;
			}
		else
			{
			std::visit([key](auto const& var) {ExceptionPolicy::keyAlreadyExists(key, var.get());}, i->second);
			assert(false);
			return *this;
			}
		}
	}

#endif


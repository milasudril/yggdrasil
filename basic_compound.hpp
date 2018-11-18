//@	{"targets":[{"name": "basic_compound.hpp", "type":"include"}]}

#ifndef DATA_STORE_BASIC_COMPOUND_HPP
#define DATA_STORE_BASIC_COMPOUND_HPP

#include <variant>
#include <map>
#include <memory>
#include <vector>
#include <type_traits>
#include <cassert>
#include <string>
#include <algorithm>

namespace DataStore
	{
	template<class T, class Enable=void>
	class ValueWrapper
		{
		public:
			using element_type = T;

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
			using element_type = T;

			template<class...Args>
			ValueWrapper(Args&&... args) : m_value(std::forward<Args>(args)...)
				{}

			T get() {return m_value;}

			T const& get() const {return m_value;}

		private:
			T m_value;
		};

	template<class ExceptionPolicy, class... Types>
	class BasicCompound
		{
		public:
			using mapped_type = std::variant
				<
				  ValueWrapper<BasicCompound>
				, ValueWrapper<Types>...
				, ValueWrapper<std::vector<BasicCompound>>
				, ValueWrapper<std::vector<Types>>...
				>;

			using key_type = typename ExceptionPolicy::key_type; //::string;


			template<class T>
			T& get(std::string_view key)
				{return const_cast<T&>(const_cast<BasicCompound const*>(this)->get<T>(key));}


			template<class T>
			T const& get(std::string_view key) const;


			bool contains(std::string_view key) const
				{return m_content.find(key) != m_content.end();}


			template<class T>
			BasicCompound& insert(key_type&& key, T&& value)
				{return insert_impl(std::move(key), std::forward<T>(value));}

			template<class T>
			BasicCompound& insert(key_type const& key, T&& value)
				{return insert(key_type{key}, std::forward<T>(value));}

			template<class T>
			BasicCompound& insertOrReplace(key_type&& key, T&& value)
				{
				using PlainT = std::remove_reference_t<T>;
				m_content.insert_or_assign(std::move(key), ValueWrapper<PlainT>{std::forward<PlainT>(value)});
				return *this;
				}

			template<class T>
			BasicCompound& insertOrReplace(key_type const& key, T&& value)
				{return insertOrReplace(key_type{key}, std::forward<T>(value));}

			size_t childCount() const
				{return m_content.size();}

			template<class ItemVisitor>
			void visitItems(ItemVisitor&& visitor) const;

			bool erase(key_type const& key)
				{return m_content.erase(key) != 0;}

			void clear()
				{m_content.clear();}

		private:
			using MapType = std::map<key_type, mapped_type, std::less<>>;
			MapType m_content;

			template<class T>
			BasicCompound& insert_impl(key_type&& key, T&& value);

		};

	template<class ExceptionPolicy, class... Types>
	template<class T>
	T const& BasicCompound<ExceptionPolicy, Types...>::get(std::string_view key) const
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
	BasicCompound<ExceptionPolicy, Types...>& BasicCompound<ExceptionPolicy, Types...>::insert_impl(key_type&& key, T&& value)
		{
		auto i = m_content.find(key);
		if(i == m_content.end())
			{
			using PlainT = std::remove_reference_t<T>;
			m_content.insert(std::make_pair(std::move(key), ValueWrapper<PlainT>{std::forward<PlainT>(value)}));
			return *this;
			}
		else
			{
			std::visit([key](auto const& var) {ExceptionPolicy::keyAlreadyExists(key, var.get());}, i->second);
			assert(false);
			return *this;
			}
		}

	template<class ExceptionPolicy, class... Types>
	template<class NodeVisitor>
	void BasicCompound<ExceptionPolicy, Types...>::visitItems(NodeVisitor&& visitor) const
		{
		std::for_each(m_content.begin(), m_content.end(), [&visitor](auto const& item)
			{
			std::visit([&item, &visitor](auto const& val)
				{
				visitor(item.first, val.get());
				}, item.second);
			});
		}




	template<class T, class ExceptionPolicy, class... Types>
	T& get(BasicCompound<ExceptionPolicy, Types...>& compound, std::string_view head)
		{return compound.template get<T>(head);}


	template<class T, class ExceptionPolicy, class... Types, class ... Path>
	T& get(BasicCompound<ExceptionPolicy, Types...>& compound, std::string_view head, Path ... path)
		{
		auto& next = get<BasicCompound<ExceptionPolicy, Types...>>(compound, head);
		return get<T>(next, path...);
		}

	template<class T, class ExceptionPolicy, class... Types>
	T const& get(BasicCompound<ExceptionPolicy, Types...> const& compound, std::string_view head)
		{return compound.template get<T>(head);}


	template<class T, class ExceptionPolicy, class... Types, class ... Path>
	T const& get(BasicCompound<ExceptionPolicy, Types...> const& compound, std::string_view head, Path ... path)
		{
		auto const& next = get<BasicCompound<ExceptionPolicy, Types...>>(compound, head);
		return get<T>(next, path...);
		}


	template<class ExceptionPolicy, class... Types>
	bool contains(BasicCompound<ExceptionPolicy, Types...> const& compound, std::string_view head)
		{return compound.contains(head);}


	template<class ExceptionPolicy, class... Types, class ... Path>
	bool contains(BasicCompound<ExceptionPolicy, Types...> const& compound, std::string_view head, Path ... path)
		{
		if(!contains(compound, head))
			{return false;}

		auto const& next = get<BasicCompound<ExceptionPolicy, Types...>>(compound, head);
		return contains(next, path...);
		}

	template<class ExceptionPolicy, class... Types>
	bool erase(BasicCompound<ExceptionPolicy, Types...>& compound, std::string_view head)
		{return compound.erase(typename BasicCompound<ExceptionPolicy, Types...>::key_type{head});}

	template<class ExceptionPolicy, class... Types, class... Path>
	bool erase(BasicCompound<ExceptionPolicy, Types...>& compound, std::string_view head, Path ... path)
		{
		auto& next = get<BasicCompound<ExceptionPolicy, Types...>>(compound, head);
		return erase(next, path...);
		}

	template<class T, class ExceptionPolicy, class... Types>
	void insert(T&& value, BasicCompound<ExceptionPolicy, Types...>& compound, std::string_view head)
		{compound.template insert<T>(typename BasicCompound<ExceptionPolicy, Types...>::key_type{head}, std::forward<T>(value));}

	template<class T, class ExceptionPolicy, class... Types, class... Path>
	void insert(T&& value, BasicCompound<ExceptionPolicy, Types...>& compound, std::string_view head, Path... path)
		{
		auto& next = get<BasicCompound<ExceptionPolicy, Types...>>(compound, head);
		insert(std::forward<T>(value), next, path...);
		}

	template<class T, class ExceptionPolicy, class... Types>
	void insertOrReplace(T&& value, BasicCompound<ExceptionPolicy, Types...>& compound, std::string_view head)
		{compound.template insertOrReplace<T>(typename BasicCompound<ExceptionPolicy, Types...>::key_type{head}, std::forward<T>(value));}

	template<class T, class ExceptionPolicy, class... Types, class... Path>
	void insertOrReplace(T&& value, BasicCompound<ExceptionPolicy, Types...>& compound, std::string_view head, Path... path)
		{
		auto& next = get<BasicCompound<ExceptionPolicy, Types...>>(compound, head);
		insertOrReplace(std::forward<T>(value), next, path...);
		}
	}
#endif

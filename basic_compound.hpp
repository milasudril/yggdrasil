//@	{"targets":[{"name": "basic_compound.hpp", "type":"include"}]}

#ifndef DATA_STORE_BASIC_COMPOUND_HPP
#define DATA_STORE_BASIC_COMPOUND_HPP

#include "utility.hpp"
#include "analib/typeset/type_set.hpp"

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
	class ValueWrapper<T, std::enable_if_t<(sizeof(std::variant<T>) <= 32) && !Analib::IsContainerOfEmpty<T>::value> >
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

	template<class T>
	class ValueWrapper<T, std::enable_if_t<Analib::IsContainerOfEmpty<T>::value> >
		{
		public:
			ValueWrapper() = delete;
			constexpr bool operator==(ValueWrapper b) const {return true;}
			constexpr bool operator!=(ValueWrapper b) const {return false;}
		};

	template<class T>
	[[nodiscard]] bool operator==(ValueWrapper<T> const& a, ValueWrapper<T> const& b)
		{return a.get() == b.get();}

	template<class T>
	[[nodiscard]] bool operator!=(ValueWrapper<T> const& a, ValueWrapper<T> const& b)
		{return !(a == b);}

	template<class T>
	[[nodiscard]] bool operator<(ValueWrapper<T> const& a, ValueWrapper<T> const& b)
		{return a.get() < b.get();}

	template<class T>
	[[nodiscard]] bool operator>(ValueWrapper<T> const& a, ValueWrapper<T> const& b)
		{return b < a;}

	template<class T>
	[[nodiscard]] bool operator<=(ValueWrapper<T> const& a, ValueWrapper<T> const& b)
		{return !(a > b);}

	template<class T>
	[[nodiscard]] bool operator>=(ValueWrapper<T> const& a, ValueWrapper<T> const& b)
		{return !(a < a);}


	template<class T, class Enable=void>
	class ExceptionPolicyCaller
		{
		public:
			explicit ExceptionPolicyCaller(T const& obj) : r_obj(&obj) {}

			template<class Function, class ... Args>
			[[noreturn]] void call(Function&& f , Args&& ... args) const
				{(r_obj->*f)(std::forward<Args>(args)...);}

		private:
			T const* r_obj;
		};

	template<class T>
	class ExceptionPolicyCaller<T, std::enable_if_t<std::is_empty_v<T>>>
		{
		public:
			template<class Function, class ... Args>
			[[noreturn]] static void call(Function&& f, Args&& ... args)
				{f(std::forward<Args>(args)...);}
		};

	template<class ExceptionPolicy, class KeyType, class... Types>
	class BasicCompound : private ExceptionPolicyCaller<ExceptionPolicy>
		{
		public:
			using ExceptionHandler = ExceptionPolicyCaller<ExceptionPolicy>;

			using mapped_type = std::variant
				<
				  ValueWrapper<Types>...
				, ValueWrapper<BasicCompound>
				, ValueWrapper<std::vector<Types>>...
				, ValueWrapper<std::vector<BasicCompound>>
				>;

			using key_type = KeyType;
			using SupportedTypes = Analib::TypeSet
				<
				  Types...
				, BasicCompound
				, std::vector<Types>...
				, std::vector<BasicCompound>
				>;

			template<class ... ExceptionHandlerArgs>
			explicit BasicCompound(ExceptionHandlerArgs&& ... args) :
				ExceptionHandler(std::forward<ExceptionHandlerArgs>(args)...) {}

			template<class T, class KeyLike>
			[[nodiscard]] T& get(KeyLike const& key)
				{return const_cast<T&>(const_cast<BasicCompound const*>(this)->get<T>(key));}


			template<class T, class KeyLike>
			[[nodiscard]] T const& get(KeyLike const& key) const;


			template<class KeyLike>
			[[nodiscard]] bool contains(KeyLike const& key) const
				{return m_content.find(key) != m_content.end();}


			template<class T>
			BasicCompound& insert(key_type&& key, T&& value) &
				{return insert_impl(std::move(key), std::forward<T>(value));}

			template<class T>
			[[nodiscard]] BasicCompound&& insert(key_type&& key, T&& value) &&
				{return std::move(insert_impl(std::move(key), std::forward<T>(value)));}

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

			[[nodiscard]] size_t childCount() const
				{return m_content.size();}

			template<class ItemVisitor>
			bool visitItems(ItemVisitor&& visitor) const;

			template<class KeyLike>
			bool erase(KeyLike const& key)
				{return m_content.erase(key) != 0;}

			void clear()
				{m_content.clear();}

			[[nodiscard]] bool operator==(BasicCompound const& other) const
				{return m_content == other.m_content;}

			[[nodiscard]] bool operator!=(BasicCompound const& other) const
				{return m_content != other.m_content;}

		private:
			using MapType = std::map<key_type, mapped_type, std::less<>>;
			MapType m_content;

			template<class T>
			BasicCompound& insert_impl(key_type&& key, T&& value);

			template<class KeyLike>
			auto __attribute__((noinline)) find(KeyLike const& key) const
				{return m_content.find(key);}

			void __attribute__((noinline)) throwIfFound(key_type const& key) const
				{
				auto i = m_content.find(key);
				if(i != m_content.end())
					{ExceptionHandler::call(&ExceptionPolicy::keyAlreadyExists,key);}
				}

			template<class KeyLike>
			auto __attribute__((noinline)) throwIfNotFound(KeyLike const& key) const
				{
				auto i = m_content.find(key);
				if(i == m_content.end())
					{ExceptionHandler::call(&ExceptionPolicy::keyNotFound,key);}
				else
					{return i;}
				}
		};


	template<class ExceptionPolicy, class KeyType, class... Types>
	template<class T, class KeyLike>
	[[nodiscard]] T const& BasicCompound<ExceptionPolicy, KeyType, Types...>::get(KeyLike const& key) const
		{
		auto i = throwIfNotFound(key);
		auto x = std::get_if<ValueWrapper<T>>(&i->second);
		if( x != nullptr )
			{return x->get();}
		else
			{ExceptionHandler::call(&ExceptionPolicy::keyValueHasWrongType, key, i->second.index());}
		}

	template<class ExceptionPolicy, class KeyType, class... Types>
	template<class T>
	[[nodiscard]] BasicCompound<ExceptionPolicy, KeyType, Types...>& BasicCompound<ExceptionPolicy, KeyType, Types...>::insert_impl
		(key_type&& key, T&& value)
		{
		throwIfFound(key);
		using PlainT = std::remove_reference_t<T>;
		m_content.insert(std::make_pair(std::move(key), ValueWrapper<PlainT>{std::forward<PlainT>(value)}));
		return *this;
		}

	template<class ExceptionPolicy, class KeyType, class... Types>
	template<class NodeVisitor>
	bool BasicCompound<ExceptionPolicy, KeyType, Types...>::visitItems(NodeVisitor&& visitor) const
		{
		return std::find_if_not(m_content.begin(), m_content.end(), [&visitor](auto const& item)
			{
			return std::visit([&item, &visitor](auto const& val)
				{
				using T = std::decay_t<decltype(val)>;
				if constexpr(std::is_empty_v<T>)
					{return false;}
				else
					{return visitor(item.first, val.get());}
				}, item.second);
			}) == m_content.end();
		}

	template<class T, class ExceptionPolicy, class KeyType, class... Types, class KeyLike>
	[[nodiscard]] T& get(BasicCompound<ExceptionPolicy, KeyType, Types...>& compound, KeyLike const& head)
		{return compound.template get<T>(head);}


	template<class T, class ExceptionPolicy, class KeyType, class... Types, class KeyLike, class ... Path>
	[[nodiscard]] T& get(BasicCompound<ExceptionPolicy, Types...>& compound, KeyLike const& head, Path ... path)
		{
		auto& next = get<BasicCompound<ExceptionPolicy, KeyType, Types...>>(compound, head);
		return get<T>(next, path...);
		}

	template<class T, class ExceptionPolicy, class KeyType, class... Types, class KeyLike>
	[[nodiscard]] T const& get(BasicCompound<ExceptionPolicy, KeyType, Types...> const& compound, KeyLike const& head)
		{return compound.template get<T>(head);}


	template<class T, class ExceptionPolicy, class KeyType, class... Types, class KeyLike, class ... Path>
	[[nodiscard]] T const& get(BasicCompound<ExceptionPolicy, KeyType, Types...> const& compound, KeyLike const& head
		, Path ... path)
		{
		auto const& next = get<BasicCompound<ExceptionPolicy, KeyType, Types...>>(compound, head);
		return get<T>(next, path...);
		}

	template<class ExceptionPolicy, class KeyType, class ... Types, class KeyLike>
	[[nodiscard]] bool contains(BasicCompound<ExceptionPolicy, KeyType, Types...> const& compound, KeyLike const& head)
		{return compound.contains(head);}


	template<class ExceptionPolicy, class KeyType, class ... Types, class KeyLike, class ... Path>
	[[nodiscard]] bool contains(BasicCompound<ExceptionPolicy, KeyType, Types...> const& compound, KeyLike const& head, Path ... path)
		{
		if(!contains(compound, head))
			{return false;}

		auto const& next = get<BasicCompound<ExceptionPolicy, KeyType, Types...>>(compound, head);
		return contains(next, path...);
		}


	template<class ExceptionPolicy, class KeyType, class... Types, class KeyLike>
	bool erase(BasicCompound<ExceptionPolicy, KeyType, Types...>& compound, KeyLike const& head)
		{return compound.erase(typename BasicCompound<ExceptionPolicy, KeyType, Types...>::key_type{head});}

	template<class ExceptionPolicy, class KeyType, class... Types, class... Path, class KeyLike>
	bool erase(BasicCompound<ExceptionPolicy, KeyType, Types...>& compound, KeyLike const& head, Path ... path)
		{
		auto& next = get<BasicCompound<ExceptionPolicy, KeyType, Types...>>(compound, head);
		return erase(next, path...);
		}

	template<class T, class ExceptionPolicy, class KeyType, class... Types, class KeyLike>
	void insert(T&& value, BasicCompound<ExceptionPolicy, KeyType, Types...>& compound, KeyLike const& head)
		{
		compound.template insert<T>(typename BasicCompound<ExceptionPolicy, KeyType, Types...>::key_type{head}
			, std::forward<T>(value));
		}

	template<class T, class ExceptionPolicy, class KeyType, class... Types, class KeyLike, class... Path>
	void insert(T&& value, BasicCompound<ExceptionPolicy, KeyType, Types...>& compound, KeyLike const& head
		, Path... path)
		{
		auto& next = get<BasicCompound<ExceptionPolicy, KeyType, Types...>>(compound, head);
		insert(std::forward<T>(value), next, path...);
		}

	template<class T, class ExceptionPolicy, class KeyType, class... Types, class KeyLike>
	void insertOrReplace(T&& value, BasicCompound<ExceptionPolicy, KeyType, Types...>& compound, KeyLike const& head)
		{
		compound.template insertOrReplace<T>(typename BasicCompound<ExceptionPolicy, KeyType, Types...>::key_type{head}
			, std::forward<T>(value));
		}

	template<class T, class ExceptionPolicy, class KeyType, class... Types, class KeyLike, class... Path>
	void insertOrReplace(T&& value, BasicCompound<ExceptionPolicy, KeyType, Types...>& compound, KeyLike const& head
		, Path... path)
		{
		auto& next = get<BasicCompound<ExceptionPolicy, KeyType, Types...>>(compound, head);
		insertOrReplace(std::forward<T>(value), next, path...);
		}



	template<class ExceptionPolicy, class KeyType, class... Types, class Serializer>
	[[nodiscard]] inline auto store(BasicCompound<ExceptionPolicy, KeyType, Types...> const& val, Serializer&& serializer)
		{return serializer(val);}

	template<class ExceptionPolicy, class KeyType, class... Types, class Deserializer>
	[[nodiscard]] inline auto load(BasicCompound<ExceptionPolicy, KeyType, Types...>& val, Deserializer&& deserializer)
		{return deserializer(val);}
	}

#endif

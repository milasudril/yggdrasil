//@	{"targets":[{"name":"key_type_count_value_serializer.hpp","type":"include"}]}

#ifndef DATA_STORE_KEY_TYPE_COUNT_VALUE_SERIALIZER_HPP
#define DATA_STORE_KEY_TYPE_COUNT_VALUE_SERIALIZER_HPP

#include "utility.hpp"
#include "key_type_count_value_defs.hpp"
#include "analib/typeset/type_set.hpp"

#include <cstdint>

namespace DataStore
	{
	namespace detail
		{
		template<class Compound, class Writer>
		class KeyTypeCountValueSerializerSelector
			{
			public:
				explicit KeyTypeCountValueSerializerSelector(Writer&& sink)
					: r_sink(std::forward<Writer>(sink)) {}
				using SupportedTypes = typename Compound::SupportedTypes;

				template<class Value>
				std::enable_if_t<IsPod<Value>::value, bool> operator()(KeyTypeCountValueDefs::KeyType key, Value item)
					{
					writeHeader<Value>(key);
					return r_sink.write(item);
					}

				template<class Value>
				std::enable_if_t<IsSimpleArray<Value>::value, bool> operator()(KeyTypeCountValueDefs::KeyType key
					, Value const& item)
					{
					if(unlikely(!writeHeader<Value>(key)))
						{return false;}

					if(unlikely(!r_sink.write(static_cast<KeyTypeCountValueDefs::ArraySize>(item.size()))))
						{return false;}

					return r_sink.write(item.data(), item.size());
					}

				bool operator()(KeyTypeCountValueDefs::KeyType key, Compound const& item)
					{
					if(unlikely(!writeHeader<Compound>(key)))
						{return false;}
					if(unlikely(!r_sink.write(static_cast<KeyTypeCountValueDefs::ArraySize>(item.childCount()))))
						{return false;}
					return item.visitItems(*this);
					}

				template<template<class> class Sequence>
				std::enable_if_t<IsSequenceOf<Sequence<Compound>, Compound>::value, bool>
				operator()(KeyTypeCountValueDefs::KeyType key, Sequence<Compound> const& item)
					{
					if(unlikely(!writeHeader<Sequence<Compound>>(key)))
						{return false;}

					if(unlikely(!r_sink.write(static_cast<KeyTypeCountValueDefs::ArraySize>(item.size()))))
						{return false;}

					return std::find_if_not(item.begin(), item.end(), [this](auto const& val)
						{
						if(unlikely(!r_sink.write(static_cast<KeyTypeCountValueDefs::ArraySize>(val.childCount()))))
							{return false;}
						return val.visitItems(*this);
						}) == item.end();
					}

				template<template<class> class Sequence, class SimpleArray>
				std::enable_if_t<IsSimpleArray<SimpleArray>::value
					&& IsSequenceOf<Sequence<SimpleArray>, SimpleArray>::value, bool>
				operator()(KeyTypeCountValueDefs::KeyType key, Sequence<SimpleArray> const& item)
					{
					if(unlikely(!writeHeader<Sequence<SimpleArray>>(key)))
						{return false;}
					if(unlikely(!r_sink.write(static_cast<KeyTypeCountValueDefs::ArraySize>(item.size()))))
						{return false;}
					return std::find_if_not(item.begin(), item.end(), [this](SimpleArray const& value)
						{
						if(unlikely(!r_sink.write(static_cast<KeyTypeCountValueDefs::ArraySize>(value.size()))))
							{return false;}
						return r_sink.write(value.data(), value.size());
						}) == item.end();
					}

				template<class T>
				std::enable_if_t<Analib::IsContainerOfEmpty<T>::value, bool>
				operator()(KeyTypeCountValueDefs::KeyType, T) = delete;

			private:
				template<class Value>
				bool writeHeader(KeyTypeCountValueDefs::KeyType key)
					{
					auto type_id = KeyTypeCountValueDefs::TypeId::create<SupportedTypes::template getTypeIndex<Value>()>();

					if(unlikely(!r_sink.write(key)))
						{return false;}
					return r_sink.write(type_id);
					}

				Writer r_sink;
			};
		}

	template<class Sink>
	class KeyTypeCountValueSerializer
		{
		public:
			KeyTypeCountValueSerializer(Sink&& sink) :
				  m_sink(std::forward<Sink>(sink))
				{}

			template<class Compound>
			[[nodiscard]] bool operator()(Compound const& val)
				{
				if(unlikely(!m_sink.write(static_cast<KeyTypeCountValueDefs::ArraySize>(val.childCount()))))
					{return false;}
				return val.visitItems(detail::KeyTypeCountValueSerializerSelector<Compound, Sink>{std::move(m_sink)});
				}

		private:
			Sink m_sink;
		};

	}
#endif

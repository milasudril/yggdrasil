//@	{"targets":[{"name":"key_type_count_value_serializer.hpp","type":"include"}]}

#ifndef DATA_STORE_KEY_TYPE_COUNT_VALUE_SERIALIZER_HPP
#define DATA_STORE_KEY_TYPE_COUNT_VALUE_SERIALIZER_HPP

#include "utility.hpp"
#include "key_type_count_value_defs.hpp"

#include <cstdint>

namespace DataStore
	{
	template<class Compound, class Writer>
	class KeyTypeCountValueSerializer
		{
		public:
			explicit KeyTypeCountValueSerializer(Writer&& sink)
				: m_sink(std::forward<Writer>(sink)) {}
			using SupportedTypes = typename Compound::SupportedTypes;

			template<class Value>
			std::enable_if_t<std::is_arithmetic_v<Value>> operator()(KeyTypeCountValueDefs::KeyType key, Value item)
				{
				writeHeader<Value>(key);
				m_sink.write(item);
				}

			template<class Value>
			std::enable_if_t<IsSimpleArray<Value>::value> operator()(KeyTypeCountValueDefs::KeyType key, Value const& item)
				{
				writeHeader<Value>(key);
				m_sink.write(static_cast<KeyTypeCountValueDefs::ArraySize>(item.size()))
					.write(item.data(), item.size());
				}

			void operator()(KeyTypeCountValueDefs::KeyType key, Compound const& item)
				{
				writeHeader<Compound>(key);
				m_sink.write(static_cast<KeyTypeCountValueDefs::ArraySize>(item.childCount()));
				item.visitItems(*this);
				}

			template<template<class> class Sequence>
			std::enable_if_t<IsSequenceOf<Sequence<Compound>, Compound>::value>
			operator()(KeyTypeCountValueDefs::KeyType key, Sequence<Compound> const& item)
				{
				writeHeader<Sequence<Compound>>(key);
				m_sink.write(static_cast<KeyTypeCountValueDefs::ArraySize>(item.size()));
				std::for_each(item.begin(), item.end(), [*this](auto const& val) mutable
					{
					m_sink.write(static_cast<KeyTypeCountValueDefs::ArraySize>(val.childCount()));
					val.visitItems(*this);
					});
				}

			template<template<class> class Sequence, class SimpleArray>
			std::enable_if_t<IsSimpleArray<SimpleArray>::value && IsSequenceOf<Sequence<SimpleArray>, SimpleArray>::value>
			operator()(KeyTypeCountValueDefs::KeyType key, Sequence<SimpleArray> const& item)
				{
				writeHeader<Sequence<SimpleArray>>(key);
				m_sink.write(static_cast<KeyTypeCountValueDefs::ArraySize>(item.size()));
				std::for_each(item.begin(), item.end(), [this](SimpleArray const& value)
					{
					m_sink.write(static_cast<KeyTypeCountValueDefs::ArraySize>(value.size()))
						.write(value.data(), value.size());
					});
				}

		private:
			template<class Value>
			void writeHeader(KeyTypeCountValueDefs::KeyType key)
				{
				auto type_id = KeyTypeCountValueDefs::TypeId::create<SupportedTypes::template getTypeIndex<Value>()>();

				m_sink.write(key)
					.write(type_id);
				}

			Writer m_sink;
		};
	}
#endif

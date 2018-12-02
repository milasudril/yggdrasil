//@	{"targets":[{"name":"key_type_count_value_writer.hpp","type":"include"}]}

#ifndef DATA_STORE_KEY_TYPE_COUNT_VALUE_WRITER_HPP
#define DATA_STORE_KEY_TYPE_COUNT_VALUE_WRITER_HPP

#include "utility.hpp"
#include "key_type_count_value_defs.hpp"

#include <cstdint>

namespace DataStore
	{
	template<class Compound, class Writer>
	class KeyTypeCountValueWriter
		{
		public:
			explicit KeyTypeCountValueWriter(Writer&& sink) : r_sink(sink) {}
			using SupportedTypes = typename Compound::SupportedTypes;

			template<class Value>
			std::enable_if_t<std::is_arithmetic_v<Value>> operator()(KeyTypeCountValue::KeyType key, Value item)
				{
				writeHeader<Value>(key);
				r_sink.write(item);
				}

			template<class Value>
			std::enable_if_t<IsSimpleArray<Value>::value> operator()(KeyTypeCountValue::KeyType key, Value const& item)
				{
				writeHeader<Value>(key);
				r_sink.write(static_cast<KeyTypeCountValue::ArraySize>(item.size()))
					.write(item.data(), item.size());
				}

			void operator()(KeyTypeCountValue::KeyType key, Compound const& item)
				{
				writeHeader<Compound>(key);
				r_sink.write(static_cast<KeyTypeCountValue::ArraySize>(item.childCount()));
				item.visitItems(*this);
				}

			template<template<class> class Sequence>
			std::enable_if_t<IsSequenceOf<Sequence<Compound>, Compound>::value>
			operator()(KeyTypeCountValue::KeyType key, Sequence<Compound> const& item)
				{
				writeHeader<Sequence<Compound>>(key);
				r_sink.write(static_cast<KeyTypeCountValue::ArraySize>(item.size()));
				std::for_each(item.begin(), item.end(), [*this](auto const& val) mutable
					{
					r_sink.write(static_cast<KeyTypeCountValue::ArraySize>(val.childCount()));
					val.visitItems(*this);
					});
				}

			template<template<class> class Sequence, class SimpleArray>
			std::enable_if_t<IsSimpleArray<SimpleArray>::value && IsSequenceOf<Sequence<SimpleArray>, SimpleArray>::value>
			operator()(KeyTypeCountValue::KeyType key, Sequence<SimpleArray> const& item)
				{
				writeHeader<Sequence<SimpleArray>>(key);
				r_sink.write(static_cast<KeyTypeCountValue::ArraySize>(item.size()));
				std::for_each(item.begin(), item.end(), [this](SimpleArray const& value)
					{
					r_sink.write(static_cast<KeyTypeCountValue::ArraySize>(value.size()))
						.write(value.data(), value.size());
					});
				}

		private:
			template<class Value>
			void writeHeader(KeyTypeCountValue::KeyType key)
				{
				auto type_id = KeyTypeCountValue::TypeId::create<SupportedTypes::template getTypeIndex<Value>()>();

				r_sink.write(key)
					.write(type_id);
				}

			Writer& r_sink;
		};
	}
#endif

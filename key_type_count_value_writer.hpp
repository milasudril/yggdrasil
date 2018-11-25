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
				r_sink.write(key)
					.write(SupportedTypes::template getTypeIndex<Value>())
					.write(item);
				}

			template<class Value>
			std::enable_if_t<IsSimpleArray<Value>::value> operator()(KeyTypeCountValue::KeyType key, Value const& item)
				{
				r_sink.write(key)
					.write(SupportedTypes::template getTypeIndex<Value>())
					.write(static_cast<uint64_t>(item.size()))
					.write(item.data(), item.size());
				}

			void operator()(KeyTypeCountValue::KeyType key, Compound const& item)
				{
			//	puts(key.begin());
				}

			template<template<class> class Sequence>
			std::enable_if_t<IsSequenceOf<Sequence<Compound>, Compound>::value>
			operator()(KeyTypeCountValue::KeyType key, Sequence<Compound> const&)
				{
			//	puts(key.begin());
				}

			template<template<class> class Sequence, class SimpleArray>
			std::enable_if_t<IsSimpleArray<SimpleArray>::value && IsSequenceOf<Sequence<SimpleArray>, SimpleArray>::value>
			operator()(KeyTypeCountValue::KeyType key, Sequence<SimpleArray> const& item)
				{
				r_sink.write(key)
					.write(SupportedTypes::template getTypeIndex<Sequence<SimpleArray>>())
					.write(static_cast<uint64_t>(item.size()));
				std::for_each(item.begin(), item.end(), [this](SimpleArray const& value)
					{
					r_sink.write(static_cast<uint64_t>(value.size()))
						.write(value.data(), value.size());
					});
				}

		private:
			Writer& r_sink;
		};
	}
#endif

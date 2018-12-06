//@	{"targets":[{"name":"key_type_count_value_deserializer.hpp","type":"include"}]}

#ifndef DATA_STORE_KEY_TYPE_COUNT_VALUE_DESERIALIZER_HPP
#define DATA_STORE_KEY_TYPE_COUNT_VALUE_DESERIALIZER_HPP

#include "utility.hpp"
#include "key_type_count_value_defs.hpp"

#include "analib/typeset/type_set.hpp"

#include <cstdint>
#include <vector>

namespace DataStore
	{
	enum class StatusCode:int{Success, UnknownType, EndOfFile, TreeTooDeep};

	[[nodiscard]] inline bool readFailed(StatusCode code)
		{return code!=StatusCode::Success;}

	template<class Source>
	class KeyTypeCountValueDeserializer
		{
		public:
			KeyTypeCountValueDeserializer(Source&& source) : m_source(std::forward<Source>(source)) {}

			template<class Compound>
			[[nodiscard]] StatusCode operator()(Compound& val);

		private:
			Source m_source;
		};


	namespace detail
		{
		template<class Source, class Compound, class Deserializer>
		class KeyTypeCountValueDeserializerSelector
			{
			public:
				explicit KeyTypeCountValueDeserializerSelector(Source& source
					, KeyTypeCountValueDefs::KeyType key_current
					, Compound& sink
					, Deserializer& deserializer):
					  r_source(source)
					, m_key_current(key_current)
					, r_sink(sink)
					, r_deserializer(deserializer)
					{}

				[[nodiscard]] constexpr auto operator()()
					{return StatusCode::UnknownType;}

				template<class T>
				[[nodiscard]] std::enable_if_t<std::is_arithmetic_v<T>, StatusCode> operator()(Analib::Empty<T>)
					{
					T ret{};
					if(unlikely(!r_source.read(ret)))
						{return StatusCode::EndOfFile;}

					r_sink.insert(m_key_current, std::move(ret));

					return StatusCode::Success;
					}

				template<class T>
				[[nodiscard]] std::enable_if_t<IsSimpleArray<T>::value, StatusCode> operator()(Analib::Empty<T>)
					{
					KeyTypeCountValueDefs::ArraySize size{0};
					if(unlikely(!r_source.read(size)))
						{return StatusCode::EndOfFile;}

					T val{};
					val.reserve(size);

					if(unlikely(!r_source.read(val.data(), size)))
						{return StatusCode::EndOfFile;}

					r_sink.insert(m_key_current, std::move(val));

					return StatusCode::Success;
					}

				template<template<class> class Sequence, class SimpleArray>
				std::enable_if_t<IsSimpleArray<SimpleArray>::value && IsSequenceOf<Sequence<SimpleArray>, SimpleArray>::value, StatusCode>
				operator()(Analib::Empty<Sequence<SimpleArray>>)
					{
					KeyTypeCountValueDefs::ArraySize size{0};
					if(unlikely(!r_source.read(size)))
						{return StatusCode::EndOfFile;}
					Sequence<SimpleArray> seq;
					seq.reserve(size);
					while(size != 0)
						{
						KeyTypeCountValueDefs::ArraySize array_size{0};
						if(unlikely(!r_source.read(array_size)))
							{return StatusCode::EndOfFile;}

						SimpleArray val;
						val.reserve(array_size);
						if(unlikely(!r_source.read(val.data(), val.size())))
							{return StatusCode::EndOfFile;}
						seq.push_back(std::move(val));
						--size;
						}
					r_sink.insert(m_key_current, std::move(seq));
					return StatusCode::Success;
					}

				[[nodiscard]] StatusCode operator()(Analib::Empty<Compound>)
					{
					Compound val;
					auto result = r_deserializer(val);
					if(unlikely(readFailed(result)))
						{return result;}

					r_sink.insert(m_key_current, std::move(val));

					return result;
					}


				template<template<class> class Sequence>
				[[nodiscard]] std::enable_if_t<IsSequenceOf<Sequence<Compound>, Compound>::value, StatusCode>
				operator()(Analib::Empty<Sequence<Compound>>)
					{
					KeyTypeCountValueDefs::ArraySize size{0};
					if(unlikely(!r_source.read(size)))
						{return StatusCode::EndOfFile;}
					Sequence<Compound> seq;
					seq.reserve(size);
					while(size != 0)
						{
						Compound val;
						auto result = r_deserializer(val);
						if(unlikely(readFailed(result)))
							{return result;}
						seq.push_back(std::move(val));
						--size;
						}
					r_sink.insert(m_key_current, std::move(seq));
					return StatusCode::Success;
					}

			private:
				Source& r_source;
				KeyTypeCountValueDefs::KeyType m_key_current;
				Compound& r_sink;
				Deserializer& r_deserializer;
			};
		}


	template<class Source>
	template<class Compound>
	[[nodiscard]] StatusCode KeyTypeCountValueDeserializer<Source>::operator()(Compound& val)
		{
		KeyTypeCountValueDefs::ArraySize size{0};
		if(unlikely(!m_source.read(size)))
			{return StatusCode::EndOfFile;}

		while(unlikely(size != 0))
			{
			KeyTypeCountValueDefs::KeyType key;
			if(unlikely(!m_source.read(key)))
				{return StatusCode::EndOfFile;}

			KeyTypeCountValueDefs::TypeId type_id;
			if(unlikely(!m_source.read(key)))
				{return StatusCode::EndOfFile;}

			auto status = Compound::SupportedTypes::select(type_id
				, detail::KeyTypeCountValueDeserializerSelector{m_source, key, val, *this});
			if(unlikely(readFailed(status)))
				{return status;}

			--size;
			}

		return StatusCode::Success;
		}
	}

#endif

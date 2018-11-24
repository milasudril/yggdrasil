//@	{"targets":[{"name":"compound_decoder.hpp","type":"include"}]}


#ifndef DATA_STORE_COMPOUND_DECODER_HPP
#define DATA_STORE_COMPOUND_DECODER_HPP

#include "utility.hpp"
#include "analib/inlinestring/inlinestring.hpp"

#include <cstdint>
#include <vector>

namespace DataStore
	{
	enum class StatusCode:int{Success, UnknownType, EndOfFile};

	[[nodiscard]] inline bool readFailed(StatusCode code)
		{return code!=StatusCode::Success;}


	template<class Source, class Compound>
	[[nodiscard]] StatusCode read(Source&& source, Compound& val);

	template<class Source, class Compound>
	class BinaryCompoundDecoder
		{
		public:
			explicit BinaryCompoundDecoder(Source& src
				, Compound& dest
				, Analib::InlineString<char, 16> key_current):
				  r_source(src)
				, r_sink(dest)
				, m_key_current(key_current)
				{}

			using SupportedTypes = typename Compound::SupportedTypes;
			static constexpr auto UnknownType = GetSize<SupportedTypes>::value;

			[[nodiscard]] std::pair<size_t, StatusCode> readType()
				{
				uint64_t type{UnknownType};
				if(unlikely(!r_source.read(type)))
					{return std::make_pair(type, StatusCode::EndOfFile);}
				return std::make_pair(type, StatusCode::Success);
				}

			[[nodiscard]] static constexpr auto unknownType()
				{return StatusCode::UnknownType;}



			template<class T>
			[[nodiscard]]
			std::enable_if_t<std::is_arithmetic_v<T>, StatusCode> read(Empty<T>)
				{
				T ret{};
				if(unlikely(!r_source.read(ret)))
					{return StatusCode::EndOfFile;}

				r_sink.insert(m_key_current, std::move(ret));

				return StatusCode::Success;
				}

			template<class T>
			[[nodiscard]]
			std::enable_if_t<IsSimpleArray<T>::value, StatusCode> read(Empty<T>)
				{
				uint64_t size{0};
				if(unlikely(!r_source.read(size)))
					{return StatusCode::EndOfFile;}

				T val{};
				val.reserve();

				if(unlikely(!r_source.read(val.data(), size)))
					{return StatusCode::EndOfFile;}

				r_sink.insert(m_key_current, std::move(val));

				return StatusCode::Success;
				}

			[[nodiscard]] StatusCode read(Empty<Compound>)
				{
				Compound val;
				auto result = DataStore::read(r_source, val);
				if(unlikely(readFailed(result)))
					{return result;}

				r_sink.insert(m_key_current, std::move(val));

				return StatusCode::UnknownType;
				}
			[[nodiscard]] StatusCode read(Empty<std::vector<Compound>>) {return StatusCode::UnknownType;}


		private:
			Source& r_source;
			Compound& r_sink;
			Analib::InlineString<char, 16> const m_key_current;

		};


	template<class Source, class Compound>
	[[nodiscard]] StatusCode read(Source&& source, Compound& val)
		{
		uint64_t size{0};
		if(unlikely(!source.read(size)))
			{return StatusCode::EndOfFile;}

		while(unlikely(size!=0))
			{
			Analib::InlineString<char, 16> key_next;
			if(unlikely(!source.read(key_next)))
				{return StatusCode::EndOfFile;}

			auto result = readRecord(BinaryCompoundDecoder{source, val, key_next});
			if(unlikely(readFailed(result)))
				{return result;}

			--size;
			}

		return StatusCode::Success;
		}
	}

#endif

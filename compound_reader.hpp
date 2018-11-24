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

	bool readFailed(StatusCode code)
		{return code!=StatusCode::Success;}

	template<class Source, class Compound>
	class BinaryCompoundDecoder
		{
		public:
			explicit BinaryCompoundDecoder(Source&& src, Compound& dest):r_source(src), r_sink(dest)
				{}

			explicit BinaryCompoundDecoder(Source&& src, Compound&& dest):r_source(src), r_sink(dest)
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

				r_current_compound->insert(m_current_key, std::move(ret));

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

				r_current_compound->insert(m_current_key, std::move(val));

				return StatusCode::Success;
				}

			[[nodiscard]] StatusCode read(Empty<Compound>)
				{
				uint64_t size{0};
				if(unlikely(!r_source.read(size)))
					{return StatusCode::EndOfFile;}

				Compound val;
				while(unlikely(size!=0))
					{
//					readRecord(*this);
					--size;
					}

				r_current_compound->insert(m_current_key, std::move(val));

				return StatusCode::UnknownType;
				}
			[[nodiscard]] StatusCode read(Empty<std::vector<Compound>>) {return StatusCode::UnknownType;}


		private:
			Source& r_source;
			Compound& r_sink;


			Compound* r_current_compound;
			Analib::InlineString<char, 16> m_current_key;

		};
	}

#endif

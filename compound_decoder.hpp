//@	{"targets":[{"name":"compound_decoder.hpp","type":"include"}]}


#ifndef DATA_STORE_COMPOUND_DECODER_HPP
#define DATA_STORE_COMPOUND_DECODER_HPP

#include "analib/inlinestring/inlinestring.hpp"

namespace DataStore
	{
	enum class StatusCode:int{Success, UnknownType, EndOfFile};

	bool readFailed(StatusCode code)
		{return code!=StatusCode::Success;}

	template<class Compound>
	class BinaryCompoundDecoder
		{
		public:
			using TypeId = Analib::InlineString<char, 8>;
			using StatusCode = DataStore::StatusCode;
			std::pair<TypeId, StatusCode> getNextType();

			static constexpr intptr_t hash(TypeId type);

			using SupportedTypes = typename Compound::SupportedTypes;

			static constexpr auto unknownType()
				{return StatusCode::UnknownType;}


			template<class T>
			StatusCode decode(Empty<T>)
				{return StatusCode::Success;}


		private:

		};
	}

#endif

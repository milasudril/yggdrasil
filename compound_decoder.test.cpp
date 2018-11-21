//@	{"targets":[{"name":"compound_decoder.test","type":"application","autorun":1}]}

#include "compound_decoder.hpp"
#include "value_decoder.hpp"
#include "basic_compound.hpp"

template<class DecoderPolicy>
auto decode(DecoderPolicy&& decoder)
	{
	while(true)
		{
		auto [type_id, status] = decoder.getNextType();
		if(readFailed(status))
			{return status;}

		status = decode(decoder, type_id);
		if(readFailed(status))
			{return status;}
		}
	}

struct MyExceptionPolicy
	{
	[[noreturn]]
	static void keyNotFound(std::string_view key)
		{throw std::string{key};}

	template<class T>
	[[noreturn]]
	static void keyValueHasWrongType(std::string_view key, size_t actualType)
		{throw std::string{key};}

	template<class T>
	[[noreturn]]
	static void keyAlreadyExists(std::string const& key, T const& value)
		{throw key;}

	};

using Compound = DataStore::BasicCompound<MyExceptionPolicy, std::string, std::string, int>;

int main()
	{
	DataStore::BinaryCompoundDecoder<Compound> decoder;
	decode(decoder);
	}

//@	{"targets":[{"name":"compound.test","type":"application", "autorun":1}]}

#include "compound.hpp"
#include "native_decoder.hpp"
#include "posix_file_reader.hpp"
#include "key_type_count_value_deserializer.hpp"

#include "native_encoder.hpp"
#include "mem_writer.hpp"
#include "key_type_count_value_serializer.hpp"

struct MyExceptionPolicy
	{
	[[noreturn]]
	static void keyNotFound(std::string_view key)
		{throw std::string(key);}

	[[noreturn]]
	static void keyValueHasWrongType(std::string_view key, size_t actualType)
		{throw std::string(key);}

	[[noreturn]]
	static void keyAlreadyExists(Yggdrasil::KeyType const& key)
		{throw key;}
	};

int main()
	{
	Yggdrasil::Compound<MyExceptionPolicy> test;
	DataStore::PosixFileReader fileIn(__FILE__);
	assert(load(test, fileIn) == Yggdrasil::StatusCode::Success);
	printf("%zu\n", test.childCount());

	test.insert(Yggdrasil::KeyType("Foobar"), Yggdrasil::String("Hello"));

	std::vector<std::byte> buffer;
	DataStore::MemWriter writer{buffer};
	assert(store(test, writer));

	return 0;
	}

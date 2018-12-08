//@	{"targets":[{"name":"key_type_count_value_deserializer.test","type":"application","autorun":1}]}

#include "key_type_count_value_deserializer.hpp"

#include "test/compound_test.hpp"
#include "mem_reader.hpp"
#include "native_decoder.hpp"

#include <type_traits>

int main()
	{
	Test::Compound compound;
	DataStore::MemReader memReader
		{
		  reinterpret_cast<std::byte const*>(std::begin(Test::data_le))
		, reinterpret_cast<std::byte const*>(std::end(Test::data_le))
		};

	auto status = DataStore::KeyTypeCountValueDeserializer{DataStore::NativeDecoder{memReader}}(compound);
	assert(status == DataStore::StatusCode::Success);
	assert(memReader.eof());

	assert(compound.childCount() == 3);


	assert(DataStore::get<int>(compound, DataStore::KeyTypeCountValueDefs::KeyType{"A"}
		,DataStore::KeyTypeCountValueDefs::KeyType{"a1"})==0x4030201);
	assert(DataStore::get<std::string>(compound, DataStore::KeyTypeCountValueDefs::KeyType{"A"}
		,DataStore::KeyTypeCountValueDefs::KeyType{"a2"}) == "Foobar");

		{
		auto& v = DataStore::get<std::vector<Test::Compound>>(compound, DataStore::KeyTypeCountValueDefs::KeyType{"B"});
		assert(v.size() == 2);
		assert(v[0].get<int>(DataStore::KeyTypeCountValueDefs::KeyType{"B01"}) == 0x4030202);
		assert(v[1].childCount() == 0);
		}

		{
		auto& v = DataStore::get<std::vector<std::string>>(compound,  DataStore::KeyTypeCountValueDefs::KeyType{"C"});
		assert(v.size() == 2);
		assert(v[0] == "Hello");
		assert(v[1] == "World");
		}
	}

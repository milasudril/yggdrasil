//@	{"targets":[{"name":"key_type_count_value_deserializer.test","type":"application","autorun":1}]}

#undef NDEBUG

#include "key_type_count_value_deserializer.hpp"

#include "mem_reader.hpp"
#include "native_decoder.hpp"
#include "test/compound_test.hpp"

int main()
	{
		{
		Test::Compound compound;
		DataStore::MemReader memReader
			{
			reinterpret_cast<std::byte const*>(std::begin(Test::data_le))
			, reinterpret_cast<std::byte const*>(std::end(Test::data_le))
			};

		auto status = DataStore::KeyTypeCountValueDeserializer{DataStore::NativeDecoder{memReader}}(compound);
		assert(status == DataStore::KeyTypeCountValueDefs::StatusCode::Success);
		assert(memReader.eof());

		assert(compound == Test::createTestCompound());
		}

		{
		Test::MyStatefulExceptionPolicy eh;
		Test::CompoundStatefulEh compound{eh};

		DataStore::MemReader memReader
			{
			reinterpret_cast<std::byte const*>(std::begin(Test::data_le))
			, reinterpret_cast<std::byte const*>(std::end(Test::data_le))
			};

		auto status = DataStore::KeyTypeCountValueDeserializer{DataStore::NativeDecoder{memReader}}(compound);
		assert(status == DataStore::KeyTypeCountValueDefs::StatusCode::Success);
		assert(memReader.eof());
		}
	}

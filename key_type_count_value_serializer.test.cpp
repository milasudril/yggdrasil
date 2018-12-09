//@	{"targets":[{"name":"key_type_count_value_serializer.test","type":"application","autorun":1}]}

#include "key_type_count_value_serializer.hpp"

#include "basic_compound.hpp"
#include "mem_writer.hpp"
#include "native_encoder.hpp"
#include "test/compound_test.hpp"

#include <type_traits>

int main()
	{
	auto compound = Test::createTestCompound();
	std::vector<std::byte> output_buffer;
	DataStore::MemWriter writer{output_buffer};

	assert(DataStore::KeyTypeCountValueSerializer{DataStore::NativeEncoder{writer}}(compound));

	assert(output_buffer.size() == sizeof(Test::data_le));
	assert(std::equal(std::begin(output_buffer), std::end(output_buffer)
		, reinterpret_cast<std::byte const*>(Test::data_le)));
	return 0;
	}

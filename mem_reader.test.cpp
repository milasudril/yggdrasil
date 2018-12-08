//@	{"targets":[{"name":"mem_reader.test","type":"application", "autorun":1}]}

#include "mem_reader.hpp"

#include "test/data_generator.hpp"

#include <vector>
#include <algorithm>
#include <cassert>


int main()
	{
	std::minstd_rand randgen;
	auto random_bytes = Test::getRandomData(randgen, 1.0);

	DataStore::MemReader reader{random_bytes.data(), random_bytes.data() + random_bytes.size()};

	std::vector<std::byte> read_back;
	read_back.reserve(random_bytes.size());

	std::geometric_distribution<size_t> chunk_size{1.0/4096.0};
	std::vector<std::byte> read_buffer(chunk_size(randgen) + 1);
	auto N = read_buffer.size();
	while(N != 0)
		{
		N = reader.read(read_buffer.data(), read_buffer.size());
		read_back.insert(read_back.end(), read_buffer.begin(), read_buffer.begin() + N);
		read_buffer.resize(chunk_size(randgen) + 1);
		}

	assert(read_back.size() == random_bytes.size());
	assert(read_back == random_bytes);

	return 0;
	}

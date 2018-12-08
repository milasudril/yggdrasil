//@	{"targets":[{"name":"mem_writer.test","type":"application", "autorun":1}]}

#include "mem_writer.hpp"
#include "test/data_generator.hpp"

#include <cassert>


int main()
	{
	std::minstd_rand randgen(time(0));
	auto random_bytes = Test::getRandomData(randgen, 1.0/16.0);

	std::vector<std::byte> output;
	DataStore::MemWriter writer{output};

	std::geometric_distribution<size_t> chunk_size{1.0/4096.0};
	auto ptr =  random_bytes.data();
	auto N = random_bytes.size();
	while(N != 0)
		{
		auto n = std::min(N, chunk_size(randgen));
		n = writer.write(ptr, n);
		N -= n;
		ptr += n;
		}

	assert(output.size() == random_bytes.size());
	assert(output == random_bytes);

	return 0;
	}


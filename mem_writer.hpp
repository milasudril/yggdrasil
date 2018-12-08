//@	{"targets":[{"name":"mem_writer.hpp","type":"include"}]}

#ifndef DATA_STORE_MEM_WRITER_HPP
#define DATA_STORE_MEM_WRITER_HPP

#include <vector>
#include <algorithm>
#include <cstddef>

namespace DataStore
	{
	class MemWriter
		{
		public:
			explicit MemWriter(std::vector<std::byte>& buffer) : r_buffer(buffer) {}

			[[nodiscard]] size_t write(void const* buffer, size_t count)
				{
				auto bytes_begin = reinterpret_cast<std::byte const*>(buffer);
				auto bytes_end = bytes_begin + count;
				std::copy(bytes_begin, bytes_end, std::back_inserter(r_buffer));
				return count;
				}

		private:
			std::vector<std::byte>& r_buffer;
		};
	};

#endif

//@	{"targets":[{"name":"mem_reader.hpp","type":"include"}]}

#ifndef DATA_STORE_MEM_READER_HPP
#define DATA_STORE_MEM_READER_HPP

#include "utility.hpp"

#include <cstring>
#include <algorithm>

namespace DataStore
	{
	class MemReader
		{
		public:
			explicit MemReader(std::byte const* begin, std::byte const* end):
				r_read_offset(begin), m_size(end - begin)
				{}

			[[nodiscard]] size_t read(void* buffer, size_t count)
				{
				auto N = std::min(count, m_size);
				memcpy(buffer, r_read_offset, N);
				r_read_offset += N;
				m_size -= N;
				return N;
				}

			[[nodiscard]] bool eof() const
				{return m_size == 0;}

		private:
			std::byte const* r_read_offset;
			size_t m_size;
		};
	};

#endif

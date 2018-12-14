#ifndef DATA_STORE_POSIX_FILE_READER_HPP
#define DATA_STORE_POSIX_FILE_READER_HPP

#include <unistd.h>
#include <fcntl.h>

#include <cstring>
#include <memory>

namespace DataStore
	{
	class PosixFileReader
		{
		public:
			PosixFileReader(PosixFileReader const&) = delete;

			explicit PosixFileReader(char const* path):
				m_buffer(new std::byte[BufferSize]), m_remaining(0)
				{
				m_fd = open(path, O_RDONLY); // TODO: Check errno
				}

			~PosixFileReader()
				{close(m_fd);}

			[[nodiscard]] __attribute__((noinline)) size_t read(void* buffer, size_t count)
				{
				if(count > BufferSize)
					{
					auto write_ptr = reinterpret_cast<std::byte*>(buffer);
					auto N = std::min(count, m_remaining);
					memcpy(write_ptr, r_read_ptr, N);
					m_remaining = 0;
					return N + read_helper(write_ptr, count - N);
					}
				else
					{
					if(m_remaining == 0)
						{fetch();}

					auto write_ptr = reinterpret_cast<std::byte*>(buffer);
					auto N = std::min(count, m_remaining);
					memcpy(write_ptr, r_read_ptr, N);
					m_remaining -= N;
					r_read_ptr += N;
					return N;
					}
				}

		private:
			static constexpr size_t BufferSize = 4096;

			std::unique_ptr<std::byte> m_buffer;
			std::byte const* r_read_ptr;
			size_t m_remaining;
			int m_fd;


			size_t __attribute__((noinline)) read_helper(std::byte* buffer, size_t N)
				{
				auto const start_pos = buffer;
				while(N != 0)
					{
					auto n = ::read(m_fd, buffer, N);
					if(n < 1)
						{return static_cast<size_t>(buffer - start_pos);}
					buffer += n;
					N -= n;
					}
				return static_cast<size_t>(buffer - start_pos);
				}

			void fetch()
				{
				m_remaining = read_helper(m_buffer.get(), BufferSize);
				r_read_ptr = m_buffer.get();
				}
		};
	}

#endif

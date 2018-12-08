//@	{"targets":[{"name":"native_decoder.test", "type":"application", "autorun":1}]}

#include "native_decoder.hpp"
#include <cassert>

#include <cstdio>


class Source
	{
	public:
		Source() : r_buffer(nullptr), m_n(0), m_fail(false){}

		size_t read(void* buffer, size_t n)
			{
			r_buffer = buffer;
			m_n = n;
			return m_fail? 0 : n;
			}

		void const* getBuffer() const
			{return r_buffer;}

		size_t getBufferSize() const
			{return m_n;}


		void setFail(bool status)
			{m_fail = status;}

	private:
		void* r_buffer;
		size_t m_n;
		bool m_fail;
	};


int main()
 	{
 	Source src;
 	DataStore::NativeDecoder decoder{src};

		{
		int x;
		assert(decoder.read(x));
		assert(src.getBufferSize() == sizeof(x));
		assert(src.getBuffer() == &x);
		}

		{
		int x[4];
		assert(sizeof(x) == 4*sizeof(int));
		assert(decoder.read(&x[0], 4));
		assert(src.getBuffer() == &x);
		assert(src.getBufferSize() == sizeof(x));
		}

	src.setFail(true);

		{
		int x;
		assert(!decoder.read(x));
		assert(src.getBufferSize() == sizeof(x));
		assert(src.getBuffer() == &x);
		}

		{
		int x[4];
		assert(sizeof(x) == 4*sizeof(int));
		assert(!decoder.read(&x[0], 4));
		assert(src.getBuffer() == &x);
		assert(src.getBufferSize() == sizeof(x));
		}



 	return 0;
 	}

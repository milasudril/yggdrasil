//@	{"targets":[{"name":"native_encoder.test", "type":"application", "autorun":1}]}

#undef NDEBUG

#include "native_encoder.hpp"
#include <cassert>

class Sink
	{
	public:
		Sink() : r_buffer(nullptr), m_n(0), m_fail(false){}

		size_t write(void const* buffer, size_t n)
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
		void const* r_buffer;
		size_t m_n;
		bool m_fail;
	};


int main()
 	{
 	Sink sink;
 	DataStore::NativeEncoder encoder{sink};

		{
		int x;
		assert(encoder.write(x));
		assert(sink.getBufferSize() == sizeof(x));
		assert(sink.getBuffer() == &x);
		}

		{
		int x[4];
		assert(sizeof(x) == 4*sizeof(int));
		assert(encoder.write(&x[0], 4));
		assert(sink.getBuffer() == &x);
		assert(sink.getBufferSize() == sizeof(x));
		}

	sink.setFail(true);

		{
		int x;
		assert(!encoder.write(x));
		assert(sink.getBufferSize() == sizeof(x));
		assert(sink.getBuffer() == &x);
		}

		{
		int x[4];
		assert(sizeof(x) == 4*sizeof(int));
		assert(!encoder.write(&x[0], 4));
		assert(sink.getBuffer() == &x);
		assert(sink.getBufferSize() == sizeof(x));
		}

 	return 0;
 	}

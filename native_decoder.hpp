//@	{"targets":[{"name":"native_decoder.hpp", "type":"include"}]}

#ifndef DATA_STORE_NATIVE_DECODER_HPP
#define DATA_STORE_NATIVE_DECODER_HPP

#include "utility.hpp"

namespace DataStore
	{
	template<class Source>
	class NativeDecoder
		{
		public:
			constexpr explicit NativeDecoder(Source& source) : r_source(source) {}

			template<class T>
			constexpr
			std::enable_if_t<DataStore::IsPod<T>::value, bool>
			read(T& value)
				{return r_source.read(&value, sizeof(value)) == sizeof(value);}

			template<class T>
			constexpr
			std::enable_if_t<DataStore::IsPod<T>::value, bool>
			read(T* value, size_t N)
				{return r_source.read(value, N*sizeof(T)) == N*sizeof(T);}

		private:
			Source& r_source;
		};
	}

#endif

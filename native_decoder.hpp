//@	{"targets":[{"name":"native_decoder.hpp", "type":"include"}]}

#ifndef DATA_STORE_NATIVE_DECODER_HPP
#define DATA_STORE_NATIVE_DECODER_HPP

#include "utility.hpp"

#include <type_traits>

namespace DataStore
	{
	template<class Source>
	class NativeDecoder
		{
		public:
			constexpr explicit NativeDecoder(Source& source) : r_source(source) {}

			template<class T>
			[[nodiscard]]
			constexpr
			std::enable_if_t<DataStore::IsPod<T>::value, bool>
			read(T& value)
				{return r_source.read(&value, sizeof(value)) == sizeof(value);}

			template<class T>
			[[nodiscard]]
			constexpr
			std::enable_if_t<DataStore::IsPod<T>::value, bool>
			read(T* value, size_t N)
				{
				constexpr auto elem_size = sizeof(T);
				return r_source.read(value, N*elem_size) == N*elem_size;
				}

		private:
			Source& r_source;
		};
	}

#endif

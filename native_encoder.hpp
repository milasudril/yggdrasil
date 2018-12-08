//@	{"targets":[{"name":"native_encoder.hpp", "type":"include"}]}

#ifndef DATA_STORE_NATIVE_ENCODER_HPP
#define DATA_STORE_NATIVE_ENCODER_HPP

#include "utility.hpp"

#include <type_traits>

namespace DataStore
	{
	template<class Sink>
	class NativeEncoder
		{
		public:
			constexpr explicit NativeEncoder(Sink& sink) : r_sink(sink) {}

			template<class T>
			[[nodiscard]]
			constexpr
			std::enable_if_t<DataStore::IsPod<T>::value, bool>
			write(T const& value)
				{return r_sink.write(&value, sizeof(value)) == sizeof(value);}

			template<class T>
			[[nodiscard]]
			constexpr
			std::enable_if_t<DataStore::IsPod<T>::value, bool>
			write(T const* value, size_t N)
				{
				constexpr auto elem_size = sizeof(T);
				return r_sink.write(value, N*elem_size) == N*elem_size;
				}

		private:
			Sink& r_sink;
		};
	}

#endif

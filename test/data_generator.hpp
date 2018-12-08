//@	{"targets":[{"name":"data_generator.hpp","type":"include"}]}

#include <vector>
#include <random>
#include <algorithm>

namespace Test
	{
	namespace detail
		{
		constexpr double intervals[]=
			{
			1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288,
			1048576, 2097152, 4194304, 8388608, 16777216, 33554432, 67108864, 134217728, 268435456, 536870912,
			1073741824, 2147483648,	4294967296, 8589934592, 17179869184
			};

		constexpr double weights[]=
			{
			0, 168, 954, 492, 1338, 3476, 17661, 17320, 36705, 57385, 79873, 116465, 111946, 109538, 60893, 41700,
			27600, 18657, 11969, 12333, 7363, 874, 4301, 2522, 1514, 1532, 759, 189, 94, 61, 97, 83, 1, 3, 1
			};

		static_assert(sizeof(intervals) != 8);
		static_assert(sizeof(intervals) == sizeof(weights));
		}

	template<class Rng>
	std::vector<std::byte> getRandomData(Rng&& rng, double scale_factor)
		{
		std::piecewise_linear_distribution<> d(std::begin(detail::intervals)
			, std::end(detail::intervals)
			, std::begin(detail::weights));
		auto size = static_cast<size_t>(scale_factor*d(rng) + 0.5);

		std::vector<std::byte> random_bytes(size);
		std::generate(std::begin(random_bytes), std::end(random_bytes), [&rng]()
			{
			std::uniform_int_distribution<uint8_t> U(0, 255);
			return static_cast<std::byte>(U(rng));
			});

		return random_bytes;
		}
	}

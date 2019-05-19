//@	{"targets":[{"name":"compound.test","type":"application", "autorun":1}]}

#include "compound.hpp"

#include "mem_reader.hpp"
#include "mem_writer.hpp"
#include "test/data_generator.hpp"

#include <random>
#include <limits>
#include <exception>
#include <cmath>

struct MyExceptionPolicy
	{
	[[noreturn]]
	static void keyNotFound(std::string_view key)
		{throw std::string(key);}

	[[noreturn]]
	static void keyValueHasWrongType(std::string_view key, size_t actualType)
		{throw std::string(key);}

	[[noreturn]]
	static void keyAlreadyExists(Yggdrasil::KeyType const& key)
		{throw std::runtime_error(std::string(std::begin(key), std::end(key)));}
	};

using SupportedTypes = Yggdrasil::Compound<MyExceptionPolicy>::SupportedTypes;

constexpr std::array<double, 64> type_weights
	{
	 1.0     	// VecInt8
	,1.0     	// VecUint8
	,1.0     	// VecFloat8
	,0.0     	// Reserved3
	,1.0     	// VecInt16
	,1.0     	// VecUint16
	,1.0     	// VecFloat16
	,1.0     	// Reserved7
	,1.0     	// VecInt32
	,1.0     	// VecUint32
	,1.0     	// VecFloat32
	,0.0     	// Reserved11
	,1.0     	// VecInt64
	,1.0     	// VecUint64
	,1.0     	// VecFloat64
	,0.0     	// Reserved15
	,1.0     	// Int8
	,1.0     	// Uint8
	,1.0     	// Float8
	,1.0     	// String
	,1.0     	// Int16
	,1.0     	// Uint16
	,1.0     	// Float16
	,0.0     	// Reserved23
	,1.0     	// Int32
	,1.0     	// Uint32
	,1.0     	// Float32
	,0.0     	// Reserved27
	,1.0     	// Int64
	,1.0     	// Uint64
	,1.0     	// Float64
	,1.0     	// Compound
	,1.0     	// ArrayVecInt8
	,1.0     	// ArrayVecUint8
	,1.0     	// ArrayVecFloat8
	,1.0     	// ArrayReserved3
	,1.0     	// ArrayVecInt16
	,1.0     	// ArrayVecUint16
	,1.0     	// ArrayVecFloat16
	,0.0     	// ArrayReserved7
	,1.0     	// ArrayVecInt32
	,1.0     	// ArrayVecUint32
	,1.0     	// ArrayVecFloat32
	,0.0     	// ArrayReserved11
	,1.0     	// ArrayVecInt64
	,1.0     	// ArrayVecUint64
	,1.0     	// ArrayVecFloat64
	,0.0     	// ArrayReserved15
	,1.0     	// ArrayInt8
	,1.0     	// ArrayUint8
	,1.0     	// ArrayFloat8
	,1.0     	// ArrayString
	,1.0     	// ArrayInt16
	,1.0     	// ArrayUint16
	,1.0     	// ArrayFloat16
	,0.0     	// ArrayReserved23
	,1.0     	// ArrayInt32
	,1.0     	// ArrayUint32
	,1.0     	// ArrayFloat32
	,0.0     	// ArrayReserved27
	,1.0     	// ArrayInt64
	,1.0     	// ArrayUint64
	,1.0     	// ArrayFloat64
	,1.0     	// ArrayCompound
	};

static std::geometric_distribution<uint32_t> number_of_children{1.0/24.0};
static std::discrete_distribution<uint8_t> type_freq{std::begin(type_weights), std::end(type_weights)};

template<class Rng>
Yggdrasil::KeyType generateKey(Rng& rng)
	{
	Yggdrasil::KeyType ret{};
	std::generate(ret.begin(), ret.end(), [&rng]()
		{return Test::generateAscii(rng);});
	return ret;
	}


template<class Rng>
void fillCompound(Yggdrasil::Compound<MyExceptionPolicy>& compound, Rng& rng, size_t depth);

template<class Compound, class Rng>
class DataGenerator
	{
	public:
		explicit DataGenerator(Compound& compound, Rng& rng, size_t depth) : r_compound(compound), r_rng(rng)
			,m_depth(depth)
			{}

		void operator()() {}


		template<template<class> class Sequence, class T>
		std::enable_if_t<DataStore::IsSequenceOf<Sequence<T>, T>::value>
		operator()(Analib::Empty<Sequence<T>>)
			{
			static_assert(DataStore::IsPod<T>::value);
			auto M = Test::generateSize(r_rng, 1.0/(1024.0*1024.0*sizeof(T)));
			std::vector<T> ret;
			while(M!=0)
				{
				ret.push_back(Test::generate<T>(r_rng));
				--M;
				}
			auto key = generateKey(r_rng);
			r_compound.insert(key, std::move(ret));
			}


		template<template<class> class Sequence>
		std::enable_if_t<DataStore::IsSequenceOf<Sequence<Compound>, Compound>::value>
		operator()(Analib::Empty<Sequence<Compound>>)
			{
			auto N = number_of_children(r_rng);
			Sequence<Compound> compounds;
			while(N!=0)
				{
				Compound next;
				fillCompound(next, r_rng, m_depth);
				compounds.push_back(std::move(next));
				--N;
				}
			auto key = generateKey(r_rng);
			r_compound.insert(key, std::move(compounds));
			}

		template<template<class> class Sequence>
		std::enable_if_t<DataStore::IsSequenceOf<Sequence<Yggdrasil::String>, Yggdrasil::String>::value>
		operator()(Analib::Empty<Sequence<Yggdrasil::String>>)
			{
			auto N = number_of_children(r_rng);
			Sequence<Yggdrasil::String> strings;
			while(N!=0)
				{
				auto M = Test::generateSize(r_rng, 1.0/(1024.0*1024.0));
				Yggdrasil::String ret;
				while(M!=0)
					{
					ret.append(Test::generateAscii(r_rng));
					--M;
					}
				strings.push_back(std::move(ret));
				--N;
				}
			auto key = generateKey(r_rng);
			r_compound.insert(key, std::move(strings));
			}

		template<class T>
		void operator()(Analib::Empty<T>)
			{
			if constexpr(DataStore::IsPod<T>::value)
				{
				auto key = generateKey(r_rng);
				r_compound.insert(key, Test::generate<T>(r_rng));
				}
			else if constexpr(std::is_same_v<Compound, T>)
				{
				Compound next;
				fillCompound(next, r_rng, m_depth);
				auto key = generateKey(r_rng);
				r_compound.insert(key, std::move(next));
				}
			else if constexpr(std::is_same_v<Yggdrasil::String, T>)
				{
				auto N = Test::generateSize(r_rng, 1.0/(1024.0*1024.0));
				Yggdrasil::String ret;
				while(N!=0)
					{
					ret.append(Test::generateAscii(r_rng));
					--N;
					}
				auto key = generateKey(r_rng);
				r_compound.insert(key, std::move(ret));
				}
			else
				{printf("%zx ", SupportedTypes::getTypeIndex<T>());}
			}

	private:
		Compound& r_compound;
		Rng& r_rng;
		size_t m_depth;

	};


template<class Rng>
void fillCompound(Yggdrasil::Compound<MyExceptionPolicy>& compound, Rng& rng, size_t depth)
	{
	auto l = -1.0*static_cast<double>(depth);
	auto N = static_cast<size_t>( static_cast<double>(number_of_children(rng)) * std::exp2(l) +0.5) + 1;
	while(N!=0)
		{
		auto type_id = type_freq(rng);
		SupportedTypes::select(type_id, DataGenerator{compound, rng, depth + 1});
		--N;
		}

	}


int main()
	{
	Yggdrasil::Compound<MyExceptionPolicy> test_1;
		{
		std::minstd_rand randgen(time(0));
		fillCompound(test_1, randgen, 0);
		}

	assert(test_1.childCount() > 0);

	std::vector<std::byte> buffer_1;
		{
		DataStore::MemWriter writer{buffer_1};
		assert(store(test_1, writer));
		}

	Yggdrasil::Compound<MyExceptionPolicy> test_2;
		{
		DataStore::MemReader reader{buffer_1.data(), buffer_1.data() + buffer_1.size()};
		assert(load(test_2, reader) == Yggdrasil::StatusCode::Success);
		}

	std::vector<std::byte> buffer_2;
		{
		DataStore::MemWriter writer{buffer_2};
		assert(store(test_2, writer));
		}

	assert(buffer_1 == buffer_2);
	return 0;
	}

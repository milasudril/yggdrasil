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

namespace ImplCheck
	{
	static_assert(sizeof(Yggdrasil::KeyType) == Yggdrasil::KeySize);
	static_assert(sizeof(DataStore::KeyTypeCountValueDefs::KeyType) == Yggdrasil::KeySize);
	static_assert(sizeof(DataStore::KeyTypeCountValueDefs::ArraySize) == Yggdrasil::ArrayElemCountSize);
	static_assert(sizeof(DataStore::KeyTypeCountValueDefs::TypeId) == Yggdrasil::TypeIdSize);


	static_assert(SupportedTypes::size() == static_cast<size_t>(Yggdrasil::TypeId::Size));

	static_assert(SupportedTypes::getTypeIndex<int8_t>() == static_cast<size_t>(Yggdrasil::TypeId::Int8));
	static_assert(SupportedTypes::getTypeIndex<int16_t>() == static_cast<size_t>(Yggdrasil::TypeId::Int16));
	static_assert(SupportedTypes::getTypeIndex<int32_t>() == static_cast<size_t>(Yggdrasil::TypeId::Int32));
	static_assert(SupportedTypes::getTypeIndex<int64_t>() == static_cast<size_t>(Yggdrasil::TypeId::Int64));

	static_assert(SupportedTypes::getTypeIndex<uint8_t>() == static_cast<size_t>(Yggdrasil::TypeId::Uint8));
	static_assert(SupportedTypes::getTypeIndex<uint16_t>() == static_cast<size_t>(Yggdrasil::TypeId::Uint16));
	static_assert(SupportedTypes::getTypeIndex<uint32_t>() == static_cast<size_t>(Yggdrasil::TypeId::Uint32));
	static_assert(SupportedTypes::getTypeIndex<uint64_t>() == static_cast<size_t>(Yggdrasil::TypeId::Uint64));

	static_assert(SupportedTypes::getTypeIndex<Yggdrasil::Minifloat>() == static_cast<size_t>(Yggdrasil::TypeId::Float8));
	static_assert(SupportedTypes::getTypeIndex<Yggdrasil::Half>() == static_cast<size_t>(Yggdrasil::TypeId::Float16));
	static_assert(SupportedTypes::getTypeIndex<float>() == static_cast<size_t>(Yggdrasil::TypeId::Float32));
	static_assert(SupportedTypes::getTypeIndex<double>() == static_cast<size_t>(Yggdrasil::TypeId::Float64));

	static_assert(SupportedTypes::getTypeIndex<Yggdrasil::String>() == static_cast<size_t>(Yggdrasil::TypeId::String));
	static_assert(SupportedTypes::getTypeIndex<Yggdrasil::Reserved<3>>() == static_cast<size_t>(Yggdrasil::TypeId::Reserved3));
	static_assert(SupportedTypes::getTypeIndex<Yggdrasil::Reserved<7>>() == static_cast<size_t>(Yggdrasil::TypeId::Reserved7));


	static_assert(SupportedTypes::getTypeIndex<Yggdrasil::vec4_t<int8_t>>() == static_cast<size_t>(Yggdrasil::TypeId::VecInt8));
	static_assert(SupportedTypes::getTypeIndex<Yggdrasil::vec4_t<int16_t>>() == static_cast<size_t>(Yggdrasil::TypeId::VecInt16));
	static_assert(SupportedTypes::getTypeIndex<Yggdrasil::vec4_t<int32_t>>() == static_cast<size_t>(Yggdrasil::TypeId::VecInt32));
	static_assert(SupportedTypes::getTypeIndex<Yggdrasil::vec4_t<int64_t>>() == static_cast<size_t>(Yggdrasil::TypeId::VecInt64));

	static_assert(SupportedTypes::getTypeIndex<Yggdrasil::vec4_t<uint8_t>>() == static_cast<size_t>(Yggdrasil::TypeId::VecUint8));
	static_assert(SupportedTypes::getTypeIndex<Yggdrasil::vec4_t<uint16_t>>() == static_cast<size_t>(Yggdrasil::TypeId::VecUint16));
	static_assert(SupportedTypes::getTypeIndex<Yggdrasil::vec4_t<uint32_t>>() == static_cast<size_t>(Yggdrasil::TypeId::VecUint32));
	static_assert(SupportedTypes::getTypeIndex<Yggdrasil::vec4_t<uint64_t>>() == static_cast<size_t>(Yggdrasil::TypeId::VecUint64));

	static_assert(SupportedTypes::getTypeIndex<Yggdrasil::vec4_t<Yggdrasil::Minifloat>>() == static_cast<size_t>(Yggdrasil::TypeId::VecFloat8));
	static_assert(SupportedTypes::getTypeIndex<Yggdrasil::vec4_t<Yggdrasil::Half>>() == static_cast<size_t>(Yggdrasil::TypeId::VecFloat16));
	static_assert(SupportedTypes::getTypeIndex<Yggdrasil::vec4_t<float>>() == static_cast<size_t>(Yggdrasil::TypeId::VecFloat32));
	static_assert(SupportedTypes::getTypeIndex<Yggdrasil::vec4_t<double>>() == static_cast<size_t>(Yggdrasil::TypeId::VecFloat64));

	static_assert(SupportedTypes::getTypeIndex<Yggdrasil::Reserved<11>>() == static_cast<size_t>(Yggdrasil::TypeId::Reserved11));
	static_assert(SupportedTypes::getTypeIndex<Yggdrasil::Reserved<15>>() == static_cast<size_t>(Yggdrasil::TypeId::Reserved15));
	static_assert(SupportedTypes::getTypeIndex<Yggdrasil::Reserved<23>>() == static_cast<size_t>(Yggdrasil::TypeId::Reserved23));
	static_assert(SupportedTypes::getTypeIndex<Yggdrasil::Compound<MyExceptionPolicy>>() == static_cast<size_t>(Yggdrasil::TypeId::Compound));



	static_assert(SupportedTypes::getTypeIndex<std::vector<int8_t>>() == static_cast<size_t>(Yggdrasil::TypeId::ArrayInt8));
	static_assert(SupportedTypes::getTypeIndex<std::vector<int16_t>>() == static_cast<size_t>(Yggdrasil::TypeId::ArrayInt16));
	static_assert(SupportedTypes::getTypeIndex<std::vector<int32_t>>() == static_cast<size_t>(Yggdrasil::TypeId::ArrayInt32));
	static_assert(SupportedTypes::getTypeIndex<std::vector<int64_t>>() == static_cast<size_t>(Yggdrasil::TypeId::ArrayInt64));

	static_assert(SupportedTypes::getTypeIndex<std::vector<uint8_t>>() == static_cast<size_t>(Yggdrasil::TypeId::ArrayUint8));
	static_assert(SupportedTypes::getTypeIndex<std::vector<uint16_t>>() == static_cast<size_t>(Yggdrasil::TypeId::ArrayUint16));
	static_assert(SupportedTypes::getTypeIndex<std::vector<uint32_t>>() == static_cast<size_t>(Yggdrasil::TypeId::ArrayUint32));
	static_assert(SupportedTypes::getTypeIndex<std::vector<uint64_t>>() == static_cast<size_t>(Yggdrasil::TypeId::ArrayUint64));

	static_assert(SupportedTypes::getTypeIndex<std::vector<Yggdrasil::Minifloat>>() == static_cast<size_t>(Yggdrasil::TypeId::ArrayFloat8));
	static_assert(SupportedTypes::getTypeIndex<std::vector<Yggdrasil::Half>>() == static_cast<size_t>(Yggdrasil::TypeId::ArrayFloat16));
	static_assert(SupportedTypes::getTypeIndex<std::vector<float>>() == static_cast<size_t>(Yggdrasil::TypeId::ArrayFloat32));
	static_assert(SupportedTypes::getTypeIndex<std::vector<double>>() == static_cast<size_t>(Yggdrasil::TypeId::ArrayFloat64));

	static_assert(SupportedTypes::getTypeIndex<std::vector<Yggdrasil::String>>() == static_cast<size_t>(Yggdrasil::TypeId::ArrayString));
	static_assert(SupportedTypes::getTypeIndex<std::vector<Yggdrasil::Reserved<27>>>() == static_cast<size_t>(Yggdrasil::TypeId::ArrayReserved27));


	static_assert(SupportedTypes::getTypeIndex<std::vector<Yggdrasil::vec4_t<int8_t>>>() == static_cast<size_t>(Yggdrasil::TypeId::ArrayVecInt8));
	static_assert(SupportedTypes::getTypeIndex<std::vector<Yggdrasil::vec4_t<int16_t>>>() == static_cast<size_t>(Yggdrasil::TypeId::ArrayVecInt16));
	static_assert(SupportedTypes::getTypeIndex<std::vector<Yggdrasil::vec4_t<int32_t>>>() == static_cast<size_t>(Yggdrasil::TypeId::ArrayVecInt32));
	static_assert(SupportedTypes::getTypeIndex<std::vector<Yggdrasil::vec4_t<int64_t>>>() == static_cast<size_t>(Yggdrasil::TypeId::ArrayVecInt64));

	static_assert(SupportedTypes::getTypeIndex<std::vector<Yggdrasil::vec4_t<uint8_t>>>() == static_cast<size_t>(Yggdrasil::TypeId::ArrayVecUint8));
	static_assert(SupportedTypes::getTypeIndex<std::vector<Yggdrasil::vec4_t<uint16_t>>>() == static_cast<size_t>(Yggdrasil::TypeId::ArrayVecUint16));
	static_assert(SupportedTypes::getTypeIndex<std::vector<Yggdrasil::vec4_t<uint32_t>>>() == static_cast<size_t>(Yggdrasil::TypeId::ArrayVecUint32));
	static_assert(SupportedTypes::getTypeIndex<std::vector<Yggdrasil::vec4_t<uint64_t>>>() == static_cast<size_t>(Yggdrasil::TypeId::ArrayVecUint64));

	static_assert(SupportedTypes::getTypeIndex<std::vector<Yggdrasil::vec4_t<Yggdrasil::Minifloat>>>() == static_cast<size_t>(Yggdrasil::TypeId::ArrayVecFloat8));
	static_assert(SupportedTypes::getTypeIndex<std::vector<Yggdrasil::vec4_t<Yggdrasil::Half>>>() == static_cast<size_t>(Yggdrasil::TypeId::ArrayVecFloat16));
	static_assert(SupportedTypes::getTypeIndex<std::vector<Yggdrasil::vec4_t<float>>>() == static_cast<size_t>(Yggdrasil::TypeId::ArrayVecFloat32));
	static_assert(SupportedTypes::getTypeIndex<std::vector<Yggdrasil::vec4_t<double>>>() == static_cast<size_t>(Yggdrasil::TypeId::ArrayVecFloat64));

	static_assert(SupportedTypes::getTypeIndex<std::vector<Yggdrasil::Compound<MyExceptionPolicy>>>() == static_cast<size_t>(Yggdrasil::TypeId::ArrayCompound));
	}

constexpr std::array<double, 64> type_weights
	{
	 1.0
	,1.0
	,1.0
	,1.0

	,1.0
	,1.0
	,1.0
	,1.0

	,1.0
	,1.0
	,1.0
	,1.0

	,1.0
	,1.0
	,1.0
	,1.0

	,1.0
	,1.0
	,1.0
	,1.0

	,1.0
	,1.0
	,1.0
	,1.0

	,1.0
	,1.0
	,1.0
	,1.0

	,1.0
	,1.0
	,1.0
	,8.0


	,1.0
	,1.0
	,1.0
	,1.0

	,1.0
	,1.0
	,1.0
	,1.0

	,1.0
	,1.0
	,1.0
	,1.0

	,1.0
	,1.0
	,1.0
	,1.0

	,1.0
	,1.0
	,1.0
	,1.0

	,1.0
	,1.0
	,1.0
	,1.0

	,1.0
	,1.0
	,1.0
	,1.0

	,1.0
	,1.0
	,1.0
	,1.0
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

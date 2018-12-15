//@	{"targets":[{"name":"compound.test","type":"application", "autorun":1}]}

#include "compound.hpp"
#include "native_decoder.hpp"
#include "posix_file_reader.hpp"
#include "key_type_count_value_deserializer.hpp"

#include "native_encoder.hpp"
#include "mem_writer.hpp"
#include "key_type_count_value_serializer.hpp"

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
		{throw key;}
	};

namespace ImplCheck
	{
	using SupportedTypes = Yggdrasil::Compound<MyExceptionPolicy>::SupportedTypes;

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
	static_assert(SupportedTypes::getTypeIndex<std::byte>() == static_cast<size_t>(Yggdrasil::TypeId::Raw));
	static_assert(SupportedTypes::getTypeIndex<Yggdrasil::Reserved<0>>() == static_cast<size_t>(Yggdrasil::TypeId::Reserved0));
	static_assert(SupportedTypes::getTypeIndex<Yggdrasil::Reserved<1>>() == static_cast<size_t>(Yggdrasil::TypeId::Reserved1));


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

	static_assert(SupportedTypes::getTypeIndex<Yggdrasil::Reserved<2>>() == static_cast<size_t>(Yggdrasil::TypeId::Reserved2));
	static_assert(SupportedTypes::getTypeIndex<Yggdrasil::Reserved<3>>() == static_cast<size_t>(Yggdrasil::TypeId::Reserved3));
	static_assert(SupportedTypes::getTypeIndex<Yggdrasil::Reserved<4>>() == static_cast<size_t>(Yggdrasil::TypeId::Reserved4));
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
	static_assert(SupportedTypes::getTypeIndex<std::vector<std::byte>>() == static_cast<size_t>(Yggdrasil::TypeId::ArrayRaw));
	static_assert(SupportedTypes::getTypeIndex<std::vector<Yggdrasil::Reserved<0>>>() == static_cast<size_t>(Yggdrasil::TypeId::ArrayReserved0));
	static_assert(SupportedTypes::getTypeIndex<std::vector<Yggdrasil::Reserved<1>>>() == static_cast<size_t>(Yggdrasil::TypeId::ArrayReserved1));


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

	static_assert(SupportedTypes::getTypeIndex<std::vector<Yggdrasil::Reserved<2>>>() == static_cast<size_t>(Yggdrasil::TypeId::ArrayReserved2));
	static_assert(SupportedTypes::getTypeIndex<std::vector<Yggdrasil::Reserved<3>>>() == static_cast<size_t>(Yggdrasil::TypeId::ArrayReserved3));
	static_assert(SupportedTypes::getTypeIndex<std::vector<Yggdrasil::Reserved<4>>>() == static_cast<size_t>(Yggdrasil::TypeId::ArrayReserved4));
	static_assert(SupportedTypes::getTypeIndex<std::vector<Yggdrasil::Compound<MyExceptionPolicy>>>() == static_cast<size_t>(Yggdrasil::TypeId::ArrayCompound));
	}


int main()
	{
	Yggdrasil::Compound<MyExceptionPolicy> test;
	DataStore::PosixFileReader fileIn(__FILE__);
	assert(load(test, fileIn) == Yggdrasil::StatusCode::Success);
	printf("%zu\n", test.childCount());

	test.insert(Yggdrasil::KeyType("Foobar"), Yggdrasil::String("Hello"));

	std::vector<std::byte> buffer;
	DataStore::MemWriter writer{buffer};
	assert(store(test, writer));

	return 0;
	}

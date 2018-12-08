//@	{"targets":[{"name":"key_type_count_value_deserializer.test","type":"application","autorun":1}]}

#include "key_type_count_value_deserializer.hpp"

#include "basic_compound.hpp"
#include "mem_reader.hpp"

#include <type_traits>

struct MyExceptionPolicy
	{
	[[noreturn]]
	static void keyNotFound(DataStore::KeyTypeCountValueDefs::KeyType key)
		{throw key;}

	template<class T>
	[[noreturn]]
	static void keyValueHasWrongType(DataStore::KeyTypeCountValueDefs::KeyType key, size_t actualType)
		{throw key;}

	template<class T>
	[[noreturn]]
	static void keyAlreadyExists(DataStore::KeyTypeCountValueDefs::KeyType key, T const& value)
		{throw key;}
	};

template<class Source>
struct NativeDecoder
	{
	template<class T>
	std::enable_if_t<DataStore::IsPod<T>::value, bool>
	read(T& value)
		{return r_source.read(&value, sizeof(value)) == sizeof(value);}

	template<class T>
	std::enable_if_t<DataStore::IsPod<T>::value, bool>
	read(T* value, size_t N)
		{return r_source.read(value, N*sizeof(T)) == N*sizeof(T);}

	Source& r_source;
	};

using Compound = DataStore::BasicCompound<MyExceptionPolicy, DataStore::KeyTypeCountValueDefs::KeyType, std::string, int>;

constexpr uint8_t data_le[]=
	{
	 3, 0, 0, 0
	,'A',0, 0, 0, 0, 0, 0, 0
	,0,  0, 0, 0, 0, 0, 0, 0
	,0,  0, 0, 0, 0, 0, 0, 0
	,0,  0, 0, 0, 0, 0, 0, 0
	,Compound::SupportedTypes::getTypeIndex<Compound>()
	,2, 0, 0, 0
	,'a','1', 0, 0, 0, 0, 0, 0
	,0,  0, 0, 0, 0, 0, 0, 0
	,0,  0, 0, 0, 0, 0, 0, 0
	,0,  0, 0, 0, 0, 0, 0, 0
	,Compound::SupportedTypes::getTypeIndex<int>()
	,1, 2, 3, 4
	,'a','2', 0, 0, 0, 0, 0, 0
	,0,  0, 0, 0, 0, 0, 0, 0
	,0,  0, 0, 0, 0, 0, 0, 0
	,0,  0, 0, 0, 0, 0, 0, 0
	,Compound::SupportedTypes::getTypeIndex<std::string>()
	,6, 0, 0, 0
	,'F','o','o','b','a','r'
	,'B',0, 0, 0, 0, 0, 0, 0
	,0,  0, 0, 0, 0, 0, 0, 0
	,0,  0, 0, 0, 0, 0, 0, 0
	,0,  0, 0, 0, 0, 0, 0, 0
	,Compound::SupportedTypes::getTypeIndex<std::vector<Compound>>()
	,2, 0, 0, 0
	,1 ,0 ,0 ,0
	,'B','0', '1', 0, 0, 0, 0, 0
	,0,  0, 0, 0, 0, 0, 0, 0
	,0,  0, 0, 0, 0, 0, 0, 0
	,0,  0, 0, 0, 0, 0, 0, 0
	,Compound::SupportedTypes::getTypeIndex<int>()
	,2, 2, 3, 4
	,0, 0, 0, 0
	,'C',0, 0, 0, 0, 0, 0, 0
	,0,  0, 0, 0, 0, 0, 0, 0
	,0,  0, 0, 0, 0, 0, 0, 0
	,0,  0, 0, 0, 0, 0, 0, 0
	,Compound::SupportedTypes::getTypeIndex<std::vector<std::string>>()
	,2, 0, 0, 0
	,5, 0, 0, 0
	,'H','e','l','l','o'
	,5, 0, 0, 0
	,'W','o','r','l','d'
	};

static_assert(data_le[sizeof(DataStore::KeyTypeCountValueDefs::KeyType) + sizeof(DataStore::KeyTypeCountValueDefs::ArraySize) ]==Compound::SupportedTypes::getTypeIndex<Compound>());

int main()
	{
	Compound compound;
	DataStore::MemReader memReader
		{
		  reinterpret_cast<std::byte const*>(std::begin(data_le))
		, reinterpret_cast<std::byte const*>(std::end(data_le))
		};

	auto status = DataStore::KeyTypeCountValueDeserializer{NativeDecoder<DataStore::MemReader>{memReader}}(compound);
	assert(status == DataStore::StatusCode::Success);
	assert(memReader.eof());

	assert(compound.childCount() == 3);


	assert(DataStore::get<int>(compound, DataStore::KeyTypeCountValueDefs::KeyType{"A"}
		,DataStore::KeyTypeCountValueDefs::KeyType{"a1"})==0x4030201);
	assert(DataStore::get<std::string>(compound, DataStore::KeyTypeCountValueDefs::KeyType{"A"}
		,DataStore::KeyTypeCountValueDefs::KeyType{"a2"}) == "Foobar");

		{
		auto& v = DataStore::get<std::vector<Compound>>(compound, DataStore::KeyTypeCountValueDefs::KeyType{"B"});
		assert(v.size() == 2);
		assert(v[0].get<int>(DataStore::KeyTypeCountValueDefs::KeyType{"B01"}) == 0x4030202);
		assert(v[1].childCount() == 0);
		}

		{
		auto& v = DataStore::get<std::vector<std::string>>(compound,  DataStore::KeyTypeCountValueDefs::KeyType{"C"});
		assert(v.size() == 2);
		assert(v[0] == "Hello");
		assert(v[1] == "World");
		}
	}

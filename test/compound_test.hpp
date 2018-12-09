//@	{"targets":[{"name":"compound_test.hpp","type":"include"}]}

#ifndef COMPOUND_TEST_H
#define COMPOUND_TEST_H

#include "basic_compound.hpp"
#include <string>
#include <vector>

namespace Test
	{
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

		template<class StatusCode, class Deserializer>
		[[noreturn]]
		static void readError(StatusCode status, Deserializer&&)
			{throw status;}
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
		,'b','0', '1', 0, 0, 0, 0, 0
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

	Compound createTestCompound()
		{
		std::vector<Compound> B;
		B.push_back(Compound{}.insert(DataStore::KeyTypeCountValueDefs::KeyType{"b01"}, 0x4030202));
		B.push_back(Compound{});
		std::vector<std::string> C{"Hello", "World"};

		Compound ret;
		ret.insert(DataStore::KeyTypeCountValueDefs::KeyType{"A"}, Compound{}
				.insert(DataStore::KeyTypeCountValueDefs::KeyType{"a1"}, 0x4030201)
				.insert(DataStore::KeyTypeCountValueDefs::KeyType{"a2"}, std::string("Foobar")))
			.insert(DataStore::KeyTypeCountValueDefs::KeyType{"B"}, std::move(B))
			.insert(DataStore::KeyTypeCountValueDefs::KeyType{"C"}, std::move(C));

		return ret;
		}
	}

#endif

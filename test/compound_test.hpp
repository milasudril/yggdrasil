//@	{"targets":[{"name":"compound_test.hpp","type":"include"}]}

#ifndef COMPOUND_TEST_H
#define COMPOUND_TEST_H

#include "../basic_compound.hpp"
#include "../key_type_count_value_defs.hpp"
#include <string>
#include <vector>

namespace Test
	{
	using KeyType = DataStore::KeyTypeCountValueDefs::KeyType;
	using ArraySize = DataStore::KeyTypeCountValueDefs::KeyType;

	struct MyExceptionPolicy
		{
		[[noreturn]]
		static void keyNotFound(KeyType key)
			{throw key;}

		[[noreturn]]
		static void keyValueHasWrongType(KeyType key, size_t actualType)
			{throw key;}

		[[noreturn]]
		static void keyAlreadyExists(KeyType key)
			{throw key;}

/*		template<class StatusCode, class Deserializer>
		[[noreturn]]
		static void readError(StatusCode status, Deserializer&&)
			{throw status;} */
		};

	struct Empty{};


	using Compound = DataStore::BasicCompound<MyExceptionPolicy, KeyType, std::string, int, Empty>;

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

	Compound createTestCompound()
		{
		std::vector<Compound> B;
		B.push_back(Compound{}.insert(KeyType{"b01"}, 0x4030202));
		B.push_back(Compound{});
		std::vector<std::string> C{"Hello", "World"};

		Compound ret;
		ret.insert(KeyType{"A"}, Compound{}
				.insert(KeyType{"a1"}, 0x4030201)
				.insert(KeyType{"a2"}, std::string("Foobar")))
			.insert(KeyType{"B"}, std::move(B))
			.insert(KeyType{"C"}, std::move(C));

		return ret;
		}
	}

#endif

//@	{"targets":[{"name":"compound.test","type":"application","autorun":1}]}

#include "compound.hpp"

#include "stic/stic.hpp"

#include <string>

struct MyExceptionPolicy
	{
	static void keyNotFound(const std::string& key)
		{throw key;}

	template<class T>
	static void keyValueHasWrongType(const std::string& key, size_t actualType)
		{throw key;}


	template<class T>
	static void keyAlreadyExists(const std::string& key, T const& value)
		{throw key;}
	};

STIC_TESTCASE("Visit items recursive")
	{
	std::vector<std::string> keys_expected{"foo", "key", "subobj", "value in subobj", "value2 in subobj", "third key"};

	using Compound = DataStore::Compound<MyExceptionPolicy, std::string, int>;

	Compound obj;

	obj.insert(keys_expected[0], std::string("bar"))
		.insert(keys_expected[1], 1)
		.insert(keys_expected[2], Compound{}
			.insert(keys_expected[3], int{34})
			.insert(keys_expected[4], int{14}))
		.insert(keys_expected[5], std::vector<int>{1, 2, 3});

// 	obj.visitItems<DataStore::RecursiveItemVisitor>(VisitItemsRecursive{std::move(keys_expected)});
	}


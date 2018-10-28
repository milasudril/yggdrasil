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

using Compound = DataStore::Compound<MyExceptionPolicy, std::string, int>;

struct MyVisitor
	{
	MyVisitor(){printf("{");}

	void operator()(std::string const& key, std::vector<int> const& value)
		{
		printf("%s:i32a{", key.c_str());
		std::for_each(value.begin(),value.end(), [](auto val){printf("{%d}", val);});
		printf("}\n");
		}

	void operator()(std::string const& key, std::vector<std::string> const& value)
		{
		printf("%s:stra{", key.c_str());
		std::for_each(value.begin(),value.end(), [](auto const& val){printf("{%s}", val.c_str());});
		printf("}\n");
		}

	void operator()(std::string const& key, std::vector<Compound> const& value)
		{
		printf("%s:obja{", key.c_str());
		std::for_each(value.begin(),value.end(), [this](auto const& val)
			{
			printf("{");
			val.visitItems(*this);
			printf("}\n");
			});
		printf("}\n");
		}

	void operator()(std::string const& key, int value)
		{
		printf("%s:i32{%d}\n", key.c_str(), value);
		}

	void operator()(std::string const& key, std::string const& value)
		{
		printf("%s:str{%s}\n", key.c_str(), value.c_str());
		}

	void operator()(std::string const& key, Compound const& value)
		{
		printf("%s:obj{",key.c_str());
		value.visitItems(*this);
		printf("}\n");
		}

	~MyVisitor(){printf("}\n");}
	};


STIC_TESTCASE("Visit items recursive")
	{
	std::vector<std::string> keys_expected{"foo", "key", "subobj", "value in subobj", "value2 in subobj", "third key"};


	Compound obj;

	obj.insert(keys_expected[0], std::string("bar"))
		.insert(keys_expected[1], 1)
		.insert(keys_expected[2], Compound{}
			.insert(keys_expected[3], int{34})
			.insert(keys_expected[4], int{14}))
		.insert(keys_expected[5], std::vector<int>{1, 2, 3});

	obj.visitItems(MyVisitor{});

	obj.insertOrReplace(keys_expected[2], Compound{}.insert(keys_expected[3], int{40}));
	}


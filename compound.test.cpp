//@	{"targets":[{"name":"compound.test","type":"application","autorun":1}]}

#include "compound.hpp"

#include "stic/stic.hpp"

#include <string>

struct MyExceptionPolicy
	{
	static void keyNotFound(std::string const& key)
		{throw key;}

	template<class T>
	static void keyValueHasWrongType(std::string const& key, size_t actualType)
		{throw key;}


	template<class T>
	static void keyAlreadyExists(std::string const& key, T const& value)
		{throw key;}
	};

using Compound = DataStore::Compound<MyExceptionPolicy, std::string, int>;

Compound makeSut(std::vector<std::string> const& keys_expected)
	{
	Compound obj;

	obj.insert(keys_expected[0], std::string("bar"))
		.insert(keys_expected[1], 1)
		.insert(keys_expected[2], Compound{}
			.insert(keys_expected[3], int{34})
			.insert(keys_expected[4], int{14}))
		.insert(keys_expected[5], std::vector<int>{1, 2, 3});
	return obj;
	}

struct MyVisitor
	{
	MyVisitor(std::vector<std::string> const& keys_expected): m_key_current{keys_expected.begin()}
		{printf("{");}

	void operator()(std::string const& key, std::vector<int> const& value)
		{
		STIC_ASSERT(*m_key_current == key);
		++m_key_current;
		printf("%s:i32a{", key.c_str());
		std::for_each(value.begin(),value.end(), [](auto val){printf("{%d}", val);});
		printf("}\n");
		}

	void operator()(std::string const& key, std::vector<std::string> const& value)
		{
		STIC_ASSERT(*m_key_current == key);
		++m_key_current;
		printf("%s:stra{", key.c_str());
		std::for_each(value.begin(),value.end(), [](auto const& val){printf("{%s}", val.c_str());});
		printf("}\n");
		}

	void operator()(std::string const& key, std::vector<Compound> const& value)
		{
		STIC_ASSERT(*m_key_current == key);
		++m_key_current;
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
		STIC_ASSERT(*m_key_current == key);
		++m_key_current;
		printf("%s:i32{%d}\n", key.c_str(), value);
		}

	void operator()(std::string const& key, std::string const& value)
		{
		STIC_ASSERT(*m_key_current == key);
		++m_key_current;
		printf("%s:str{%s}\n", key.c_str(), value.c_str());
		}

	void operator()(std::string const& key, Compound const& value)
		{
		STIC_ASSERT(*m_key_current == key);
		++m_key_current;
		printf("%s:obj{",key.c_str());
		value.visitItems(*this);
		printf("}\n");
		}

	~MyVisitor(){printf("}\n");}

	std::vector<std::string>::const_iterator  m_key_current;
	};


STIC_TESTCASE("Visit items recursive")
	{
	std::vector<std::string> keys_expected{"foo", "key", "subobj", "value in subobj", "value2 in subobj", "third key"};

	auto obj = makeSut(keys_expected);

	obj.visitItems(MyVisitor{keys_expected});
	}


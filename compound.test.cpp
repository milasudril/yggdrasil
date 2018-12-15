//@	{"targets":[{"name":"compound.test","type":"application","autorun":1}]}

#include "compound.hpp"

#include "stic/stic.hpp"

#include <string>

struct MyExceptionPolicy
	{
	[[noreturn]]
	static void keyNotFound(std::string_view key)
		{throw std::string{key};}

	template<class T>
	[[noreturn]]
	static void keyValueHasWrongType(std::string_view key, size_t actualType)
		{throw std::string{key};}

	template<class T>
	[[noreturn]]
	static void keyAlreadyExists(std::string const& key, T const& value)
		{throw key;}
	};

using Compound = DataStore::Compound<MyExceptionPolicy, std::string, int>;

Compound makeSut()
	{
	Compound obj;

	obj.insert("foo", std::string("bar"))
		.insert("key", 1)
		.insert("subobj", Compound{}
			.insert("value in subobj", int{34})
			.insert("value2 in subobj", int{14}))
		.insert("third key", std::vector<int>{1, 2, 3});
	return obj;
	}


STIC_TESTCASE("childCount")
	{
	auto sut = makeSut();
	auto const n_children_ref = sut.childCount();
	std::remove_const_t<decltype(n_children_ref)> n_children_counted{0};
	sut.visitItems([&n_children_counted](auto const&, auto const&)
		{
		++n_children_counted;
		});

	STIC_ASSERT(n_children_counted == n_children_ref);
	}

STIC_TESTCASE("erase")
	{
	auto sut = makeSut();
	auto const n_children_ref = sut.childCount();
	STIC_ASSERT(sut.contains("subobj"));
	STIC_ASSERT(sut.erase("subobj"));
	STIC_ASSERT(!sut.erase("subobj"));
	STIC_ASSERT(!sut.contains("subobj"));
	STIC_ASSERT(n_children_ref == sut.childCount() + 1);
	}

STIC_TESTCASE("erase in subobj")
	{
	auto sut = makeSut();
	auto const n_children_ref = sut.childCount();
	STIC_ASSERT(contains(sut, "subobj", "value in subobj"));
	STIC_ASSERT(erase(sut, "subobj", "value in subobj"));
	STIC_ASSERT(!erase(sut, "subobj", "value in subobj"));
	STIC_ASSERT(!contains(sut ,"subobj", "value in subobj"));
	STIC_ASSERT(n_children_ref == sut.childCount() );
	}

STIC_TESTCASE("get")
	{
	auto sut = makeSut();
	auto const n_children_ref = sut.childCount();
	STIC_ASSERT(sut.get<int>("key") == 1);
	STIC_ASSERT(sut.get<std::string>("foo") == "bar");
	STIC_ASSERT(DataStore::get<int>(sut, "subobj", "value in subobj") == 34);
	STIC_ASSERT(n_children_ref == sut.childCount());
	}

STIC_TESTCASE("insert")
	{
	auto sut = makeSut();
	auto const n_children_ref = sut.childCount();
	STIC_ASSERT(!sut.contains("Hello"));
	sut.insert("Hello", 123);
	STIC_ASSERT(sut.contains("Hello"));
	STIC_ASSERT(sut.get<int>("Hello") == 123);
	STIC_ASSERT(n_children_ref + 1 == sut.childCount() );
	}

STIC_TESTCASE("replace")
	{
	auto sut = makeSut();
	auto const n_children_ref = sut.childCount();
	STIC_ASSERT(sut.contains("subobj"));
	sut.insertOrReplace("subobj", 123);
	STIC_ASSERT(sut.contains("subobj"));
	STIC_ASSERT(sut.get<int>("subobj") == 123);
	STIC_ASSERT(n_children_ref == sut.childCount() );
	}

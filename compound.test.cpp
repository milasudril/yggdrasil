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

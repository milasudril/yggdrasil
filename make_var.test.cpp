//@	{"targets":[{"name":"make_var.test","type":"application","autorun":1}]}

#include "make_var.hpp"

#include "stic/stic.hpp"
#include <string>

namespace
	{
	using var_t = std::variant<std::unique_ptr<std::string>, int>;
	}

STIC_TESTCASE("empty")
	{
	auto empty = var_t{};
	STIC_ASSERT(DataStore::get_if<std::string>(&empty) == nullptr);
	STIC_ASSERT(DataStore::get_if<int>(&empty) == nullptr);
	}

STIC_TESTCASE("Use std::string")
	{
	auto val_a = DataStore::make_var<var_t>(std::string{"Hello"});
	auto content = DataStore::get_if<std::string>(&val_a);
	STIC_ASSERT(content != nullptr);
	STIC_ASSERT(*content == "Hello");
	STIC_ASSERT(DataStore::get_if<int>(&val_a) == nullptr);

	std::string non_temp{"World"};
	auto val_b = DataStore::make_var<var_t>(non_temp);
	content = DataStore::get_if<std::string>(&val_b);
	STIC_ASSERT(content !=  nullptr);
	STIC_ASSERT(*content == "World");
	}

STIC_TESTCASE("Use int")
	{
	auto val_a = DataStore::make_var<var_t>(123);
	auto content = DataStore::get_if<int>(&val_a);
	STIC_ASSERT(content != nullptr);
	STIC_ASSERT(*content == 123);
	STIC_ASSERT(DataStore::get_if<std::string>(&val_a) == nullptr);
	}


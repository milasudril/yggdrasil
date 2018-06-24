//@	{"targets":[{"name":"make_default.test","type":"application","autorun":1}]}

#include "make_default.hpp"

#include "stic/stic.hpp"

#include "strint/integer.hpp"
#include <string>

STIC_TESTCASE("Strint thing")
	{
	auto val = DataStore::make_default<Strint::Integer<>>();
	STIC_ASSERT(val == 0);
	}

STIC_TESTCASE("Default constructible")
	{
	auto val = DataStore::make_default<std::string>();
	STIC_ASSERT(val.empty());
	}

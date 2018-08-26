//@	{"targets":[{"name":"value_wrapper.test","type":"application","autorun":1}]}

#include "value_wrapper.hpp"

#include "stic/stic.hpp"

#include <utility>

STIC_TESTCASE("ValueWrapper direct container")
	{
	DataStore::ValueWrapper<std::pair<int, int>> val{1,2};
	STIC_ASSERT( (val.get() == std::pair<int,int>{1,2}) );
	}

STIC_TESTCASE("ValueWrapper unique_ptr")
	{
	DataStore::ValueWrapper<std::unique_ptr<std::pair<int,int>>> val{1,2};
	STIC_ASSERT( (val.get() == std::pair<int,int>{1,2}) );
	}

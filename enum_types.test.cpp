//@	{"targets":[{"name":"enum_types.test","type":"application","autorun":1}]}

#include "enum_types.hpp"

#include "stic/stic.hpp"

namespace
	{
	class TypeVisitor
		{
		public:
			template<class T>
			void visit()
				{}
		};
	}

STIC_TESTCASE("List all types")
	{
	using var = std::variant<int, double, std::string>;
	DataStore::enumTypes<var>(TypeVisitor{});
	}

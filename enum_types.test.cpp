//@	{"targets":[{"name":"enum_types.test","type":"application","autorun":1}]}

#include "enum_types.hpp"

#include "strint/common_types.hpp"
#include "stic/stic.hpp"
#include <cstring>

namespace
	{
	constexpr char const* valid_names[]={"i20", "i10", "i40"};
	class TypeVisitor
		{
		public:
			template<class T, int N>
			void visit()
				{STIC_ASSERT(strcmp(T::typeName(), valid_names[N]) == 0);}
		};
	}

STIC_TESTCASE("List all types")
	{
	using var = std::variant<Strint::Int32, Strint::Int16, Strint::Int64>;
	DataStore::enumTypes<var>(TypeVisitor{});
	}

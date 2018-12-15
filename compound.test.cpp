//@	{"targets":[{"name":"compound.test","type":"application","autorun":1}]}

#include "compound.hpp"
#include "stic/stic.hpp"

struct MyType
	{
	int a;
	int b;
	};

DataStore::var_t make_var(const MyType& self)
	{
	DataStore::Compound obj;
	obj.set("a", Strint::Int32{self.a})
		.set("b", Strint::Int32{self.b});
	return make_var(std::move(obj));
	}

STIC_TESTCASE("Add stuff")
	{
	DataStore::Compound obj;
	
	obj.set("a", Strint::Int32{123});
		{
		auto foo = DataStore::make_var(std::string{"foo"});
		obj.set("foo", std::move(foo));
		}
	
	std::string non_temporary_string{"non-temp"};
	
	obj.set("bar", std::string{"bar"})
		.set("non_temporary_string", non_temporary_string)
		.set("c style string", "c str");
		
	DataStore::Compound obj2;
	obj2.set("obj", std::move(obj))
		.set("my_type", MyType{1,2});
	
	DataStore::Compound const& obj_ref = obj2;
		
	auto my_type = obj_ref.getIf<DataStore::Compound>("my_type");
	auto a = my_type->getIf<Strint::Int32>("a");
	
	printf("a = %d\n", static_cast<int>(*a));
	
	printf("a = %d\n"
		,static_cast<int>(obj2.get<DataStore::Compound>("my_type").get<Strint::Int32>("a")));
	}

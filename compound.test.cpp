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
	
STIC_TESTCASE("Add values of builtin types")
	{
	using namespace DataStore;
	Compound obj;
	
//	Add stuff to obj
	char const* keys[] = 
		{
		 "Int8"
		,"Int16"
		,"Int32"
		,"Int64"
		,"UInt8"
		,"UInt16"
		,"UInt32"
		,"UInt64"
		,"Float"
		,"Double"
		,"String"
		
		,"Int8 array"
		,"Int16 array"
		,"Int32 array"
		,"Int64 array"
		,"UInt8 array"
		,"UInt16 array"
		,"UInt32 array"
		,"UInt64 array"
		,"Float array"
		,"Double array"
		,"String array"
		
		,"Compound"
		,"Compound array"
		};
	
	obj.set(keys[0], Int8{1})
		.set(keys[1], Int16{2})
		.set(keys[2], Int32{3})
		.set(keys[3], Int64{4})
		.set(keys[4], UInt8{8})
		.set(keys[5], UInt16{16})
		.set(keys[6], UInt32{32})
		.set(keys[7], UInt64{64})
		.set(keys[8], Float{1.0f})
		.set(keys[9], Double{2.0})
		.set(keys[10], String{"Hello, World"})
		
		.set(keys[11], Array<Int8> {Int8{1},Int8{2}})
		.set(keys[12], Array<Int16>{Int16{1},Int16{2},Int16{3}})
		.set(keys[13], Array<Int32>{1,2,3,4})
		.set(keys[14], Array<Int64>{1,2,3,4,5})
		.set(keys[15], Array<UInt8> {UInt8{1},UInt8{2},UInt8{3},UInt8{4},UInt8{5},UInt8{6}})
		.set(keys[16], Array<UInt16>{UInt16{1},UInt16{2},UInt16{3},UInt16{4},UInt16{5},UInt16{6},UInt16{7}})
		.set(keys[17], Array<UInt32>{1u,2u,3u,4u,5u,6u,7u,8u})
		.set(keys[18], Array<UInt64>{1u,2u,3u,4u,5u,6u,7u,8u,9u})
		.set(keys[19], Array<Float>{1.0f, 2.0f})
		.set(keys[20], Array<Double>{1.0, 2.0, 3.0})
		.set(keys[21], Array<String>{"Foo", "Bar"});
	
		{
		Compound temp;
		temp.set("Sub-key", "Foo");
		
		obj.set(keys[22], std::move(temp));
		}
	
		{
		Array<Compound> array;
		array.push_back(Compound{});
		array.push_back(Compound{});
		obj.set(keys[23], std::move(array));
		}
	

//	Check that all keys have been added, and remove them one by one
	static_assert(sizeof(keys) > 8);
	std::for_each(std::begin(keys), std::end(keys), [&obj](const auto& key)
		{
		auto N = obj.size();
		STIC_ASSERT(obj.typeOfValue(key) != Compound::invalidKey());
		obj.remove(key);
		STIC_ASSERT(obj.typeOfValue(key) == Compound::invalidKey());
		STIC_ASSERT(obj.size() == N-1);
		});
	STIC_ASSERT(obj.size() == 0);
	STIC_ASSERT(obj.empty());
	}

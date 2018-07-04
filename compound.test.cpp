//@	{"targets":[{"name":"compound.test","type":"application","autorun":1}]}

#include "compound.hpp"
#include "make_default.hpp"
#include "enum_types.hpp"

#include "stic/stic.hpp"

#include <type_traits>

class ValInserter
	{
	public:
		template<class T, int N>
		void visit()
			{
			using OtherType = std::remove_reference_t<decltype(*std::get_if<N==0? 1 : N - 1>(static_cast<DataStore::var_t*>(nullptr)))>;
			
			DataStore::Compound obj;
			STIC_ASSERT(obj.empty());

			STIC_ASSERT(obj.insert("key 1", DataStore::make_default<T>()));
			STIC_ASSERT(!obj.insert("key 1", DataStore::make_default<T>()));
			STIC_ASSERT(!obj.insert("key 1", DataStore::make_default<OtherType>()));
			STIC_ASSERT(obj.find("key 1"));
			
			STIC_ASSERT(!obj.find("key 2"));
			STIC_ASSERT(obj.insert("key 2", DataStore::make_default<T>()));
			STIC_ASSERT(obj.find("key 2"));
			
			STIC_ASSERT(!obj.find("key 3"));
			obj.set("key 3", DataStore::make_default<T>());
			STIC_ASSERT(obj.find("key 3"));
			obj.set("key 2", DataStore::make_default<T>());
			
			STIC_ASSERT(obj.typeOfValue("key 3") == N);
			STIC_ASSERT(obj.typeOfValue("bad key") == std::variant_npos);
			
			STIC_ASSERT(obj.get("key 3").index() == N);
			STIC_ASSERT_THROW(obj.get("bad key"););
			
			STIC_ASSERT(obj.getIf<T>("key 3"));
			STIC_ASSERT(!obj.getIf<OtherType>("key 3"));
			obj.set("key 3", DataStore::make_default<OtherType>());
			STIC_ASSERT(obj.getIf<OtherType>("key 3"));
			STIC_ASSERT(!obj.getIf<T>("key 3"));
			
			STIC_ASSERT_NOTHROW(obj.get<T>("key 2"););
			STIC_ASSERT_THROW(obj.get<T>("key 3"););
			
			STIC_ASSERT(obj.size() == 3);
			obj.remove("key 2");
			STIC_ASSERT(!obj.find("key 2"));
			STIC_ASSERT(obj.size() == 2);
			STIC_ASSERT(obj.insert("key 2", DataStore::make_default<T>()));
			STIC_ASSERT(obj.size() == 3);
			}
	};

STIC_TESTCASE("Set and get values")
	{
	DataStore::enumTypes<DataStore::var_t>(ValInserter{});
	}

STIC_TESTCASE("Set and get string")
	{
	DataStore::Compound obj;
	obj.set("foo", DataStore::String("bar"));
	STIC_ASSERT(obj.get<DataStore::String>("foo") == "bar");
	}
	
	
STIC_TESTCASE("Set and get compound")
	{
	DataStore::Compound obj;
	obj.set("foo", DataStore::Compound{});
	STIC_ASSERT_NOTHROW(obj.get<DataStore::Compound>("foo"););
	}

STIC_TESTCASE("Visit items")
	{
	DataStore::Compound obj;
	obj.set("foo", DataStore::String("bar"))
		.set("key", DataStore::Int32{0})
		.set("subobj", DataStore::Compound{});
	
	obj.visitItems([](auto const item)
		{
		printf("%s ",item.first.c_str());
		});
	}


#if 0

template<int N = std::variant_size_v<DataStore::var_t>, class Function>
void process(Function&& f)
	{
	process<N-1>(std::forward<Function>(f));
	f(createDefault< decltype( std::get<N-1>(DataStore::var_t{}) ) >());
	}

template<class Function>
void process<0,Function>(Function&&)
	{}





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

STIC_TESTCASE("Test builtin types")
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
		.set(keys[4], UInt8{5})
		.set(keys[5], UInt16{6})
		.set(keys[6], UInt32{7})
		.set(keys[7], UInt64{8})
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

//	Check that we can get values of all properties
	STIC_ASSERT(obj.get<Int8>(keys[0]) == Int8{1});
	STIC_ASSERT(obj.get<Int16>(keys[1]) == Int16{2});
	STIC_ASSERT(obj.get<Int32>(keys[2]) == Int32{3});
	STIC_ASSERT(obj.get<Int64>(keys[3]) == Int64{4});
	STIC_ASSERT(obj.get<UInt8>(keys[4]) == UInt8{5});
	STIC_ASSERT(obj.get<UInt16>(keys[5]) == UInt16{6});
	STIC_ASSERT(obj.get<UInt32>(keys[6]) == UInt32{7});
	STIC_ASSERT(obj.get<UInt64>(keys[7]) == UInt64{8});
	STIC_ASSERT(obj.get<Float>(keys[8]) == 1.0f);
	STIC_ASSERT(obj.get<Double>(keys[9]) == 2.0f);
	STIC_ASSERT(obj.get<String>(keys[10]) == "Hello, World");

	STIC_ASSERT(obj.get<Array<Int8>>(keys[11]) == (Array<Int8>{Int8{1},Int8{2}}));
	STIC_ASSERT(obj.get<Array<Int16>>(keys[12]) == (Array<Int16>{Int16{1},Int16{2},Int16{3}}));
	STIC_ASSERT(obj.get<Array<Int32>>(keys[13]) == (Array<Int32>{1,2,3,4}));
	STIC_ASSERT(obj.get<Array<Int64>>(keys[14]) == (Array<Int64>{1,2,3,4,5}));
	STIC_ASSERT(obj.get<Array<UInt8>>(keys[15]) ==
		(Array<UInt8>{UInt8{1},UInt8{2},UInt8{3},UInt8{4},UInt8{5},UInt8{6}}));
	STIC_ASSERT(obj.get<Array<UInt16>>(keys[16]) ==
		(Array<UInt16>{UInt16{1},UInt16{2},UInt16{3},UInt16{4},UInt16{5},UInt16{6},UInt16{7}}));
	STIC_ASSERT(obj.get<Array<UInt32>>(keys[17]) == (Array<UInt32>{1u,2u,3u,4u,5u,6u,7u,8u}));
	STIC_ASSERT(obj.get<Array<UInt64>>(keys[18]) == (Array<UInt64>{1u,2u,3u,4u,5u,6u,7u,8u,9u}));
	STIC_ASSERT(obj.get<Array<Float>>(keys[19]) == (Array<Float>{1.0f, 2.0f}));
	STIC_ASSERT(obj.get<Array<Double>>(keys[20]) == (Array<Double>{1.0, 2.0, 3.0}));
	STIC_ASSERT(obj.get<Array<String>>(keys[21]) == (Array<String>{"Foo", "Bar"}));

	STIC_ASSERT(obj.get<Compound>(keys[22]).get<String>("Sub-key") == "Foo");
	STIC_ASSERT(obj.get<Array<Compound>>(keys[23]).size() == 2);

	std::for_each(obj.begin(), obj.end(), [](const auto& val)
		{
		printf("%s %s\n", val.first.c_str(), std::visit([](const auto& var)
			{
			return getTypeName<std::decay_t<decltype(var)>>();
			},val.second));
		});

//	Now remove all keys
	static_assert(sizeof(keys) > 8);
	std::for_each(std::begin(keys), std::end(keys), [&obj](const auto& key)
		{
		auto N = obj.size();
		obj.remove(key);
		STIC_ASSERT(obj.typeOfValue(key) == Compound::invalidKey());
		STIC_ASSERT(obj.size() == N-1);
		});
	STIC_ASSERT(obj.size() == 0);
	STIC_ASSERT(obj.empty());
	}
#endif

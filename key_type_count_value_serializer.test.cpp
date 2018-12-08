//@	{"targets":[{"name":"key_type_count_value_serializer.test","type":"application","autorun":1}]}

#include "key_type_count_value_serializer.hpp"

#include "basic_compound.hpp"
#include "mem_writer.hpp"
#include "native_encoder.hpp"

#include <type_traits>

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
	static void keyAlreadyExists(DataStore::KeyTypeCountValueDefs::KeyType key, T const& value)
		{throw key;}
	};

using Compound = DataStore::BasicCompound<MyExceptionPolicy, DataStore::KeyTypeCountValueDefs::KeyType, std::string, int>;

Compound makeSut()
	{
	Compound obj;
	obj.insert("foo", std::string("bar"))
		.insert("key", 1)
		.insert("subobj", Compound{}
			.insert("value in subobj", int{34})
			.insert("value2 in subobj", int{14}))
		.insert("third key", std::vector<int>{1, 2, 3})
		.insert("array of strings", std::vector<std::string>{"Hello", "World"});
	return obj;
	}

int main()
	{
	auto sut = makeSut();
	std::vector<std::byte> output_buffer;
	DataStore::MemWriter writer{output_buffer};

	DataStore::NativeEncoder encoder(writer);

	assert(sut.visitItems(DataStore::KeyTypeCountValueSerializer
		<Compound, DataStore::NativeEncoder<DataStore::MemWriter>>{DataStore::NativeEncoder{writer}}));

	std::for_each(output_buffer.begin(), output_buffer.end(), [](auto x){putchar(static_cast<char>(x));});
	}

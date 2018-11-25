//@	{"targets":[{"name":"key_type_count_value_writer.test","type":"application","autorun":1}]}

#include "key_type_count_value_writer.hpp"

#include "basic_compound.hpp"

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
	static void keyAlreadyExists(DataStore::KeyTypeCountValue::KeyType key, T const& value)
		{throw key;}
	};

struct Writer
	{
	template<class T>
	std::enable_if_t<std::is_arithmetic_v<T>, Writer>&
	write(T value)
		{return *this;}

	template<class T>
	std::enable_if_t<!std::is_arithmetic_v<T>
		&& std::is_trivially_default_constructible_v<T>
		&& std::is_standard_layout_v<T>, Writer>&
	write(T const& data_block)
		{return *this;}

	template<class T>
	std::enable_if_t<std::is_arithmetic_v<T>, Writer>&
	write(T const* value, size_t N)
		{return *this;}

	template<class T>
	std::enable_if_t<!std::is_arithmetic_v<T>
		&& std::is_trivially_default_constructible_v<T>
		&& std::is_standard_layout_v<T>, Writer>&
	write(T const& data_block, size_t N)
		{return *this;}
	};

using Compound = DataStore::BasicCompound<MyExceptionPolicy, DataStore::KeyTypeCountValue::KeyType, std::string, int>;

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

	sut.visitItems(DataStore::KeyTypeCountValueWriter<Compound, Writer>{Writer{}});
	}

//@	{"targets":[{"name":"key_type_count_value_reader.test","type":"application","autorun":1}]}

#include "key_type_count_value_reader.hpp"

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

struct Reader
	{
	template<class T>
	std::enable_if_t<std::is_arithmetic_v<T>, bool>
	read(T& value)
		{return true;}

	template<class T>
	std::enable_if_t<!std::is_arithmetic_v<T>
		&& std::is_trivially_default_constructible_v<T>
		&& std::is_standard_layout_v<T>, bool>
	read(T& data_block)
		{return true;}

	template<class T>
	std::enable_if_t<std::is_arithmetic_v<T>, bool>
	read(T* value, size_t N)
		{return true;}

	template<class T>
	std::enable_if_t<!std::is_arithmetic_v<T>
		&& std::is_trivially_default_constructible_v<T>
		&& std::is_standard_layout_v<T>, bool>
	read(T& data_block, size_t N)
		{return true;}
	};

using Compound = DataStore::BasicCompound<MyExceptionPolicy, DataStore::KeyTypeCountValue::KeyType, std::string, int>;


int main()
	{
	Compound compound;

	assert(DataStore::read(Reader{}, compound) == DataStore::StatusCode::Success);

	printf("%zu\n", compound.childCount());
	}

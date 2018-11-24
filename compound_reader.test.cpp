//@	{"targets":[{"name":"compound_reader.test","type":"application","autorun":1}]}

#include "compound_reader.hpp"
#include "read.hpp"
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
	static void keyAlreadyExists(Analib::InlineString<char, 16> const& key, T const& value)
		{throw key;}

	};

struct Reader
	{
	template<class T>
	std::enable_if_t<std::is_arithmetic_v<T>, bool>
	read(T& value);

	template<class T>
	std::enable_if_t<!std::is_arithmetic_v<T>
		&& std::is_trivially_default_constructible_v<T>
		&& std::is_standard_layout_v<T>, bool>
	read(T& data_block);

	template<class T>
	std::enable_if_t<std::is_arithmetic_v<T>, bool>
	read(T* value, size_t N);

	template<class T>
	std::enable_if_t<!std::is_arithmetic_v<T>
		&& std::is_trivially_default_constructible_v<T>
		&& std::is_standard_layout_v<T>, bool>
	read(T& data_block, size_t N);
	};

using Compound = DataStore::BasicCompound<MyExceptionPolicy, Analib::InlineString<char, 16>, std::string, int>;


int main()
	{
	Compound compound;
	assert(readAll(DataStore::BinaryCompoundDecoder{Reader{}, compound}) == DataStore::StatusCode::EndOfFile);
	}

//@	{"targets":[{"name":"compound.test","type":"application", "autorun":1}]}

#include "compound.hpp"

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
	static void keyAlreadyExists(std::string const& key, T const& value)
		{throw key;}
	};

int main()
	{
	Yggdrasil::Compound<MyExceptionPolicy> test;
	return 0;
	}

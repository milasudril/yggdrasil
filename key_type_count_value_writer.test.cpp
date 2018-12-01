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

#include <iostream>

class OutputMock
	{
	public:
		void write(void const* buffer, size_t count)
			{
			auto bytes = reinterpret_cast<std::byte const*>(buffer);
			while(count!=0)
				{
				m_buffer.push_back(*bytes);
				++bytes;
				--count;
				}
			}

		auto begin() const
			{return m_buffer.begin();}

		auto end() const
			{return m_buffer.end();}

	private:
		std::vector<std::byte> m_buffer;
	};

template<class Sink>
struct Writer
	{
	explicit Writer(Sink& sink) : r_sink(sink) {}

	template<class T>
	std::enable_if_t<std::is_arithmetic_v<T>, Writer>&
	write(T value)
		{
		r_sink.write(&value, sizeof(T));
		return *this;
		}

	template<class T>
	std::enable_if_t<!std::is_arithmetic_v<T>
		&& std::is_trivially_default_constructible_v<T>
		&& std::is_standard_layout_v<T>, Writer>&
	write(T const& data_block)
		{
		r_sink.write(&data_block, sizeof(T));
		return *this;
		}

	template<class T>
	std::enable_if_t<std::is_arithmetic_v<T>, Writer>&
	write(T const* value, size_t N)
		{
		r_sink.write(value, N*sizeof(T));
		return *this;
		}

	template<class T>
	std::enable_if_t<!std::is_arithmetic_v<T>
		&& std::is_trivially_default_constructible_v<T>
		&& std::is_standard_layout_v<T>, Writer>&
	write(T const* data_block, size_t N)
		{return *this;}

	Sink& r_sink;
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
	OutputMock output;
	auto sut = makeSut();

	sut.visitItems(DataStore::KeyTypeCountValueWriter<Compound, Writer<OutputMock>>{Writer{output}});

	std::for_each(output.begin(), output.end(), [](auto x){putchar(static_cast<char>(x));});
	}

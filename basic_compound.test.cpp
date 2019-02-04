//@	{"targets":[{"name":"basic_compound.test","type":"application","autorun":1}]}

#include "basic_compound.hpp"
#include "test/compound_test.hpp"

#include "analib/inlinestring/inlinestring.hpp"
#include <string>

Test::Compound makeSut()
	{
	Test::Compound obj;

	obj.insert("foo", std::string("bar"))
		.insert("key", 1)
		.insert("subobj", Test::Compound{}
			.insert("value in subobj", int{34})
			.insert("value2 in subobj", int{14}))
		.insert("third key", std::vector<int>{1, 2, 3});
	return obj;
	}

int main()
	{
		{
		auto sut = makeSut();
		auto const n_children_ref = sut.childCount();
		std::remove_const_t<decltype(n_children_ref)> n_children_counted{0};
		assert(sut.visitItems([&n_children_counted](auto const&, auto const&)
			{
			++n_children_counted;
			return true;
			}));

		assert(n_children_counted == n_children_ref);
		}

		{
		auto sut = makeSut();
		auto const n_children_ref = sut.childCount();
		assert(sut.contains(Test::KeyType{"subobj"}));
		assert(sut.erase(Test::KeyType{"subobj"}));
		assert(!sut.erase(Test::KeyType{"subobj"}));
		assert(!sut.contains(Test::KeyType{"subobj"}));
		assert(n_children_ref == sut.childCount() + 1);
		}

		{
		auto sut = makeSut();
		auto const n_children_ref = sut.childCount();
		assert(contains(sut, Test::KeyType{"subobj"}, Test::KeyType{"value in subobj"}));
		assert(erase(sut, Test::KeyType{"subobj"}, Test::KeyType{"value in subobj"}));
		assert(!erase(sut, Test::KeyType{"subobj"}, Test::KeyType{"value in subobj"}));
		assert(!contains(sut ,Test::KeyType{"subobj"}, Test::KeyType{"value in subobj"}));
		assert(n_children_ref == sut.childCount() );
		}

		{
		auto sut = makeSut();
		auto const n_children_ref = sut.childCount();
		assert(sut.get<int>(Test::KeyType{"key"}) == 1);
		assert(sut.get<std::string>(Test::KeyType{"foo"}) == "bar");
		assert(DataStore::get<int>(sut, Test::KeyType{"subobj"}, Test::KeyType{"value in subobj"}) == 34);
		assert(n_children_ref == sut.childCount());
		}

		{
		auto sut = makeSut();
		auto const n_children_ref = sut.childCount();
		assert(!sut.contains(Test::KeyType{"Hello"}));
		sut.insert(Test::KeyType{"Hello"}, 123);
		assert(sut.contains(Test::KeyType{"Hello"}));
		assert(sut.get<int>(Test::KeyType{"Hello"}) == 123);
		assert(n_children_ref + 1 == sut.childCount() );
		}

		{
		auto sut = makeSut();
		auto const n_children_ref = sut.childCount();
		assert(sut.contains(Test::KeyType{"subobj"}));
		insert(123, sut, Test::KeyType{"subobj"}, Test::KeyType{"Hello"});
		assert(contains(sut, Test::KeyType{"subobj"}, Test::KeyType{"Hello"}));
		assert(DataStore::get<int>(sut, Test::KeyType{"subobj"}, Test::KeyType{"Hello"}));
		assert(n_children_ref == sut.childCount() );

		insertOrReplace(std::string("Boo"), sut, Test::KeyType{"subobj"}, Test::KeyType{"Hello"});
		assert(DataStore::get<std::string>(sut, Test::KeyType{"subobj"}, Test::KeyType{"Hello"}) == "Boo");
		}

		{
		auto sut = makeSut();
		auto const n_children_ref = sut.childCount();
		assert(sut.contains(Test::KeyType{"subobj"}));
		sut.insertOrReplace(Test::KeyType{"subobj"}, 123);
		assert(sut.contains(Test::KeyType{"subobj"}));
		assert(sut.get<int>(Test::KeyType{"subobj"}) == 123);
		assert(n_children_ref == sut.childCount() );
		}

		{
		auto sut = makeSut();
		assert(store(sut, [&sut](decltype(sut) const& compound)
			{
			assert(&sut == &compound);
			return true;
			}));
		}

		{
		Test::MyStatefulExceptionPolicy eh;
		Test::CompoundStatefulEh sut{eh};
		sut.insert(Test::KeyType{"Hello"}, Test::CompoundStatefulEh{eh});
		}
	}

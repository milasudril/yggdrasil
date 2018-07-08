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
	
void findMe(std::string const&)
{}

void findMe(DataStore::Compound const&)
{}

void findMe(DataStore::Int32)
{}

template<class T>
void findMe(T const&)
{}

class VisitItems
	{
	public:
		explicit VisitItems(std::vector<std::string>&& keys_expected) : m_keys_expected{std::move(keys_expected)}
			{m_position = m_keys_expected.begin();}
		
		template<class T>
		void visit(T const& item)
			{
			STIC_ASSERT(m_position != m_keys_expected.end());
			STIC_ASSERT(item.first == *m_position);
			findMe(item.second);
			++m_position;
			}
	private:
		std::vector<std::string> m_keys_expected;
		std::vector<std::string>::const_iterator m_position;
	};

STIC_TESTCASE("Visit items")
	{
	std::vector<std::string> keys_expected{"foo", "key", "subobj"};
		
	DataStore::Compound obj;
	obj.set(keys_expected[0], DataStore::String("bar"))
		.set(keys_expected[1], DataStore::Int32{0})
		.set(keys_expected[2], DataStore::Compound{}.set("value", DataStore::Int32{34}));
	
 	obj.visitItems<DataStore::ItemVisitor>(VisitItems{std::move(keys_expected)});
	}
	

	
class VisitItemsRecursive
	{
	public:
		explicit VisitItemsRecursive(std::vector<std::string>&& keys_expected) : m_keys_expected{std::move(keys_expected)}
			{m_position = m_keys_expected.begin();}
		
		template<class T>
		void visit(T item)
			{
			STIC_ASSERT(m_position != m_keys_expected.end());
			STIC_ASSERT(item.first == *m_position);
			++m_position;
			}

		void visit(std::pair<std::string const&, std::string const&> item)
			{
			STIC_ASSERT(m_position != m_keys_expected.end());
			STIC_ASSERT(item.first == *m_position);
			++m_position;
			}
			
			
		void compoundBegin(std::pair<std::string const&, DataStore::Compound const&> item)
			{
			STIC_ASSERT(m_position != m_keys_expected.end());
			STIC_ASSERT(item.first == *m_position);
			++m_position;
			}
		
		void compoundEnd()
			{}
		
		
	private:
		std::vector<std::string> m_keys_expected;
		std::vector<std::string>::const_iterator m_position;
	};

STIC_TESTCASE("Visit items recursive")
	{
	std::vector<std::string> keys_expected{"foo", "key", "subobj", "value in subobj", "value2 in subobj", "third key"};
		
	DataStore::Compound obj;
	obj.set(keys_expected[0], DataStore::String("bar"))
		.set(keys_expected[1], DataStore::Int32{0})
		.set(keys_expected[2], DataStore::Compound{}
			.set(keys_expected[3], DataStore::Int32{34})
			.set(keys_expected[4], DataStore::Int32{14}))
		.set(keys_expected[5], DataStore::Array<DataStore::Int32>{1, 2, 3});
	
 	obj.visitItems<DataStore::RecursiveItemVisitor>(VisitItemsRecursive{std::move(keys_expected)});	
	}


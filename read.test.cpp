//@	{"targets":[{"name":"read.test", "type":"application", "autorun":1}]}

#include "read.hpp"

#include <string>
#include <vector>
#include <cassert>

enum class StatusCode:int{Success, UnknownType, EndOfFile};

bool readFailed(StatusCode status)
	{return status!=StatusCode::Success;}

class MyReader
	{
	public:
		using SupportedTypes = DataStore::TypeSet<int, std::string, double>;

		bool checkDispatchOrder() const
			{
			std::vector<std::string> dispatch_order{"int", "string", "double"};
			return m_dispatch_order == dispatch_order;
			}

		enum class LastTypeMode{ReturnUnknownType, ReturnEndOfFile};

		explicit MyReader(LastTypeMode mode) : m_type(0), m_mode(mode){}

		auto readType()
			{
			auto ret = m_type;
			++m_type;
			if(m_mode == LastTypeMode::ReturnUnknownType)
				{return std::make_pair(ret, StatusCode::Success);}

			else
				{return std::make_pair(ret, ret<3 ? StatusCode::Success : StatusCode::EndOfFile);}
			}

		MyReader(MyReader const&) = delete;

		auto read(DataStore::Empty<std::string>)
			{
			m_dispatch_order.push_back("string");
			return StatusCode::Success;
			}

		auto read(DataStore::Empty<int>)
			{
			m_dispatch_order.push_back("int");
			return  StatusCode::Success;
			}

		auto read(DataStore::Empty<double>)
			{
			m_dispatch_order.push_back("double");
			return StatusCode::Success;
			}

		auto unknownType()
			{return StatusCode::UnknownType;}


	private:
		size_t m_type;
		LastTypeMode m_mode;

		std::vector<std::string> m_dispatch_order;
	};

int main()
	{
	assert(DataStore::read(MyReader{MyReader::LastTypeMode::ReturnUnknownType}) == StatusCode::Success);

	MyReader reader_a{MyReader::LastTypeMode::ReturnUnknownType};
	assert(DataStore::read(reader_a) == StatusCode::Success);
	assert(DataStore::read(reader_a) == StatusCode::Success);
	assert(DataStore::read(reader_a) == StatusCode::Success);
	assert(DataStore::read(reader_a) == StatusCode::UnknownType);
	assert(reader_a.checkDispatchOrder());

	MyReader reader_b{MyReader::LastTypeMode::ReturnEndOfFile};
	assert(DataStore::read(reader_b) == StatusCode::Success);
	assert(DataStore::read(reader_b) == StatusCode::Success);
	assert(DataStore::read(reader_b) == StatusCode::Success);
	assert(DataStore::read(reader_b) == StatusCode::EndOfFile);
	assert(reader_b.checkDispatchOrder());

	return 0;
	}

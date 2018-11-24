//@	{"targets":[{"name":"value_decoder.test", "type":"application", "autorun":1}]}

#include "value_decoder.hpp"

#include <string>
#include <vector>
#include <cassert>

enum class StatusCode:int{Success, UnknownType, EndOfFile};

bool readFailed(StatusCode status)
	{return status!=StatusCode::Success;}

class MyDecoderPolicy
	{
	public:
		using SupportedTypes = DataStore::TypeSet<int, std::string, double>;

		bool checkDispatchOrder() const
			{
			std::vector<std::string> dispatch_order{"int", "string", "double"};
			return m_dispatch_order == dispatch_order;
			}

		enum class LastTypeMode{ReturnUnknownType, ReturnEndOfFile};

		explicit MyDecoderPolicy(LastTypeMode mode) : m_type(0), m_mode(mode){}

		auto getNextType()
			{
			auto ret = m_type;
			++m_type;
			if(m_mode == LastTypeMode::ReturnUnknownType)
				{return std::make_pair(ret, StatusCode::Success);}

			else
				{return std::make_pair(ret, ret<3 ? StatusCode::Success : StatusCode::EndOfFile);}
			}

		MyDecoderPolicy(MyDecoderPolicy const&) = delete;

		auto decode(DataStore::Empty<std::string>)
			{
			m_dispatch_order.push_back("string");
			return StatusCode::Success;
			}

		auto decode(DataStore::Empty<int>)
			{
			m_dispatch_order.push_back("int");
			return  StatusCode::Success;
			}

		auto decode(DataStore::Empty<double>)
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
	assert(DataStore::decode(MyDecoderPolicy{MyDecoderPolicy::LastTypeMode::ReturnUnknownType}) == StatusCode::Success);

	MyDecoderPolicy decoder_a{MyDecoderPolicy::LastTypeMode::ReturnUnknownType};
	assert(DataStore::decode(decoder_a) == StatusCode::Success);
	assert(DataStore::decode(decoder_a) == StatusCode::Success);
	assert(DataStore::decode(decoder_a) == StatusCode::Success);
	assert(DataStore::decode(decoder_a) == StatusCode::UnknownType);
	assert(decoder_a.checkDispatchOrder());

	MyDecoderPolicy decoder_b{MyDecoderPolicy::LastTypeMode::ReturnEndOfFile};
	assert(DataStore::decode(decoder_b) == StatusCode::Success);
	assert(DataStore::decode(decoder_b) == StatusCode::Success);
	assert(DataStore::decode(decoder_b) == StatusCode::Success);
	assert(DataStore::decode(decoder_b) == StatusCode::EndOfFile);
	assert(decoder_b.checkDispatchOrder());

	return 0;
	}

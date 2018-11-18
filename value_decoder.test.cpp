//@	{"targets":[{"name":"value_decoder.test", "type":"application", "autorun":1}]}

#include "value_decoder.hpp"

#include <string>
#include <cassert>

class MyDecoderPolicy
	{
	public:
		enum class StatusCode:int{UnknownType, String, Int, Double};

		using SupportedTypes = DataStore::TypeSet<int, std::string, double>;

		enum class TypeId:intptr_t{String, Int, Double};

		static constexpr TypeId getTypeId(DataStore::Empty<std::string>)
			{return TypeId::String;}

		static constexpr TypeId getTypeId(DataStore::Empty<int>)
			{return TypeId::Int;}

		static constexpr TypeId getTypeId(DataStore::Empty<double>)
			{return TypeId::Double;}

		static constexpr auto hash(TypeId type_id)
			{return static_cast<intptr_t>(type_id);}


		MyDecoderPolicy(MyDecoderPolicy const&) = delete;

		auto decode(DataStore::Empty<std::string>)
			{return StatusCode::String;}

		auto decode(DataStore::Empty<int>)
			{return  StatusCode::Int;}

		auto unknownType()
			{return StatusCode::UnknownType;}

		auto decode(DataStore::Empty<double>)
			{return StatusCode::Double;}
	};

int main()
	{
	assert(DataStore::decode(MyDecoderPolicy{}, MyDecoderPolicy::TypeId::String) == MyDecoderPolicy::StatusCode::String);
	assert(DataStore::decode(MyDecoderPolicy{}, MyDecoderPolicy::TypeId::Int) == MyDecoderPolicy::StatusCode::Int);
	assert(DataStore::decode(MyDecoderPolicy{}, MyDecoderPolicy::TypeId::Double) == MyDecoderPolicy::StatusCode::Double);
	assert(DataStore::decode(MyDecoderPolicy{}, static_cast<MyDecoderPolicy::TypeId>(35890730)) == MyDecoderPolicy::StatusCode::UnknownType);
	return 0;
	}

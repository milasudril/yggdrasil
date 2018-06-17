//@	{"targets":[{"name":"message_formatter_test","type":"application","autorun":1}]}

#include "message_formatter.hpp"
#include "stic/stic.hpp"

class MyFormatter
	{
	public:
		std::string keyNotFound(std::string const& key, DataStore::SourceLocation const& src_loc) const
			{
			return src_loc.filename() + ":" + std::to_string(src_loc.line()) + ":" + std::to_string(src_loc.col())
				+ ": \"" + key + "\" does not exist";
			}
			
		std::string typeMismatch(std::string const& key, std::string const& type_id, DataStore::SourceLocation const& src_loc) const
			{
			return src_loc.filename() + ":" + std::to_string(src_loc.line()) + ":" + std::to_string(src_loc.col())
				+ ": \"" + key + "\" should be of type " + type_id;
			}
			
	};
	
STIC_TESTCASE("Default message")
	{
	STIC_ASSERT(DataStore::keyNotFound("Key", DataStore::SourceLocation{})
		== "Key not found");
	STIC_ASSERT(DataStore::typeMismatch("Key", "x", DataStore::SourceLocation{})
		== "Type mismatch");
	}

STIC_TESTCASE("Custom message")
	{
	DataStore::messageFormatter(MyFormatter{});
	DataStore::SourceLocation src_loc{"test.ysil",1,2};
	
	STIC_ASSERT(DataStore::keyNotFound("Key", src_loc) == "test.ysil:1:2: \"Key\" does not exist");
	STIC_ASSERT(DataStore::typeMismatch("Key", "i20", src_loc)
		== "test.ysil:1:2: \"Key\" should be of type i20");
	}

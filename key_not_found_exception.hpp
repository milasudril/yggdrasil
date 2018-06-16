//@	{"targets":[{"name":"key_not_found_exception.hpp","type":"include"}]}

#ifndef DATA_STORE_KEY_NOT_FOUND_EXCEPTION_HPP
#define DATA_STORE_KEY_NOT_FOUND_EXCEPTION_HPP

#include "message_formatter.hpp"

#include <type_traits>
#include <stdexcept>

namespace DataStore
	{
	class KeyNotFoundException : public std::exception
		{
		public:
			explicit KeyNotFoundException(std::string const& key, SourceLocation const& src_loc)
				: m_key(key), m_src_loc(src_loc)
				{}
				
			char const* what() const noexcept override
				{
				try
					{
					m_message = keyNotFound(m_key, m_src_loc);
					return m_message.data();
					}
				catch(...)
					{return "Key not found";}
				}
			
		private:
			std::string m_key;
			SourceLocation m_src_loc;
			mutable std::string m_message;
		};
	}
#endif

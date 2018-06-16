//@	{"targets":[{"name":"type_mismatch_exception.hpp","type":"include"}]}

#ifndef DATA_STORE_TYPE_MISMATCH_EXCEPTION_HPP
#define DATA_STORE_TYPE_MISMATCH_EXCEPTION_HPP

#include "message_formatter.hpp"

#include <type_traits>
#include <stdexcept>

namespace DataStore
	{
	class TypeMismatchException : public std::exception
		{
		public:
			explicit TypeMismatchException(std::string const& key, std::string const& type_id
				, SourceLocation const& src_loc)
				: m_key(key), m_type_id(type_id), m_src_loc(src_loc)
				{}
				
			const char* what() const noexcept override
				{
				try
					{
					m_message = typeMismatch(m_key, m_type_id, m_src_loc);
					return m_message.data();
					}
				catch(...)
					{return "Key not found";}
				}
			
		private:
			std::string m_key;
			std::string m_type_id;
			SourceLocation m_src_loc;
			mutable std::string m_message;
			
		};
	}
#endif

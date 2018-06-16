//@	{"targets":[{"name":"source_location.hpp","type":"include"}]}

#ifndef DATA_STORE_SOURCE_LOCATION_HPP
#define DATA_STORE_SOURCE_LOCATION_HPP

#include <string>

namespace DataStore
	{
	class SourceLocation
		{
		public:
			SourceLocation() = default;
			
			explicit SourceLocation(std::string const& filename, int line, int col)
				: m_filename(filename), m_line(line), m_col(col)
				{}
				
			std::string const& filename() const noexcept
				{return m_filename;}
				
			
			int line() const noexcept
				{return m_line;}
				
				
			int col() const noexcept
				{return m_col;}
	
		private:
			std::string m_filename;
			int m_line;
			int m_col;
		};	
	};

#endif

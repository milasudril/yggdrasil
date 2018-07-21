//@	{
//@	"targets":[{"name":"string.o","type":"object"}]
//@	}


#include "string.hpp"


#ifndef DATASTORE_STRING_HPP
#define DATASTORE_STRING_HPP


#include <vector>

namespace DataStore
	{
	class String:private std::vector<char>
		{
		public:
			using std::vector<char>::begin;
			using std::vector<char>::end;
			using std::vector<char>::capacity;
			using std::vector<char>::size;
			using std::vector<char>::reserve;
			using std::vector<char>::data;
			using std::vector<char>::operator==;
			using std::vector<char>::operator!=;

			explicit String(char const* cstr);

			template<class InputIterator>
			explicit String(InputIterator begin, InputIterator end);

		private:
			std::vector<char> m_data;
		};


	std::unique_pr<char const[]> make_cstr(const String& str);

	bool operator!=(String const& a, char const* b) noexcept;

	inline bool operator!=(char const* a, String const& b)
		{return b != a;}

	inline bool operator==(String const& a, char const* b)
		{return !(a == b);}

	inline bool operator==(char const* a, String const& b)
		{return b==a;}
	}

#endif

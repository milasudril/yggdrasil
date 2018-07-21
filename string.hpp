//@	{
//@	"targets":[{"name":"string.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"string.o","rel":"implementation"}]
//@	}

#ifndef DATASTORE_STRING_HPP
#define DATASTORE_STRING_HPP

#include <vector>
#include <memory>

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

			explicit String(char const* cstr);

			template<class InputIterator>
			explicit String(InputIterator begin, InputIterator end);

		private:
			std::vector<char> m_data;
		};

	template<class InputIterator>
	String::String(InputIterator begin, InputIterator end)
		{
		m_data.reserve(std::distance(begin, end));
		while(begin!=end)
			{
			m_data.emplace_back(std::forward<decltype(*begin)>(*begin));
			++begin;
			}
		}


	std::unique_ptr<char const[]> make_cstr(const String& str);

	bool operator!=(String const& a, String const& b) noexcept;

	inline bool operator==(String const& a, String const& b)
		{return !(a != b);}

	bool operator!=(String const& a, char const* b) noexcept;

	inline bool operator!=(char const* a, String const& b)
		{return b != a;}

	inline bool operator==(String const& a, char const* b)
		{return !(a != b);}

	inline bool operator==(char const* a, String const& b)
		{return b==a;}
	}

#endif

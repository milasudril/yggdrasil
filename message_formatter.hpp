//@	{"targets":[{"name":"message_formatter.hpp","type":"include"}]}

#ifndef DATA_STORE_MESSAGE_FORMATTER_HPP
#define DATA_STORE_MESSAGE_FORMATTER_HPP

#include "source_location.hpp"

#include <cstdint>
#include <memory>

namespace DataStore
	{
	class MessageFormatterBase
		{
		public:
			virtual std::string keyNotFound(std::string const& key, SourceLocation const& src_loc) const = 0;
			virtual std::string typeMismatch(std::string const& key, std::string const& type_id
				, SourceLocation const& src_loc) const = 0;
			virtual ~MessageFormatterBase() = default;
		};

	template<class Formatter>
	class MessageFormatter final: public MessageFormatterBase
		{
		public:
			explicit MessageFormatter(Formatter&& fmt) : m_formatter(std::forward<Formatter>(fmt))
				{}

			std::string keyNotFound(std::string const& key, SourceLocation const& src_loc) const override
				{return m_formatter.keyNotFound(key, src_loc);}
			
			std::string typeMismatch(std::string const& key, std::string const& type_id
				, SourceLocation const& src_loc) const override
				{return m_formatter.typeMismatch(key, type_id, src_loc);}

		private:
			Formatter m_formatter;
		};

	template<class From, class To>
	inline std::string formatMessage(From value, To min, To max);

	template<class Formatter>
	inline void messageFormatter(Formatter&& formatter) noexcept;

	namespace detail
		{
		template<class Dummy=int>
		struct FormatCaller
			{
			public:
				static std::string keyNotFound(std::string const& key, SourceLocation const& src_loc)
					{return s_formatter? s_formatter->keyNotFound(key, src_loc) : "Key not found";}
					
				static std::string typeMismatch(std::string const& key, std::string const& type_id
					, SourceLocation const& src_loc)
					{return s_formatter? s_formatter->typeMismatch(key, type_id, src_loc) : "Type mismatch";}

				template<class Formatter>
				static void formatter(Formatter&& fmt) noexcept
					{s_formatter = std::make_unique<MessageFormatter<Formatter> const>(std::forward<Formatter>(fmt));}

			private:
				static thread_local std::unique_ptr<MessageFormatterBase const> s_formatter;
			};

		template<class Dummy>
		thread_local std::unique_ptr<MessageFormatterBase const> FormatCaller<Dummy>::s_formatter = nullptr;
		}

	inline std::string keyNotFound(std::string const& key, SourceLocation const& src_loc)
		{return detail::FormatCaller<>::keyNotFound(key, src_loc);}
	
	inline std::string typeMismatch(std::string const& key, std::string const& type_id, SourceLocation const& src_loc)
		{return detail::FormatCaller<>::typeMismatch(key, type_id, src_loc);}

	template<class Formatter>
	inline void messageFormatter(Formatter&& formatter) noexcept
		{detail::FormatCaller<>::formatter(std::forward<Formatter>(formatter));}
	}
#endif

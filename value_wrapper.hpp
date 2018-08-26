//@	{"targets":[{"name":"value_wrapper.hpp","type":"include"}]}

#ifndef DATA_STORE_VALUE_WRAPPER_HPP
#define DATA_STORE_VALUE_WRAPPER_HPP

#include <memory>

namespace DataStore
	{
	template<class T>
	class ValueWrapper
		{
		public:
			template<class... Args>
			ValueWrapper(Args&&... args) : m_object(std::forward<Args>(args)...)
				{}

			ValueWrapper(ValueWrapper const&) = default;
			ValueWrapper& operator=(ValueWrapper const&) = default;
			ValueWrapper& operator=(ValueWrapper&&) = default;
			ValueWrapper(ValueWrapper&&) = default;

			T& get() noexcept
				{return m_object;}

			T const& get() const noexcept
				{return m_object;}

		private:
			T m_object;
		};

	template<class T>
	class ValueWrapper<std::unique_ptr<T>>
		{
		public:
			template<class... Args>
			ValueWrapper(Args&&... args) : m_object(std::make_unique<T>(std::forward<Args>(args)...))
				{}

			ValueWrapper(ValueWrapper const&) = default;
			ValueWrapper& operator=(ValueWrapper const&) = default;
			ValueWrapper& operator=(ValueWrapper&&) = default;
			ValueWrapper(ValueWrapper&&) = default;

			T& get() noexcept
				{return *m_object;}

			T const& get() const noexcept
				{return *m_object;}

		private:
			std::unique_ptr<T> m_object;
		};
	};

#endif

//@	{"targets":[{"name":"type_names.hpp","type":"include"}]}

#ifndef DATA_STORE_TYPE_NAMES_HPP
#define DATA_STORE_TYPE_NAMES_HPP

namespace DataStore
	{
	template<class T>
	constexpr decltype(T::typeName()) getTypeName() noexcept
		{return T::typeName();}

	template<class T>
	constexpr std::enable_if_t<std::is_same_v<T, String>, char const*> getTypeName() noexcept
		{return "str";}

	template<class T>
	constexpr std::enable_if_t<std::is_same_v<T, std::unique_ptr<String>>, char const*> getTypeName() noexcept
		{return getTypeName<String>();}

	template<class T>
	constexpr std::enable_if_t<std::is_same_v<T, Float>, char const*> getTypeName() noexcept
		{return "f20";}

	template<class T>
	constexpr std::enable_if_t<std::is_same_v<T, Double>, char const*> getTypeName() noexcept
		{return "f40";}

	namespace detail
		{
		// Helper function for concatenating two string sources into a std::array.
		template<class SrcA, class SrcB>
		constexpr auto strcat(SrcA const& a, SrcB const& b) noexcept
			{
			constexpr auto ptr_a = a();
			constexpr auto ptr_b = b();
			constexpr auto N_a = strlen(ptr_a);
			constexpr auto N_b = strlen(ptr_b);

			std::array<char, N_a + N_b + 1> ret{};
			auto ptr_src = ptr_a;
			auto ptr_dest = ret.begin();
			while(*ptr_src)
				{
				*ptr_dest = *ptr_src;
				++ptr_dest;
				++ptr_src;
				}

			ptr_src = ptr_b;
			while(*ptr_src)
				{
				*ptr_dest = *ptr_src;
				++ptr_dest;
				++ptr_src;
				}

			return ret;
			}
		}

	template<class T>
	struct ArrayTypeName
		{
		public:
			static constexpr char const* typeName() noexcept
				{return retval;}

		private:
			static constexpr auto val = detail::strcat([](){return getTypeName<T>();}
				, [](){return "a";});
			static constexpr auto retval = val.begin();
		};

	template<class T>
	constexpr decltype(getTypeName<typename T::value_type>()) getTypeName() noexcept
		{
		using Type = typename T::value_type;
		return ArrayTypeName<Type>::typeName();
		}
	}
#endif

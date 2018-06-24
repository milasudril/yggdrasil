//@	{"targets":[{"name":"enum_types.hpp", "type":"include"}]}

#ifndef ENUM_TYPES_HPP
#define ENUM_TYPES_HPP

#include <variant>

namespace DataStore
	{
	namespace detail
		{
		template<class Visitor, class VariantType, int N = std::variant_size_v<VariantType>>
		struct VarTypeEnumerator
			{
			static void iterate(Visitor&& f)
				{
				using CurrentType = decltype(*std::get_if<N - 1>(static_cast<VariantType*>(nullptr)));
				f.template visit<std::remove_reference_t<CurrentType>, N - 1>();
				VarTypeEnumerator<Visitor, VariantType, N - 1>::iterate(std::forward<Visitor>(f));
				}
			};

		template<class Visitor, class VariantType>
		struct VarTypeEnumerator<Visitor, VariantType, 0>
			{
			static void iterate(Visitor&& f) { }
			};
		}

	template<class VariantType, class Visitor>
	void enumTypes(Visitor&& visitor)
		{
		detail::VarTypeEnumerator<Visitor, VariantType>::iterate(std::forward<Visitor>(visitor));
		}
	}
#endif

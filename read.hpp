//@	{"targets":[{"name":"value_readr.hpp","type":"include"}]}

#ifndef DATAS_STORE_READ_HPP
#define DATAS_STORE_READ_HPP

#include "utility.hpp"

#include <type_traits>
#include <cstring>
#include <array>
#include <optional>
#include <cstddef>


namespace DataStore
	{
	namespace detail
		{
		template<class Reader, class Type>
		auto read(Reader& policy)
			{return policy.read(Empty<Type>{});}

		template<class Reader>
		auto unknown_type(Reader& policy)
			{return policy.unknownType();}

		template<class Reader>
		using ReadFunction = std::decay_t<decltype(unknown_type<Reader>)>;

		template<class Reader, size_t N>
		using VtableEntries = std::array<ReadFunction<Reader>, N>;

		template<class Reader, class TypeSet, size_t N=GetSize<TypeSet>::value>
		struct BuildVtable
			{
			using CurrentType = typename GetType<N - 1, TypeSet>::type;
			static constexpr auto vt_size = GetSize<TypeSet>::value;
			static constexpr void setCallback(VtableEntries<Reader, vt_size>& vt)
				{
				vt[N - 1] = read<Reader, CurrentType>;
				return BuildVtable<Reader, TypeSet, N - 1>::setCallback(vt);
				}
			};

		template<class Reader, class TypeSet>
		struct BuildVtable<Reader, TypeSet, 0>
			{
			static constexpr auto vt_size = GetSize<TypeSet>::value;
			static constexpr void setCallback(VtableEntries<Reader, vt_size>&)
				{ }
			};

		template<class Reader, class TypeSet>
		static constexpr auto buildVtable()
			{
			VtableEntries<Reader, GetSize<TypeSet>::value> vt{};
			BuildVtable<Reader, TypeSet>::setCallback(vt);
			return vt;
			}

		template<class Reader, class TypeSet>
		static constexpr auto vtable = buildVtable<Reader, TypeSet>();
		}

    template<class Reader>
    auto read(Reader&& policy)
        {
        auto [type_index, status] = policy.readType();
        if(unlikely(readFailed(status)))
			{return status;}

        using Policy = std::decay_t<Reader>;
        using TypeSet = typename Policy::SupportedTypes;
        if(unlikely(type_index>=GetSize<TypeSet>::value))
            {return policy.unknownType();}
        return detail::vtable<Policy, TypeSet>[type_index](policy);
        }
	}

#endif

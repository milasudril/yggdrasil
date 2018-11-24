//@	{"targets":[{"name":"value_decoder.hpp","type":"include"}]}

#ifndef DATAS_TORE_VALUE_DECODER_HPP
#define DATAS_TORE_VALUE_DECODER_HPP

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
		template<class DecoderPolicy, class Type>
		auto decode(DecoderPolicy& policy)
			{return policy.decode(Empty<Type>{});}

		template<class DecoderPolicy>
		auto unknown_type(DecoderPolicy& policy)
			{return policy.unknownType();}

		template<class DecoderPolicy>
		using Decoder = std::decay_t<decltype(unknown_type<DecoderPolicy>)>;

		template<class DecoderPolicy, size_t N>
		using VtableEntries = std::array<Decoder<DecoderPolicy>, N>;

		template<class DecoderPolicy, class TypeSet, size_t N=GetSize<TypeSet>::value>
		struct BuildVtable
			{
			using CurrentType = typename GetType<N - 1, TypeSet>::type;
			static constexpr auto vt_size = GetSize<TypeSet>::value;
			static constexpr void setCallback(VtableEntries<DecoderPolicy, vt_size>& vt)
				{
				vt[N - 1] = decode<DecoderPolicy, CurrentType>;
				return BuildVtable<DecoderPolicy, TypeSet, N - 1>::setCallback(vt);
				}
			};

		template<class DecoderPolicy, class TypeSet>
		struct BuildVtable<DecoderPolicy, TypeSet, 0>
			{
			static constexpr auto vt_size = GetSize<TypeSet>::value;
			static constexpr void setCallback(VtableEntries<DecoderPolicy, vt_size>&)
				{ }
			};

		template<class DecoderPolicy, class TypeSet>
		static constexpr auto buildVtable()
			{
			VtableEntries<DecoderPolicy, GetSize<TypeSet>::value> vt{};
			BuildVtable<DecoderPolicy, TypeSet>::setCallback(vt);
			return vt;
			}

		template<class DecoderPolicy, class TypeSet>
		static constexpr auto vtable = buildVtable<DecoderPolicy, TypeSet>();
		}

    template<class DecoderPolicy>
    auto decode(DecoderPolicy&& policy)
        {
        auto [type_index, status] = policy.getNextType();
        if(unlikely(readFailed(status)))
			{return status;}

        using Policy = std::decay_t<DecoderPolicy>;
        using TypeSet = typename Policy::SupportedTypes;
        if(unlikely(type_index>=GetSize<TypeSet>::value))
            {return policy.unknownType();}
        return detail::vtable<Policy, TypeSet>[type_index](policy);
        }
	}

#endif

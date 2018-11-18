//@	{"targets":[{"name":"value_decoder.hpp","type":"include"}]}

#ifndef DATASTORE_VALUE_DECODER_HPP
#define DATASTORE_VALUE_DECODER_HPP

#include <type_traits>
#include <cstring>
#include <array>
#include <optional>
#include <cstddef>

#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)


namespace DataStore
	{
	template<class T>
	struct Empty{};

    template<class ... Types>
    struct TypeSet
        {
        static constexpr size_t size()
            {return sizeof...(Types);}
        };

	namespace detail
		{
		template<class TypeId, class DecodeFunction>
		struct HashTableEntry
			{
			TypeId type_id;
			DecodeFunction decoder;
			};

		template<class TypeId, class DecodeFunction>
		struct HashTableKeyEntry
			{
			intptr_t key;
			HashTableEntry<TypeId, DecodeFunction> entry;
			};

        template<class TypeId, class DecodeFunction>
        constexpr bool key_in_use(HashTableKeyEntry<TypeId, DecodeFunction>* begin
			, HashTableKeyEntry<TypeId, DecodeFunction>* next_slot
			, uint32_t suggested_key)
            {
            while(begin!=next_slot)
                {
                if(suggested_key==begin->key && begin->entry.decoder!=nullptr)
                    {return true;}
                ++begin;
                }
            return false;
            }

        template<class DecoderPolicy, class Type>
        auto decode(DecoderPolicy& policy)
            {return policy.decode(Empty<Type>{});}

        template<class DecoderPolicy, class Type>
        auto unknown_type(DecoderPolicy& policy)
            {return policy.unknownType();}

        template<class DecoderPolicy, class Type>
        using Decoder = std::decay_t<decltype(decode<DecoderPolicy, Type>)>;

        template<class DecoderPolicy, class CurrentType>
        constexpr std::optional<HashTableKeyEntry<typename DecoderPolicy::TypeId, Decoder<DecoderPolicy, CurrentType>>>
        createHashTableEntry(HashTableKeyEntry<typename DecoderPolicy::TypeId, Decoder<DecoderPolicy,CurrentType>>* begin
            , HashTableKeyEntry<typename DecoderPolicy::TypeId, Decoder<DecoderPolicy,CurrentType>>* next_slot
            , uint32_t mod_factor)
            {
            using TypeId = typename DecoderPolicy::TypeId;
      		auto type_id = DecoderPolicy::getTypeId(Empty<CurrentType>{});
			const auto suggested_key = DecoderPolicy::hash(type_id) % mod_factor;
			if(key_in_use(begin, next_slot, suggested_key))
        		{return std::optional<HashTableKeyEntry<TypeId, Decoder<DecoderPolicy, CurrentType>>>{};}
            auto func = decode<DecoderPolicy, CurrentType>;
            return HashTableKeyEntry<TypeId, Decoder<DecoderPolicy, CurrentType>>{suggested_key, HashTableEntry<TypeId, Decoder<DecoderPolicy,CurrentType>>{type_id,func} };
            }

        template<size_t N, class T>
        struct GetType;

        template<size_t N, class Head, class ... Tail>
        struct GetType<N, TypeSet<Head, Tail...>>:GetType<N-1, TypeSet<Tail...>>
            {};

        template<class Head, class ... Tail>
        struct GetType<0, TypeSet<Head, Tail...>>
            { using type = Head; };

        template<class ... T>
        struct GetSize;

        template<class ... T>
        struct GetSize<TypeSet<T...>>
            {static constexpr const size_t value = sizeof...(T);};

		template<class DecoderPolicy, class TypeSet, size_t N = GetSize<TypeSet>::value>
		struct HashTableBuilder
			{
            using CurrentType = typename GetType<N - 1, TypeSet>::type;
			static constexpr bool appendEntry(
                  HashTableKeyEntry<typename DecoderPolicy::TypeId, Decoder<DecoderPolicy,CurrentType>>* begin
                , HashTableKeyEntry<typename DecoderPolicy::TypeId, Decoder<DecoderPolicy,CurrentType>>* next_slot
                , uint32_t mod_factor)
				{
                auto res = createHashTableEntry<DecoderPolicy, CurrentType>(begin, next_slot, mod_factor);
				if(res)
                    {
                    *next_slot = res.value();
				    return HashTableBuilder<DecoderPolicy, TypeSet, N - 1>::appendEntry(begin, next_slot + 1, mod_factor);
                    }
                return false;
				}
			};

		template<class DecoderPolicy, class TypeSet>
		struct HashTableBuilder<DecoderPolicy, TypeSet, 0>
			{
            using CurrentType = typename GetType<0, TypeSet>::type;
			static constexpr bool appendEntry(
                  HashTableKeyEntry<typename DecoderPolicy::TypeId, Decoder<DecoderPolicy,CurrentType>>* begin
                , HashTableKeyEntry<typename DecoderPolicy::TypeId, Decoder<DecoderPolicy,CurrentType>>* next_slot
                , uint32_t mod_factor)
				{return true;}
            };

        template<class DecoderPolicy, class TypeSet>
		struct HashTable
            {
            private:
                using DecodeFunction = typename DecoderPolicy::StatusCode (*)(DecoderPolicy&);

                using KeyEntryArray = std::array<HashTableKeyEntry<typename DecoderPolicy::TypeId, DecodeFunction>, GetSize<TypeSet>::value>;

                static constexpr auto createEntries()
                    {
                    uint8_t mod_factor = 0;  // Larger table is silly...
                    KeyEntryArray ret{};
                    do
                        {
                        ++mod_factor;
                        ret = KeyEntryArray{};
                        HashTableBuilder<DecoderPolicy, TypeSet>::appendEntry(ret.begin(), ret.begin(), mod_factor);
                        }
                    while(!HashTableBuilder<DecoderPolicy, TypeSet>::appendEntry(ret.begin(), ret.begin(), mod_factor) && mod_factor!=0);
                    return std::pair<decltype(ret), decltype(mod_factor)>{ret, mod_factor};
                    }

                template<uint16_t ModFactor>
                static constexpr auto generateContents(KeyEntryArray const& key_entries)
                    {
                    std::array<HashTableEntry<typename DecoderPolicy::TypeId, DecodeFunction>, ModFactor> ret{};
                    for(size_t k=0; k<key_entries.size(); ++k)
                        {ret[key_entries[k].key] = key_entries[k].entry;}
                    return ret;
                    }

                static constexpr auto key_entries = createEntries();
                static_assert(key_entries.second!=0, "Failed to assign unique keys for the given type ids.");

            public:
                static constexpr auto content = generateContents<key_entries.second>(key_entries.first);
                static constexpr auto mod_factor = key_entries.second;
            };
		}

    template<class DecoderPolicy>
    auto decode(DecoderPolicy&& policy, typename std::decay_t<DecoderPolicy>::TypeId const& type_id)
        {
        using Policy = std::decay_t<DecoderPolicy>;
        using HashTable = detail::HashTable<Policy, typename Policy::SupportedTypes>;
        auto slot = policy.hash(type_id) % HashTable::mod_factor;
        auto const& entry = HashTable::content[slot];
        if(unlikely(entry.type_id != type_id))
            {return policy.unknownType();}
        return entry.decoder(policy);
        }
	}

#endif

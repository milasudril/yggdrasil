#include <type_traits>
#include <cstring>
#include <array>
#include <optional>

#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)

//TODO: Requires copy of DecoderPolicy (maybe minor if it does not own the fd)
//TODO: Customized type id type
//TODO: Customized hash function (must be constexpr)

namespace DataStore
	{
	template<class T>
	struct Empty{};

	namespace detail
		{
		template<class DecodeFunction>
		struct HashTableEntry
			{
			const char* type_id;
			DecodeFunction decoder;
			};


		constexpr uint32_t fnv32(const char* str)
			{
			const auto FNV_offset_basis{0x811c9dc5};
			uint32_t hash{FNV_offset_basis};
			while(*str)
				{
				auto val = static_cast<uint32_t>(*str);
				hash^=val;
				hash*=16777619;
				++str;
				}

			return hash;
			}

		template<class DecodeFunction>
		struct HashTableKeyEntry
			{
			uint32_t key;
			HashTableEntry<DecodeFunction> entry;
			};

        template<class DecodeFunction>
        constexpr bool keyInUse(HashTableKeyEntry<DecodeFunction>* begin, HashTableKeyEntry<DecodeFunction>* next_slot, uint32_t suggested_key)
            {
            while(begin!=next_slot)
                {
                if(suggested_key==begin->key && begin->entry.type_id!=nullptr)
                    {return true;}
                ++begin;
                }
            return false;
            }

        template<class DecoderPolicy, class Type>
        auto decode(DecoderPolicy policy)
            {return policy.decode(Empty<Type>{});}

        template<class DecoderPolicy, class Type>
        using Decoder = std::decay_t<decltype(decode<DecoderPolicy, Type>)>;

        template<class DecoderPolicy, class CurrentType>
        constexpr std::optional<HashTableKeyEntry<Decoder<DecoderPolicy, CurrentType>>> createHashTableEntry(
              HashTableKeyEntry<Decoder<DecoderPolicy,CurrentType>>* begin
            , HashTableKeyEntry<Decoder<DecoderPolicy,CurrentType>>* next_slot, uint32_t mod_factor)
            {
      		auto type_id = DecoderPolicy::getTypeId(Empty<CurrentType>{});
			const auto suggested_key = fnv32(type_id) % mod_factor;
			if(keyInUse(begin, next_slot, suggested_key))
        		{return std::optional<HashTableKeyEntry<Decoder<DecoderPolicy,CurrentType>>>{};}
            auto func = decode<DecoderPolicy, CurrentType>;
            return HashTableKeyEntry<Decoder<DecoderPolicy,CurrentType>>{suggested_key, HashTableEntry<Decoder<DecoderPolicy,CurrentType>>{type_id,func} };
            }

		template<class DecoderPolicy, class CurrentType, class ... T>
		struct HashTableBuilder
			{
			static constexpr bool appendEntry(
                  HashTableKeyEntry<Decoder<DecoderPolicy,CurrentType>>* begin
                , HashTableKeyEntry<Decoder<DecoderPolicy,CurrentType>>* next_slot
                , uint32_t mod_factor)
				{
                auto res = createHashTableEntry<DecoderPolicy, CurrentType>(begin, next_slot, mod_factor);
				if(res)
                    {
                    *begin = res.value();
				    return HashTableBuilder<DecoderPolicy, T...>::appendEntry(begin, next_slot + 1, mod_factor);
                    }
                return false;
				}

			private:

			};

		template<class DecoderPolicy, class CurrentType>
		struct HashTableBuilder<DecoderPolicy, CurrentType>
			{
			static constexpr bool appendEntry(
                 HashTableKeyEntry<Decoder<DecoderPolicy,CurrentType>>* begin
                , HashTableKeyEntry<Decoder<DecoderPolicy,CurrentType>>* next_slot, uint32_t mod_factor)
				{
                auto res = createHashTableEntry<DecoderPolicy, CurrentType>(begin, next_slot, mod_factor);
                if(res)
                    {
                    *next_slot = res.value();
                    return true;
                    }
                return false;
                }
            };

        template<class DecoderPolicy, class ... T>
		struct HashTable
            {
            private:
                using DecodeFunction = typename DecoderPolicy::StatusCode (*)(DecoderPolicy);

                using KeyEntryArray = std::array<HashTableKeyEntry<DecodeFunction>, sizeof...(T)>;

                static constexpr auto createEntries()
                    {
                    uint16_t mod_factor = 0;  // Larger table is silly...
                    KeyEntryArray ret{};
                    do
                        {
                        ++mod_factor;
                        ret = KeyEntryArray{};
                        }
                    while(!HashTableBuilder<DecoderPolicy, T...>::appendEntry(ret.begin(), ret.begin(), mod_factor) && mod_factor!=0);
                    return std::pair<decltype(ret), decltype(mod_factor)>{ret, mod_factor};
                    }

                template<uint16_t ModFactor>
                static constexpr auto generateContents(KeyEntryArray const& key_entries)
                    {
                    std::array<HashTableEntry<DecodeFunction>, ModFactor> ret{};
                    for(size_t k=0; k<key_entries.size(); ++k)
                        {ret[key_entries[k].key] = key_entries[k].entry;}
                    return ret;
                    }

                static constexpr auto key_entries = createEntries();
                static_assert(key_entries.second!=0, "Failed to create unique keys for the given type ids.");

            public:
                static constexpr auto content = generateContents<key_entries.second>(key_entries.first);
                static constexpr auto mod_factor = key_entries.second;
            };
		}

    template<class DecoderPolicy>
    auto decode(DecoderPolicy policy,const char* type_id)
        {
        using HashTable = detail::HashTable<DecoderPolicy, int, std::string>;
        auto slot = detail::fnv32(type_id) % HashTable::mod_factor;
        auto const& entry = HashTable::content[slot];
        if(entry.type_id == nullptr
            || strcmp(entry.type_id, type_id)!=0)
            {return policy.unknownType();}
        return entry.decoder(policy);
        }
	}

// Beign client code


#include <string>

class MyDecoderPolicy
    {
    public:
        enum class StatusCode:int{Success, BadFormat, OutOfRange, UnknownType};

        static constexpr const char* getTypeId(DataStore::Empty<std::string>)
            {return "str";}

        static constexpr const char* getTypeId(DataStore::Empty<int>)
            {return "int";}

        auto decode(DataStore::Empty<std::string>)
            {
            puts("Decoding string");
            return StatusCode::Success;
            }

        auto decode(DataStore::Empty<int>)
            {
            puts("Decoding int");
            return  StatusCode::Success;
            }

        auto unknownType()
            {
            puts("Unkonw type");
            return StatusCode::UnknownType;
            }
    };

auto myDecode(MyDecoderPolicy& policy, const char* type_id)
    {
    return DataStore::decode(policy, type_id);
    }

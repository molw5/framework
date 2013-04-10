// Copyright (C) 2013 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

namespace framework
{
    namespace protocol_buffers
    {
// Doxygen appears to fail parsing the following, suppressing the warnings (no documentation)
#ifndef DOXYGEN
        template <unsigned long ID, unsigned char WireType>
        struct fixed_tag <ID, WireType,
            typename std::enable_if <
                !(ID >> 4),
                void
            >::type>
        {
            enum{ size = 1 };
            enum : uint64_t
            {
                buffer =
                    ((((uint64_t(ID) & 0xF) << 3) | WireType) & 0x7F)
            };
        };

        template <uint64_t ID, unsigned char WireType>
        struct fixed_tag <ID, WireType,
            typename std::enable_if <
                ((ID >> 4) && (!(ID >> 11))),
                void
            >::type>
        {
            enum{ size = 2 };
            enum : uint64_t
            {
                buffer =
                    ((((uint64_t(ID) & 0xF) << 3) | WireType) | 0x80) |
                    (((uint64_t(ID) >> 4) & 0x7F) << 8)
            };
        };

        template <uint64_t ID, unsigned char WireType>
        struct fixed_tag <ID, WireType,
            typename std::enable_if <
                (ID >> 11) &&
                !(ID >> 18),
                void
            >::type>
        {
            enum{ size = 3 };
            enum : uint64_t
            {
                buffer =
                    ((((uint64_t(ID) & 0xF) << 3) | WireType) | 0x80) |
                    (((uint64_t(ID) >> 4) | 0x80) << 8) |
                    (((uint64_t(ID) >> 11) & 0x7F) << 16)
            };
        };

        template <uint64_t ID, unsigned char WireType>
        struct fixed_tag <ID, WireType,
            typename std::enable_if <
                (ID >> 18) &&
                !(ID >> 25),
                void
            >::type>
        {
            enum{ size = 4 };
            enum : uint64_t
            {
                buffer =
                    ((((uint64_t(ID) & 0xF) << 3) | WireType) | 0x80) |
                    (((uint64_t(ID) >> 4) | 0x80) << 8) |
                    (((uint64_t(ID) >> 11) | 0x80) << 16) |
                    (((uint64_t(ID) >> 18) & 0x7F) << 24)
            };
        };
#endif

        template <unsigned long ID, unsigned char WireType, typename Enabler>
        struct fixed_tag
        {
            enum{ size = 5 };
            enum : uint64_t
            {
                buffer =
                    ((((uint64_t(ID) & 0xF) << 3) | WireType) | 0x80) |
                    (((uint64_t(ID) >> 4) | 0x80) << 8) |
                    (((uint64_t(ID) >> 11) | 0x80) << 16) |
                    (((uint64_t(ID) >> 18) | 0x80) << 24) |
                    (((uint64_t(ID) >> 25) & 0x7F) << 32)
            };
        };

        namespace detail
        {
            template <typename Tag, typename Indices>
            struct write_tag_impl;

            template <typename Tag, typename... Indices>
            struct write_tag_impl <Tag, pack_container <Indices...>>
            {
                template <typename Output>
                FRAMEWORK_ALWAYS_INLINE
                static bool run (Output& out)
                {
                    constexpr uint8_t raw[sizeof...(Indices)] = {((Tag::buffer >> (Indices::value * 8)) & 0xFF)...};
                    return serializable::stream_write <sizeof...(Indices)> (out, reinterpret_cast <char const*> (&raw[0]));
                }

                FRAMEWORK_ALWAYS_INLINE
                static bool run (char* begin, char* end, char*& it)
                {
                    (void)end; // suppress warnings
                    constexpr uint8_t raw[sizeof...(Indices)] = {((Tag::buffer >> (Indices::value * 8)) & 0xFF)...};
                    assert(static_cast <std::size_t> (end - begin) >= sizeof...(Indices));
                    memcpy(begin, &raw[0], sizeof...(Indices));
                    it = begin + sizeof...(Indices);
                    return true;
                }
            };

            template <typename Tag, typename Indices>
            struct expect_tag_impl;
            
            template <typename Tag, typename... Indices>
            struct expect_tag_impl <Tag, pack_container <Indices...>>
            {
                FRAMEWORK_ALWAYS_INLINE
                static bool run (char const* begin, char const* end, char const*& it)
                {
                    (void)end; // suppress warnings
                    constexpr uint8_t raw[sizeof...(Indices)] = {((Tag::buffer >> (Indices::value * 8)) & 0xFF)...};
                    assert(static_cast <std::size_t> (end - begin) >= sizeof...(Indices));
                    
                    if (FRAMEWORK_EXPECT_FALSE(memcmp(begin, &raw[0], sizeof...(Indices)) != 0))
                        return false;

                    it = begin + sizeof...(Indices);
                    return true;
                }
            };
        }

        template <typename Tag, typename Output>
        bool write_tag (Output& out)
        {
            return detail::write_tag_impl <Tag, make_indices <Tag::size>>::run(out);
        }

        template <typename Tag>
        FRAMEWORK_ALWAYS_INLINE
        bool write_tag (char* begin, char* end, char*& it)
        {
            return detail::write_tag_impl <Tag, make_indices <Tag::size>>::run(begin, end, it);
        }
        
        template <typename Tag>
        FRAMEWORK_ALWAYS_INLINE
        bool expect_tag (char const* begin, char const* end, char const*& it)
        {
            return detail::expect_tag_impl <Tag, make_indices <Tag::size>>::run(begin, end, it);
        }
    }
}

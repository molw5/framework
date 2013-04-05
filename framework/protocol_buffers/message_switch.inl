// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

namespace framework
{
    namespace protocol_buffers
    {
        namespace detail
        {
            template <typename Input, typename Output>
            bool read_unknown_group (uint32_t group, Input&& in, Output&& out)
            {
                using serializable::dispatch_read;

                (void)out; // suppress warnings

                // Arbitrary limit of 10 group levels
                std::array <uint64_t, 10> stack {{group}};
                auto it = stack.begin();

                while (true)
                {
                    uint64_t field;
                    if (!dispatch_read <uint64> (in, field))
                        return false;

                    uint32_t const id = field >> 3;
                    uint8_t const type = field & 0x7;
                    switch (static_cast <e_wire_type> (type))
                    {
                        case e_wire_type::varint:
                            {
                                uint64_t tmp;
                                if (!dispatch_read <uint64> (in, tmp))
                                    return false;

                                break;
                            }

                        case e_wire_type::length_delimited:
                            {
                                std::size_t size;
                                if (!dispatch_read <varint <std::size_t>> (in, size))
                                    return false;
                                if (!in.seekg(size, std::ios_base::cur))
                                    return false;

                                break;
                            }

                        case e_wire_type::start_group:
                            {
                                if (++it == stack.end())
                                    return false;

                                *it = id;
                                break;
                            }

                        case e_wire_type::end_group:
                            {
                                if (it == stack.begin())
                                    return *it == id;
                                if (*(it--) != id)
                                    return false;

                                break;
                            }
                    
                        case e_wire_type::fixed_64: 
                            {
                                if (!in.seekg(8, std::ios_base::cur))
                                    return false;

                                break;
                            }

                        case e_wire_type::fixed_32:
                            {
                                if (!in.seekg(4, std::ios_base::cur))
                                    return false;

                                break;
                            }
                            
                        default: return false;
                    }
                }
            }

            template <typename Output>
            bool read_unknown_group (
                uint32_t group,
                char const* begin, char const* end, char const*& last,
                Output&& out)
            {
                using serializable::dispatch_read;

                (void)out; // suppress warnings

                // Arbitrary limit of 10 group levels
                std::array <uint64_t, 10> stack {{group}};
                auto it = stack.begin();

                while (true)
                {
                    uint64_t field;
                    if (!dispatch_read <uint64> (begin, end, begin, field))
                        return false;

                    uint32_t const id = field >> 3;
                    uint8_t const type = field & 0x7;
                    switch (static_cast <e_wire_type> (type))
                    {
                        case e_wire_type::varint:
                            {
                                uint64_t tmp;
                                if (!dispatch_read <uint64> (begin, end, begin, tmp))
                                    return false;

                                break;
                            }

                        case e_wire_type::length_delimited:
                            {
                                std::size_t size;
                                if (!dispatch_read <varint <std::size_t>> (begin, end, begin, size))
                                    return false;
                                if (end - begin < static_cast <long> (size))
                                    return false;

                                begin += size;
                                break;
                            }

                        case e_wire_type::start_group:
                            {
                                if (++it == stack.end())
                                    return false;

                                *it = id;
                                break;
                            }

                        case e_wire_type::end_group:
                            {
                                if (it == stack.begin())
                                {
                                    if (*it != id)
                                        return false;

                                    last = end;
                                    return true;
                                }

                                if (*(it--) != id)
                                    return false;

                                break;
                            }
                    
                        case e_wire_type::fixed_64: 
                            {
                                if (end - begin < 8)
                                    return false;

                                begin += 8;
                                break;
                            }

                        case e_wire_type::fixed_32:
                            {
                                if (end - begin < 4)
                                    return false;

                                begin += 4;
                                break;
                            }
                            
                        default: return false;
                    }
                }
            }

            template <typename Input, typename Output>
            bool read_unknown (
                uint32_t field_id, uint8_t wire_type,
                Input&& in, Output&& out)
            {
                using serializable::dispatch_read;

                switch (static_cast <e_wire_type> (wire_type))
                {
                    case e_wire_type::varint:
                        {
                            uint64_t tmp;
                            return dispatch_read <uint64> (in, tmp);
                        }

                    case e_wire_type::length_delimited:
                        {
                            std::size_t size;
                            if (!dispatch_read <varint <std::size_t>> (in, size))
                                return false;

                            return in.seekg(size, std::ios_base::cur);
                        }

                    case e_wire_type::start_group: return read_unknown_group(field_id, in, out);
                    case e_wire_type::end_group: return false;
                    case e_wire_type::fixed_64: return in.seekg(8, std::ios_base::cur);
                    case e_wire_type::fixed_32: return in.seekg(4, std::ios_base::cur);
                }

                return false;
            }

            template <typename Output>
            bool read_unknown (
                uint32_t field_id, uint8_t wire_type, 
                char const* begin, char const* end, char const*& it,
                Output&& out)
            {
                using serializable::dispatch_read;

                switch (static_cast <e_wire_type> (wire_type))
                {
                    case e_wire_type::varint:
                        {
                            uint64_t tmp;
                            return dispatch_read <uint64> (begin, end, it, tmp);
                        }

                    case e_wire_type::length_delimited:
                        {
                            std::size_t size;
                            if (!dispatch_read <varint <std::size_t>> (begin, end, begin, size))
                                return false;
                            if (end - begin < static_cast <long> (size))
                                return false;

                            it = begin + size;
                            return true;
                        }

                    case e_wire_type::fixed_64:
                        {
                            if (end - begin < 8)
                                return false;

                            it = begin + 8;
                            return true;
                        }
                    
                    case e_wire_type::fixed_32: 
                        {
                            if (end - begin < 4)
                                return false;

                            it = begin + 4;
                            return true;
                        }

                    case e_wire_type::start_group: return read_unknown_group(field_id, begin, end, it, out);
                    case e_wire_type::end_group: return false;
                }

                return false;
            }

#define FRAMEWORK_VARIADIC_CASE_LIST(z, n, text) \
        BOOST_PP_COMMA_IF(n) text c_##n

#define FRAMEWORK_VARIADIC_CASE(z, n, text) \
            case field_number <c_ ## n>::value: \
                { \
                    using current_field = c_ ## n; \
                    enum{ required_index = find_pack <RequiredPack, field_matcher <field_number <current_field>::value>::template type>::value }; \
\
                    if (!serializable::dispatch_read <current_field> (args..., wire_type)) \
                        return false; \
                    /*static*/ if (is_required <current_field>::value) \
                        required.set(required_index); \
                    break; \
                }

#define FRAMEWORK_VARIADIC_SWITCH(n) \
            template <typename RequiredPack, BOOST_PP_REPEAT(n, FRAMEWORK_VARIADIC_CASE_LIST, typename)> \
            struct message_switch <RequiredPack, pack_container <BOOST_PP_REPEAT(n, FRAMEWORK_VARIADIC_CASE_LIST, )>> \
            { \
                template <typename Required, typename... Args> \
                FRAMEWORK_ALWAYS_INLINE \
                static bool run (uint32_t field_id, uint8_t wire_type, Required&& required, Args&&... args) \
                { \
                    using serializable::dispatch_read; \
                    using switch_fields = pack_container <BOOST_PP_REPEAT(n, FRAMEWORK_VARIADIC_CASE_LIST, )>; \
\
                    switch (field_id) \
                    { \
                        BOOST_PP_REPEAT(n, FRAMEWORK_VARIADIC_CASE, ) \
\
                        default: \
                            return read_unknown(field_id, wire_type, \
                                std::forward <Args> (args)...); \
                    } \
\
                    return true; \
                } \
            };

            template <typename RequiredPack, BOOST_PP_REPEAT(FRAMEWORK_PROTOCOL_BUFFERS_SWITCH_LIMIT, FRAMEWORK_VARIADIC_CASE_LIST, typename), typename... Tail>
            struct message_switch <RequiredPack, pack_container <BOOST_PP_REPEAT(FRAMEWORK_PROTOCOL_BUFFERS_SWITCH_LIMIT, FRAMEWORK_VARIADIC_CASE_LIST, ), Tail...>>
            {
                template <typename Required, typename... Args>
                FRAMEWORK_ALWAYS_INLINE
                static bool run (uint32_t field_id, uint8_t wire_type, Required&& required, Args&&... args)
                {
                    using switch_fields = pack_container <BOOST_PP_REPEAT(FRAMEWORK_PROTOCOL_BUFFERS_SWITCH_LIMIT, FRAMEWORK_VARIADIC_CASE_LIST, )>;
                    switch (field_id)
                    {
                        BOOST_PP_REPEAT(FRAMEWORK_PROTOCOL_BUFFERS_SWITCH_LIMIT, FRAMEWORK_VARIADIC_CASE, )

                        default:
                            return message_switch <RequiredPack, pack_container <Tail...>>::run(
                                field_id, wire_type,
                                std::forward <Required> (required),
                                std::forward <Args> (args)...);
                    }

                    return true;
                }
            };

            template <typename RequiredPack>
            struct message_switch <RequiredPack, pack_container <>>
            {
                template <typename Required, typename... Args>
                FRAMEWORK_ALWAYS_INLINE
                static bool run (uint32_t field_id, uint8_t wire_type, Required&& required, Args&&... args)
                {
                    return read_unknown(field_id, wire_type,
                        std::forward <Required> (required),
                        std::forward <Args> (args)...);
                }
            };

#define BOOST_PP_LOCAL_LIMITS (1, FRAMEWORK_PROTOCOL_BUFFERS_SWITCH_LIMIT-1)
#define BOOST_PP_LOCAL_MACRO(n) FRAMEWORK_VARIADIC_SWITCH(n)

#ifdef __clang__
 #pragma clang diagnostic push
 #pragma clang diagnostic ignored "-Wtrigraphs"
#endif

??=include BOOST_PP_LOCAL_ITERATE()

#ifdef __clang__
 #pragma clang diagnostic pop
#endif

#undef BOOST_PP_LOCAL_LIMITS
#undef BOOST_PP_LOCAL_MACRO

#undef FRAMEWORK_VARIADIC_CASE_LIST
#undef FRAMEWORK_VARIADIC_CASE
#undef FRAMEWORK_VARIADIC_CASE_EXPECT
#undef FRAMEWORK_VARIADIC_SWITCH
        }
    }
}

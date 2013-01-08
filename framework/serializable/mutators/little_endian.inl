// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

#include <framework/common/endian.hpp>
#include <framework/common/byte_swap.hpp>

namespace framework
{
    namespace serializable
    {
        namespace little_endian_impl
        {
#if FRAMEWORK_HOST_ENDIANNESS == FRAMEWORK_LITTLE_ENDIAN
    #define TO_HOST(Type, Macro) \
            template <typename T> \
            struct to_host <T, \
                typename std::enable_if < \
                    std::is_integral <T>::value && \
                    sizeof(T) == sizeof(Type), \
                    void \
                >::type> \
            { \
                static T run (T const& value) \
                { \
                    return value; \
                } \
            }
#elif FRAMEWORK_HOST_ENDIANNESS == FRAMEWORK_BIG_ENDIAN
    #define TO_HOST(Type, Macro) \
            template <typename T> \
            struct to_host <T, \
                typename std::enable_if < \
                    std::is_integral <T>::value && \
                    sizeof(T) == sizeof(Type), \
                    void \
                >::type> \
            { \
                static T run (T const& value) \
                { \
                    return static_cast <T> (Macro(static_cast <Type> (value))); \
                } \
            }
#else
    #error "little_endian does not support host endianness"
#endif

            TO_HOST(uint8_t, FRAMEWORK_BYTESWAP8);
            TO_HOST(uint16_t, FRAMEWORK_BYTESWAP16);
            TO_HOST(uint32_t, FRAMEWORK_BYTESWAP32);
            TO_HOST(uint64_t, FRAMEWORK_BYTESWAP64);
#undef TO_HOST

#if FRAMEWORK_HOST_FLOAT_ENDIANNESS == FRAMEWORK_LITTLE_ENDIAN
    #define TO_HOST(Type, Macro) \
            template <typename T> \
            struct to_host <T, \
                typename std::enable_if < \
                    std::is_floating_point <T>::value && \
                    sizeof(T) == sizeof(Type), \
                    void \
                >::type> \
            { \
                static T run (T const& value) \
                { \
                    return value; \
                } \
            }
#elif FRAMEWORK_HOST_FLOAT_ENDIANNESS == FRAMEWORK_BIG_ENDIAN
    #define TO_HOST(Type, Macro) \
            template <typename T> \
            struct to_host <T, \
                typename std::enable_if < \
                    std::is_floating_point <T>::value && \
                    sizeof(T) == sizeof(Type), \
                    void \
                >::type> \
            { \
                static T run (T const& value) \
                { \
                    return static_cast <T> (Macro(static_cast <Type> (value))); \
                } \
            }
#else
    #error "little_endian does not support host endianness"
#endif

            TO_HOST(uint8_t, FRAMEWORK_BYTESWAP8);
            TO_HOST(uint16_t, FRAMEWORK_BYTESWAP16);
            TO_HOST(uint32_t, FRAMEWORK_BYTESWAP32);
            TO_HOST(uint64_t, FRAMEWORK_BYTESWAP64);
#undef TO_HOST

            // The implementations are symmetric for the current supported endianness
            template <typename T, typename Enabler>
            struct from_host
            {
                static T run (T const& value)
                {
                    return to_host <T>::run(value);
                }
            };
        }
    }
}

// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

namespace framework
{
    namespace detail
    {
#if (FRAMEWORK_HOST_ENDIANNESS == FRAMEWORK_LITTLE_ENDIAN) || (FRAMEWORK_HOST_ENDIANNESS == FRAMEWORK_BIG_ENDIAN)

        //-------------------------
        // Wire type: little endian
        //-------------------------

    #if FRAMEWORK_HOST_ENDIANNESS == FRAMEWORK_LITTLE_ENDIAN
        #define TO_HOST(Type, ByteMacro, WordMacro) \
            template <typename T> \
            struct little_to_host_impl <T, \
                typename std::enable_if < \
                    std::is_integral <T>::value && \
                    sizeof(T) == sizeof(Type), \
                    void \
                >::type> \
            { \
                FRAMEWORK_ALWAYS_INLINE \
                static T run (T const& value) \
                { \
                    return value; \
                } \
            }
    #elif FRAMEWORK_HOST_ENDIANNESS == FRAMEWORK_BIG_ENDIAN
        #define TO_HOST(Type, ByteMacro, WordMacro) \
            template <typename T> \
            struct little_to_host_impl <T, \
                typename std::enable_if < \
                    std::is_integral <T>::value && \
                    sizeof(T) == sizeof(Type), \
                    void \
                >::type> \
            { \
                FRAMEWORK_ALWAYS_INLINE \
                static T run (T const& value) \
                { \
                    return static_cast <T> (ByteMacro(static_cast <Type> (value))); \
                } \
            }
    #elif FRAMEWORK_HOST_ENDIANNESS == FRAMEWORK_PDP_ENDIAN
        #define TO_HOST(Type, ByteMacro, WordMaco) \
            template <typename T> \
            struct little_to_host_impl <T, \
                typename std::enable_if < \
                    std::is_integral <T>::value && \
                    sizeof(T) == sizeof(Type), \
                    void \
                >::type> \
            { \
                FRAMEWORK_ALWAYS_INLINE \
                static T run (T const& value) \
                { \
                    return static_cast <T> (WordMacro(static_cast <Type> (value))); \
                } \
            }
    #else
        #error "logic error"
    #endif
            TO_HOST(uint8_t,  FRAMEWORK_BYTESWAP8,  FRAMEWORK_WORDSWAP8);
            TO_HOST(uint16_t, FRAMEWORK_BYTESWAP16, FRAMEWORK_WORDSWAP16);
            TO_HOST(uint32_t, FRAMEWORK_BYTESWAP32, FRAMEWORK_WORDSWAP32);
            TO_HOST(uint64_t, FRAMEWORK_BYTESWAP64, FRAMEWORK_WORDSWAP64);
    #undef TO_HOST

    #if FRAMEWORK_HOST_FLOAT_ENDIANNESS == FRAMEWORK_LITTLE_ENDIAN
        #define TO_HOST(Type, ByteMacro, WordMacro) \
            template <typename T> \
            struct little_to_host_impl <T, \
                typename std::enable_if < \
                    std::is_floating_point <T>::value && \
                    sizeof(T) == sizeof(Type), \
                    void \
                >::type> \
            { \
                FRAMEWORK_ALWAYS_INLINE \
                static T run (T const& value) \
                { \
                    return value; \
                } \
            }
    #elif FRAMEWORK_HOST_FLOAT_ENDIANNESS == FRAMEWORK_BIG_ENDIAN
        #define TO_HOST(Type, ByteMacro, WordMacro) \
            template <typename T> \
            struct little_to_host_impl <T, \
                typename std::enable_if < \
                    std::is_floating_point <T>::value && \
                    sizeof(T) == sizeof(Type), \
                    void \
                >::type> \
            { \
                FRAMEWORK_ALWAYS_INLINE \
                static T run (T const& value) \
                { \
                    return static_cast <T> (ByteMacro(static_cast <Type> (value))); \
                } \
            }
    #elif FRAMEWORK_HOST_FLOAT_ENDIANNESS == FRAMEWORK_PDP_ENDIAN
        #define TO_HOST(Type, ByteMacro, WordMacro) \
            template <typename T> \
            struct little_to_host_impl <T, \
                typename std::enable_if < \
                    std::is_floating_point <T>::value && \
                    sizeof(T) == sizeof(Type), \
                    void \
                >::type> \
            { \
                FRAMEWORK_ALWAYS_INLINE \
                static T run (T const& value) \
                { \
                    return static_cast <T> (WordMacro(static_cast <Type> (value))); \
                } \
            }
    #else
        #error "logic error"
    #endif
            TO_HOST(uint8_t,  FRAMEWORK_BYTESWAP8,  FRAMEWORK_WORDSWAP8);
            TO_HOST(uint16_t, FRAMEWORK_BYTESWAP16, FRAMEWORK_WORDSWAP16);
            TO_HOST(uint32_t, FRAMEWORK_BYTESWAP32, FRAMEWORK_WORDSWAP32);
            TO_HOST(uint64_t, FRAMEWORK_BYTESWAP64, FRAMEWORK_WORDSWAP64);
    #undef TO_HOST

        // The implementations are symmetric for the current supported types
        template <typename T, typename Enabler>
        struct host_to_little_impl
        {
            FRAMEWORK_ALWAYS_INLINE
            static T run (T const& value)
            {
                return little_to_host(value);
            }
        };

        //----------------------
        // Wire type: big endian
        //----------------------

    #if FRAMEWORK_HOST_ENDIANNESS == FRAMEWORK_BIG_ENDIAN
        #define TO_HOST(Type, ByteMacro, WordMacro) \
            template <typename T> \
            struct big_to_host_impl <T, \
                typename std::enable_if < \
                    std::is_integral <T>::value && \
                    sizeof(T) == sizeof(Type), \
                    void \
                >::type> \
            { \
                FRAMEWORK_ALWAYS_INLINE \
                static T run (T const& value) \
                { \
                    return value; \
                } \
            }
    #elif FRAMEWORK_HOST_ENDIANNESS == FRAMEWORK_LITTLE_ENDIAN
        #define TO_HOST(Type, ByteMacro, WordMacro) \
            template <typename T> \
            struct big_to_host_impl <T, \
                typename std::enable_if < \
                    std::is_integral <T>::value && \
                    sizeof(T) == sizeof(Type), \
                    void \
                >::type> \
            { \
                FRAMEWORK_ALWAYS_INLINE \
                static T run (T const& value) \
                { \
                    return static_cast <T> (ByteMacro(static_cast <Type> (value))); \
                } \
            }
    #elif FRAMEWORK_HOST_ENDIANNESS == FRAMEWORK_PDP_ENDIAN
        #define TO_HOST(Type, ByteMacro, WordMaco) \
            template <typename T> \
            struct big_to_host_impl <T, \
                typename std::enable_if < \
                    std::is_integral <T>::value && \
                    sizeof(T) == sizeof(Type), \
                    void \
                >::type> \
            { \
                FRAMEWORK_ALWAYS_INLINE \
                static T run (T const& value) \
                { \
                    return static_cast <T> (WordMacro(ByteMacro(static_cast <Type> (value)))); \
                } \
            }
    #else
        #error "logic error"
    #endif
            TO_HOST(uint8_t,  FRAMEWORK_BYTESWAP8,  FRAMEWORK_WORDSWAP8);
            TO_HOST(uint16_t, FRAMEWORK_BYTESWAP16, FRAMEWORK_WORDSWAP16);
            TO_HOST(uint32_t, FRAMEWORK_BYTESWAP32, FRAMEWORK_WORDSWAP32);
            TO_HOST(uint64_t, FRAMEWORK_BYTESWAP64, FRAMEWORK_WORDSWAP64);
    #undef TO_HOST

    #if FRAMEWORK_HOST_FLOAT_ENDIANNESS == FRAMEWORK_BIG_ENDIAN
        #define TO_HOST(Type, ByteMacro, WordMacro) \
            template <typename T> \
            struct big_to_host_impl <T, \
                typename std::enable_if < \
                    std::is_floating_point <T>::value && \
                    sizeof(T) == sizeof(Type), \
                    void \
                >::type> \
            { \
                FRAMEWORK_ALWAYS_INLINE \
                static T run (T const& value) \
                { \
                    return value; \
                } \
            }
    #elif FRAMEWORK_HOST_FLOAT_ENDIANNESS == FRAMEWORK_LITTLE_ENDIAN
        #define TO_HOST(Type, ByteMacro, WordMacro) \
            template <typename T> \
            struct big_to_host_impl <T, \
                typename std::enable_if < \
                    std::is_floating_point <T>::value && \
                    sizeof(T) == sizeof(Type), \
                    void \
                >::type> \
            { \
                FRAMEWORK_ALWAYS_INLINE \
                static T run (T const& value) \
                { \
                    return static_cast <T> (ByteMacro(static_cast <Type> (value))); \
                } \
            }
    #elif FRAMEWORK_HOST_FLOAT_ENDIANNESS == FRAMEWORK_PDP_ENDIAN
        #define TO_HOST(Type, ByteMacro, WordMacro) \
            template <typename T> \
            struct big_to_host_impl <T, \
                typename std::enable_if < \
                    std::is_floating_point <T>::value && \
                    sizeof(T) == sizeof(Type), \
                    void \
                >::type> \
            { \
                FRAMEWORK_ALWAYS_INLINE \
                static T run (T const& value) \
                { \
                    return static_cast <T> (WordMacro(ByteMacro(static_cast <Type> (value)))); \
                } \
            }
    #else
        #error "logic error"
    #endif
            TO_HOST(uint8_t,  FRAMEWORK_BYTESWAP8,  FRAMEWORK_WORDSWAP8);
            TO_HOST(uint16_t, FRAMEWORK_BYTESWAP16, FRAMEWORK_WORDSWAP16);
            TO_HOST(uint32_t, FRAMEWORK_BYTESWAP32, FRAMEWORK_WORDSWAP32);
            TO_HOST(uint64_t, FRAMEWORK_BYTESWAP64, FRAMEWORK_WORDSWAP64);
    #undef TO_HOST

        // The implementations are symmetric for the current supported types
        template <typename T, typename Enabler>
        struct host_to_big_impl
        {
            FRAMEWORK_ALWAYS_INLINE
            static T run (T const& value)
            {
                return big_to_host(value);
            }
        };

        //----------------------
        // Wire type: pdp endian
        //----------------------

    #if FRAMEWORK_HOST_ENDIANNESS == FRAMEWORK_PDP_ENDIAN
        #define TO_HOST(Type, ByteMacro, WordMacro) \
            template <typename T> \
            struct pdp_to_host_impl <T, \
                typename std::enable_if < \
                    std::is_integral <T>::value && \
                    sizeof(T) == sizeof(Type), \
                    void \
                >::type> \
            { \
                FRAMEWORK_ALWAYS_INLINE \
                static T run (T const& value) \
                { \
                    return value; \
                } \
            }
    #elif FRAMEWORK_HOST_ENDIANNESS == FRAMEWORK_LITTLE_ENDIAN
        #define TO_HOST(Type, ByteMacro, WordMacro) \
            template <typename T> \
            struct pdp_to_host_impl <T, \
                typename std::enable_if < \
                    std::is_integral <T>::value && \
                    sizeof(T) == sizeof(Type), \
                    void \
                >::type> \
            { \
                FRAMEWORK_ALWAYS_INLINE \
                static T run (T const& value) \
                { \
                    return static_cast <T> (WordMacro(static_cast <Type> (value))); \
                } \
            }
    #elif FRAMEWORK_HOST_ENDIANNESS == FRAMEWORK_BIG_ENDIAN
        #define TO_HOST(Type, ByteMacro, WordMaco) \
            template <typename T> \
            struct pdp_to_host_impl <T, \
                typename std::enable_if < \
                    std::is_integral <T>::value && \
                    sizeof(T) == sizeof(Type), \
                    void \
                >::type> \
            { \
                FRAMEWORK_ALWAYS_INLINE \
                static T run (T const& value) \
                { \
                    return static_cast <T> (WordMacro(ByteMacro(static_cast <Type> (value)))); \
                } \
            }
    #else
        #error "logic error"
    #endif
            TO_HOST(uint8_t,  FRAMEWORK_BYTESWAP8,  FRAMEWORK_WORDSWAP8);
            TO_HOST(uint16_t, FRAMEWORK_BYTESWAP16, FRAMEWORK_WORDSWAP16);
            TO_HOST(uint32_t, FRAMEWORK_BYTESWAP32, FRAMEWORK_WORDSWAP32);
            TO_HOST(uint64_t, FRAMEWORK_BYTESWAP64, FRAMEWORK_WORDSWAP64);
    #undef TO_HOST

    #if FRAMEWORK_HOST_FLOAT_ENDIANNESS == FRAMEWORK_PDP_ENDIAN
        #define TO_HOST(Type, ByteMacro, WordMacro) \
            template <typename T> \
            struct pdp_to_host_impl <T, \
                typename std::enable_if < \
                    std::is_floating_point <T>::value && \
                    sizeof(T) == sizeof(Type), \
                    void \
                >::type> \
            { \
                FRAMEWORK_ALWAYS_INLINE \
                static T run (T const& value) \
                { \
                    return value; \
                } \
            }
    #elif FRAMEWORK_HOST_FLOAT_ENDIANNESS == FRAMEWORK_LITTLE_ENDIAN
        #define TO_HOST(Type, ByteMacro, WordMacro) \
            template <typename T> \
            struct pdp_to_host_impl <T, \
                typename std::enable_if < \
                    std::is_floating_point <T>::value && \
                    sizeof(T) == sizeof(Type), \
                    void \
                >::type> \
            { \
                FRAMEWORK_ALWAYS_INLINE \
                static T run (T const& value) \
                { \
                    return static_cast <T> (WordMacro(static_cast <Type> (value))); \
                } \
            }
    #elif FRAMEWORK_HOST_FLOAT_ENDIANNESS == FRAMEWORK_BIG_ENDIAN
        #define TO_HOST(Type, ByteMacro, WordMacro) \
            template <typename T> \
            struct pdp_to_host_impl <T, \
                typename std::enable_if < \
                    std::is_floating_point <T>::value && \
                    sizeof(T) == sizeof(Type), \
                    void \
                >::type> \
            { \
                FRAMEWORK_ALWAYS_INLINE \
                static T run (T const& value) \
                { \
                    return static_cast <T> (WordMacro(ByteMacro(static_cast <Type> (value)))); \
                } \
            }
    #else
        #error "logic error"
    #endif
            TO_HOST(uint8_t,  FRAMEWORK_BYTESWAP8,  FRAMEWORK_WORDSWAP8);
            TO_HOST(uint16_t, FRAMEWORK_BYTESWAP16, FRAMEWORK_WORDSWAP16);
            TO_HOST(uint32_t, FRAMEWORK_BYTESWAP32, FRAMEWORK_WORDSWAP32);
            TO_HOST(uint64_t, FRAMEWORK_BYTESWAP64, FRAMEWORK_WORDSWAP64);
    #undef TO_HOST

        // The implementations are symmetric for the current supported types
        template <typename T, typename Enabler>
        struct host_to_pdp_impl
        {
            FRAMEWORK_ALWAYS_INLINE
            static T run (T const& value)
            {
                return pdp_to_host(value);
            }
        };
#endif
    }
}

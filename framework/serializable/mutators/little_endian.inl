// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

#include <algorithm>

#include <framework/common/endian.hpp>

namespace framework
{
    namespace serializable
    {
        // TODO: Replace with wrapper around portable endianness conversion library
        //       if possible (possibly Boost.Endian if added).  This implementation is
        //       a trivial placeholder; efficiency was not a priority here.

        namespace detail
        {
            template <typename T>
            struct little_endian_impl <T, typename std::enable_if <std::is_integral <T>::value, void>::type>
            {
                static void to_host (T& value)
                {
                    (void)value; // suppress unused value warning
#if FRAMEWORK_HOST_ENDIANNESS == FRAMEWORK_LITTLE_ENDIAN
#elif FRAMEWORK_HOST_ENDIANNESS == FRAMEWORK_BIG_ENDIAN
                    std::reverse(reinterpret_cast <char*> (&value), reinterpret_cast <char*> (&value) + sizeof(value));
#else
                    static_assert(!std::is_same <T, T>::value, "little_endian does not support host endianness");
#endif
                }

                static void from_host (T& value)
                {
                    // identical to to_host for supported host types
                    to_host(value);
                }
            };

            template <typename T>
            struct little_endian_impl <T, typename std::enable_if <std::is_floating_point <T>::value, void>::type>
            {
                static void to_host (T& value)
                {
                    (void)value; // suppress unused value warning
#if FRAMEWORK_HOST_FLOAT_ENDIANNESS == FRAMEWORK_LITTLE_ENDIAN
#elif FRAMEWORK_HOST_FLOAT_ENDIANNESS == FRAMEWORK_BIG_ENDIAN
                    std::reverse(reinterpret_cast <char*> (&value), reinterpret_cast <char*> (&value) + sizeof(value));
#else
                    static_assert(false, "little_endian does not support host endianness");
#endif
                }

                static void from_host (T& value)
                {
                    // identical to to_host for supported host types
                    to_host(value);
                }
            };
        }
    }
}

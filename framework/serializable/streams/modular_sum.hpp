// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Internet checksum wrapper.
*
* \copyright
* Copyright &copy; 2012 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

#include <string.h>
#include <cstdint>
#include <cassert>
#include <algorithm>

#include <framework/common/endian.hpp>

namespace framework
{
    namespace serializable
    {
        template <std::size_t Bytes, byte_order Order>
        class modular_sum;

        template <byte_order Order>
        class modular_sum <2, Order>
        {
            static_assert(
                Order == byte_order::little_endian || 
                Order == byte_order::big_endian,
                "Specified byte order not implemented");

            private:
                template <std::size_t N, bool State>
                bool write_impl (char const* s);

                template <bool State>
                bool write_impl (char const* s, std::size_t n);

            public:
                template <bool Flip = false>
                bool write (char const* s, std::size_t n)
                {
                    if (p_bState != Flip)
                        return write_impl <false> (s, n);
                    else
                        return write_impl <true> (s, n);
                }

                template <std::size_t N, bool Flip = false>
                bool write (char const* s) 
                {
                    if (p_bState != Flip)
                        return write_impl <N, false> (s);
                    else
                        return write_impl <N, true> (s);
                }

                uint16_t checksum () const
                {
                    uint64_t result = p_iSum;
                    while (result >> 16)
                        result = (result & 0xFFFF) + (result >> 16);

                    // The performance impact of performing this check at runtime was found to be negligible
                    if (Order == byte_order::little_endian)
                    {
                        uint16_t test = 0x0102;
                        if (static_cast <uint8_t> (*reinterpret_cast <char const*> (&test)) == 0x01)
                            return ~FRAMEWORK_BYTESWAP16(static_cast <uint16_t> (result));
                        else
                            return ~static_cast <uint16_t> (result);
                    }
                    else if (Order == byte_order::big_endian)
                    {
                        uint16_t test = 0x0102;
                        if (static_cast <uint8_t> (*reinterpret_cast <char const*> (&test)) == 0x01)
                            return ~static_cast <uint16_t> (result);
                        else
                            return ~FRAMEWORK_BYTESWAP16(static_cast <uint16_t> (result));
                    }
                    else
                    {
                        assert(false);
                    }
                }

                void reset ()
                {
                    p_iSum = 0;
                }

            private:
                bool add (uint64_t x)
                {
                    // We can do marginally better here if we don't fold or otherwise account for overflow 
                    // (reasonably safe - on the order of 256TB of data would need to be summed to overflow),
                    // however in practice the effect was found to be small when compared to the optimized
                    // overflow handlers below.

#if defined(__clang__)
                    auto const result = p_iSum + x;
                    p_iSum = result + (result < x);
#elif defined(__GNUC__)
                    p_iSum += x;                    
                    if (static_cast <int64_t> (p_iSum) < 0)
                        p_iSum = (p_iSum & 0xFFFF) + (p_iSum >> 16);
#else
                    p_iSum += x;
                    p_iSum = (p_iSum & 0xFFFF) + (p_iSum >> 16);
#endif
                    return true;
                }

            private:
                uint64_t p_iSum {0};
                bool p_bState {true};
        };

        using internet_checksum = modular_sum <2, byte_order::big_endian>;
    }
}

#include <framework/serializable/streams/modular_sum.inl>

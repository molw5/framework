// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Modular sum writer.
*
* \copyright
* Copyright &copy; 2012 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

#include <cstdint>
#include <cassert>

#include <framework/common/endian.hpp>
#include <framework/common/variadic_switch_fallthrough.hpp>

#include <iostream>

namespace framework
{
    namespace serializable
    {
        /**
        * \brief Modular sum writer.
        *
        * Computes the 8*N bit one's complement sum of the bytes received.  Support is provided
        * for big endian and little endian byte sequences.
        *
        * \tparam N number of bytes to use in the sum
        * \tparam Order assumed input byte sequence endianness
        */
        template <std::size_t N = 2, framework::byte_order Order = framework::byte_order::big_endian>
        class modular_sum final
        {
            private:
                // Note - the buffer type is required to exceed the size of the value type here.
                // If the buffer size is equal to the value type the carries may not be deferred.
                using buffer_type = 
                    typename std::conditional <
                        (N <= 1),
                        uint_fast16_t,
                        typename std::conditional <
                            (N <= 3),
                            uint_fast32_t,
                            typename std::enable_if <
                                (N <= 7),
                                uint_fast64_t
                            >::type
                        >::type
                    >::type;

                using value_type = 
                    typename std::conditional <
                        N == 1,
                        uint8_t,
                        typename std::conditional <
                            N == 2,
                            uint16_t,
                            typename std::conditional <
                                N == 3 || N == 4,
                                uint32_t,
                                typename std::conditional <
                                    (N > 4 && N <= 8),
                                    uint64_t,
                                    void
                                >::type
                            >::type
                        >::type
                    >::type;

                enum 
                { 
                    value_mask = (~static_cast <buffer_type> (0)) >> (8*(sizeof(buffer_type)-N)),
                    value_shift = 8*N,
                    offset = Order == framework::byte_order::big_endian ? 8*(N-1) : 0,
                    multiplier = Order == framework::byte_order::big_endian ? -8 : 8,
                    max_deferred = ((static_cast <uint64_t> (1) << 8*(sizeof(buffer_type) - N)) - 2)*N
                };

                class remaining_bytes
                {
                    public:
                        remaining_bytes (char const* &s, buffer_type &result)
                            : p_pString(s),
                              p_iResult(result)
                        {
                        }

                        template <std::size_t I>
                        void operator() ()
                        {
                            static_assert(N > 0, "Invalid invocation");
                            p_iResult += static_cast <buffer_type> (uint8_t(*(p_pString++))) << (offset + multiplier*I);
                        }

                        void operator() ()
                        {
                            assert(false);
                        }

                    private:
                        char const*& p_pString;
                        buffer_type& p_iResult;
                };

            public:
                /**
                * \brief Write bytes.
                *
                * Writes a sequence of bytes to the checksum stream.
                *
                * \note
                * This implementation assumes sizeof(char) == 1; this function will fail with
                * a static assert if this assumption is violated.
                *
                * \param s input character stream
                * \param n number of characters in the input stream
                */
                bool write (char const* s, std::size_t n)
                {
                    static_assert(sizeof(char) == 1, "Not implemented");
                    static_assert(
                        Order == ::framework::byte_order::little_endian || 
                        Order == ::framework::byte_order::big_endian, 
                        "Not implemented");

                    if (n > max_deferred)
                    {
                        write(s, max_deferred);
                        n -= max_deferred;
                        s += max_deferred;
                    }

                    buffer_type result {0};

                    if (n >= N - p_iState)
                    {
                        // Read the remaining partial sequence
                        variadic_switch_fallthrough <make_value_indices <N>> (
                            p_iState, remaining_bytes(s, result));

                        n -= (N - p_iState);
                        p_iState = 0;

                        // Read complete blocks
                        while (n >= N)
                        {
                            // The compiler should be able to unroll this loop
                            for (unsigned int i=0; i < N; ++i)
                                result += static_cast <buffer_type> (uint8_t(*(s++))) << (offset + multiplier*i);

                            n -= N;
                        }
                    }

                    // Read the remaining bytes
                    if (n > 0)
                    {
                        buffer_type tmp {0};

                        // Read the remaining bytes as if it it completed a partial sequence.
                        // For example, given n = 1, p_iState = 2, N = 2, Order = byte_order::big_endianness 
                        // this sequence would be equivalent to:
                        //   tmp += static_cast <buffer_type> (*(s++)); // should be << 8 here
                        variadic_switch_fallthrough <make_value_indices <N>> (
                            N - n, remaining_bytes(s, tmp));

                        // Correct the sequence depending on endianness
                        if (Order == framework::byte_order::big_endian)
                            tmp <<= 8*(n - p_iState);
                        else
                            tmp >>= 8*(n - p_iState);

                        p_iState += n;
                        result += tmp;
                    }

                    // Update the sum
                    p_iSum += result;
                    while (p_iSum >> value_shift)
                        p_iSum = (p_iSum & value_mask) + (p_iSum >> value_shift);

                    return true;
                };

                /**
                * \brief Get checksum
                *
                * Gets the checksum value, defined as the complement of the 16 bit one's complement
                * sum of the input data.
                */
                value_type get () const
                {
                    buffer_type result = p_iSum;
                    while (result >> value_shift)
                        result = (result & value_mask) + (result >> value_shift);

                    return ~result;
                }

            private:
                buffer_type p_iSum {0};
                unsigned int p_iState {0};
        };

        /**
        * \brief Internet checksum alias.
        */
        using internet_checksum = modular_sum <2, ::framework::byte_order::big_endian>;
    }
}

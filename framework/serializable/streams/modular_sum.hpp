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

#include <string.h>
#include <cstdint>
#include <cassert>
#include <algorithm>

#include <framework/common/variadic_switch_return.hpp>
#include <framework/common/endian.hpp>

namespace framework
{
    namespace serializable
    {
        /**
        * \headerfile modular_sum.hpp <framework/serializable/streams/modular_sum.hpp>
        * \brief Modular sum writer.
        *
        * Computes the 8*Bytes bit one's complement sum of the bytes written to this stream.
        *
        * \tparam Bytes number of bytes to use in the sum
        * \tparam Order input byte endianness
        */
        template <std::size_t Bytes, byte_order Order>
        class modular_sum
        {
            static_assert(
                Order == byte_order::little_endian || 
                Order == byte_order::big_endian,
                "Specified byte order not implemented");

            static_assert(
                Bytes <= 8,
                "Specified byte count not implemented");

            private:
                enum{ value_mask = (~static_cast <uint64_t> (0)) >> (64 - 8*Bytes) };
                enum{ value_shift = 8*Bytes };
                enum{ offset = (Order == byte_order::big_endian ? 8*(Bytes-1) : 0) };
                enum{ multiplier = (Order == byte_order::big_endian ? -8 : 8) };

                template <unsigned int State, std::size_t N, std::size_t Index = 0>
                struct unroll
                {
                    FRAMEWORK_ALWAYS_INLINE
                    static uint64_t run (unsigned char const* s)
                    {
                        enum{ shift = offset + multiplier*((State + Index) % Bytes) };
                        return (static_cast <uint64_t> (*s) << shift) | unroll <State, N, Index+1>::run(s+1);
                    }
                };

                template <unsigned int State, std::size_t N>
                struct unroll <State, N, N>
                {
                    FRAMEWORK_ALWAYS_INLINE
                    static uint64_t run (unsigned char const*)
                    {
                        return 0;
                    }
                };

                template <std::size_t N>
                struct handler_fixed
                {
                    template <std::size_t I, typename T>
                    FRAMEWORK_ALWAYS_INLINE
                    bool operator() (T* this_ptr, unsigned char const* s)
                    {
                        enum{ new_state = (I+N) % Bytes };
                        enum{ loop_count = N / Bytes };
                        enum{ remainder = N % Bytes };

                        for (unsigned int i=0; i < loop_count; ++i, s += Bytes)
                            this_ptr->add(unroll <I, Bytes>::run(s));

                        this_ptr->add(unroll <I, remainder>::run(s));
                        this_ptr->p_iState = new_state;
                        return true;
                    }

                    template <typename T>
                    FRAMEWORK_ALWAYS_INLINE
                    bool operator() (T*, unsigned char const*)
                    {
                        assert(false);
                    }
                };

                struct handler_dynamic
                {
                    template <std::size_t I, typename T>
                    FRAMEWORK_ALWAYS_INLINE
                    bool operator() (T* this_ptr, unsigned char const* s, std::size_t n)
                    {
                        this_ptr->p_iState = (I + n) % Bytes;

                        for (; n >= Bytes; s += Bytes, n -= Bytes)
                            this_ptr->add(unroll <I, Bytes>::run(s));

                        switch (n)
                        {
                            case 7: return this_ptr->add(unroll <I, 7>::run(s));
                            case 6: return this_ptr->add(unroll <I, 6>::run(s));
                            case 5: return this_ptr->add(unroll <I, 5>::run(s));
                            case 4: return this_ptr->add(unroll <I, 4>::run(s));
                            case 3: return this_ptr->add(unroll <I, 3>::run(s));
                            case 2: return this_ptr->add(unroll <I, 2>::run(s));
                            case 1: return this_ptr->add(unroll <I, 1>::run(s));
                            case 0: return true;
                            default: assert(false);
                        }
                    }

                    template <typename T>
                    FRAMEWORK_ALWAYS_INLINE
                    bool operator() (T*, unsigned char const*, std::size_t)
                    {
                        assert(false);
                    }
                };

            public:
                /**
                * \brief Write bytes.
                */ 
                template <std::size_t N>
                bool write (char const* s) 
                {
                    using values = make_values <std::size_t, Bytes>;
                    auto const ptr = reinterpret_cast <unsigned char const*> (s);
                    return variadic_switch_return <values> (p_iState, handler_fixed <N> (), this, ptr);
                }

                /**
                * \brief Write bytes.
                */ 
                bool write (char const* s, std::size_t n)
                {
                    using values = make_values <std::size_t, Bytes>;
                    auto const ptr = reinterpret_cast <unsigned char const*> (s);
                    return variadic_switch_return <values> (p_iState, handler_dynamic(), this, ptr, n);
                }

                /**
                * \brief Get checksum.
                *
                * Returns the checksum, defined as the complement of the current sum.
                */
                uint64_t checksum () const
                {
                    uint64_t result = p_iSum;
                    while (result >> value_shift)
                        result = (result & value_mask) + (result >> value_shift);

                    return (~result) & value_mask;
                }

                /**
                * \brief Reset.
                */
                void reset ()
                {
                    p_iSum = 0;
                }

            private:
                FRAMEWORK_ALWAYS_INLINE
                bool add (uint64_t x)
                {
                    auto const result = p_iSum + x;
                    p_iSum = result + (result < x);

                    return true;
                }

            private:
                uint64_t p_iSum {0};
                unsigned int p_iState {0};
        };

        using internet_checksum = modular_sum <2, byte_order::big_endian>;
    }
}

#include <framework/serializable/streams/modular_sum.inl>

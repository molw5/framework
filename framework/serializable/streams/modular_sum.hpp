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

#include <framework/common/endian.hpp>
#include <framework/common/variadic_switch_fallthrough.hpp>
#include <framework/common/variadic_switch_return.hpp>

#include <iostream>

namespace framework
{
    namespace serializable
    {
        namespace detail
        {
            template <unsigned int State, std::size_t N, std::size_t Index = 0, typename Enabler = void>
            struct unroll
            {
                template <typename T>
                static typename T::buffer_type run (T* this_ptr, char const* s)
                {
                    using buffer_type = typename T::buffer_type;
                    enum { shift = T::offset + T::multiplier*((State + Index) % T::byte_count) };

                    return (static_cast <buffer_type> (uint8_t(*s)) << shift) +
                           unroll <State, N, Index+1>::run(this_ptr, s+1);
                }
            };

            template <unsigned int State, std::size_t N>
            struct unroll <State, N, N, void>
            {
                template <typename T>
                static typename T::buffer_type run (T*, char const*)
                {
                    return 0;
                }
            };

            // TODO: Run some simulations to establish a reasonable limit here - 32 was selected arbitrarily.
            template <unsigned int State, std::size_t N>
            struct unroll <State, N, 0, typename std::enable_if <(N > 32) && (N <= 1024), void>::type>
            {
                template <typename T>
                static typename T::buffer_type run (T* this_ptr, char const* s)
                {
                    enum { loop_count = N / 32 };
                    enum { remainder = N % 32 };

                    typename T::buffer_type result {0};
                    for (std::size_t i=0; i < loop_count; ++i, s += 32)
                        result += unroll <State, 32>::run(this_ptr, s);

                    return result + unroll <State, remainder>::run(this_ptr, s);
                }
            };

            // TODO: Placeholder, this limit should be based on the number of overflow bits present in buffer_type.
            template <unsigned int State, std::size_t N>
            struct unroll <State, N, 0, typename std::enable_if <(N > 1024), void>::type>
            {
                template <typename T>
                static typename T::buffer_type run (T* this_ptr, char const* s)
                {
                    enum { loop_count = N / 1024 };
                    enum { remainder = N % 1024 };

                    typename T::buffer_type result {0};
                    for (std::size_t i=0; i < loop_count; ++i, s += 1024)
                    {
                        result += unroll <State, 1024>::run(this_ptr, s);
                        result = (result & T::value_mask) + (result >> T::value_shift);
                    }

                    return result + unroll <State, remainder>::run(this_ptr, s);
                }
            };
        
            template <std::size_t ByteCount, framework::byte_order ByteOrder>
            class modular_sum_impl
            {
                template <unsigned int State, std::size_t K, std::size_t Index, typename Enabler>
                friend struct unroll;
    
                protected:
                    // Note - the buffer type is required to exceed the size of the value type here.
                    // If the buffer size is equal to the value type the carries may not be deferred.
                    using buffer_type = 
                        typename std::conditional <
                            (ByteCount <= 1),
                            uint_fast16_t,
                            typename std::conditional <
                                (ByteCount <= 3),
                                uint_fast32_t,
                                typename std::enable_if <
                                    (ByteCount <= 7),
                                    uint_fast64_t
                                >::type
                            >::type
                        >::type;
    
                    using value_type = 
                        typename std::conditional <
                            ByteCount == 1,
                            uint8_t,
                            typename std::conditional <
                                ByteCount == 2,
                                uint16_t,
                                typename std::conditional <
                                    ByteCount == 3 || ByteCount == 4,
                                    uint32_t,
                                    typename std::conditional <
                                        (ByteCount > 4 && ByteCount <= 8),
                                        uint64_t,
                                        void
                                    >::type
                                >::type
                            >::type
                        >::type;
    
                    enum 
                    { 
                        byte_count = ByteCount,
                        value_mask = (~static_cast <buffer_type> (0)) >> (8*(sizeof(buffer_type)-ByteCount)),
                        value_shift = 8*ByteCount,
                        offset = ByteOrder == framework::byte_order::big_endian ? 8*(ByteCount-1) : 0,
                        multiplier = ByteOrder == framework::byte_order::big_endian ? -8 : 8,
                    };
    
                    template <std::size_t N>
                    struct handler_fixed
                    {
                        template <std::size_t I, typename T>
                        buffer_type operator() (T* this_ptr, char const* s)
                        {
                            enum { new_state = (I + N) % byte_count };
    
                            this_ptr->p_iState = new_state;
                            return unroll <I, N>::run(this_ptr, s);
                        }
    
                        template <typename T>
                        buffer_type operator() (T*, char const*)
                        {
                            assert(false);
                            return 0;
                        }
                    };

                public:
                    template <std::size_t N>
                    bool write (char const* s)
                    {
                        static_assert(sizeof(char) == 1, "Not implemented");
                        static_assert(ByteOrder == ::framework::byte_order::little_endian || 
                                      ByteOrder == ::framework::byte_order::big_endian, 
                                      "Not implemented");
    
                        p_iSum += variadic_switch_return <make_value_indices <byte_count>> (p_iState, handler_fixed <N> (), this, s);
                        p_iSum = (p_iSum & value_mask) + (p_iSum >> value_shift);
                        return true;
                    }
    
                    bool write (char const* s, std::size_t n)
                    {
                        static_assert(sizeof(char) == 1, "Not implemented");
                        static_assert(ByteOrder == ::framework::byte_order::little_endian || 
                                      ByteOrder == ::framework::byte_order::big_endian, 
                                      "Not implemented");

                        // TODO: This could be improved moderately by limiting the switch to the range [0, bye_count).
                        auto const count = std::min(std::size_t(byte_count - p_iState), n);
                        switch (count)
                        {
                            case 8: p_iSum += static_cast <buffer_type> (uint8_t(s[7])) << (offset + multiplier*((7 + p_iState) % byte_count));
                            case 7: p_iSum += static_cast <buffer_type> (uint8_t(s[6])) << (offset + multiplier*((6 + p_iState) % byte_count));
                            case 6: p_iSum += static_cast <buffer_type> (uint8_t(s[5])) << (offset + multiplier*((5 + p_iState) % byte_count));
                            case 5: p_iSum += static_cast <buffer_type> (uint8_t(s[4])) << (offset + multiplier*((4 + p_iState) % byte_count));
                            case 4: p_iSum += static_cast <buffer_type> (uint8_t(s[3])) << (offset + multiplier*((3 + p_iState) % byte_count));
                            case 3: p_iSum += static_cast <buffer_type> (uint8_t(s[2])) << (offset + multiplier*((2 + p_iState) % byte_count));
                            case 2: p_iSum += static_cast <buffer_type> (uint8_t(s[1])) << (offset + multiplier*((1 + p_iState) % byte_count));
                            case 1: p_iSum += static_cast <buffer_type> (uint8_t(s[0])) << (offset + multiplier*((0 + p_iState) % byte_count));
                                    p_iSum = (p_iSum & value_mask) + (p_iSum >> value_shift);
                                    p_iState = (p_iState + count) % byte_count;
                                    n -= count;
                                    s += count;
                            case 0: break;
                        }

                        while (true)
                        {
                            switch (n)
                            {
                                default:
                                case 8: p_iSum += static_cast <buffer_type> (uint8_t(s[7])) << (offset + multiplier*(7 % byte_count));
                                case 7: p_iSum += static_cast <buffer_type> (uint8_t(s[6])) << (offset + multiplier*(6 % byte_count));
                                case 6: p_iSum += static_cast <buffer_type> (uint8_t(s[5])) << (offset + multiplier*(5 % byte_count));
                                case 5: p_iSum += static_cast <buffer_type> (uint8_t(s[4])) << (offset + multiplier*(4 % byte_count));
                                case 4: p_iSum += static_cast <buffer_type> (uint8_t(s[3])) << (offset + multiplier*(3 % byte_count));
                                case 3: p_iSum += static_cast <buffer_type> (uint8_t(s[2])) << (offset + multiplier*(2 % byte_count));
                                case 2: p_iSum += static_cast <buffer_type> (uint8_t(s[1])) << (offset + multiplier*(1 % byte_count));
                                case 1: p_iSum += static_cast <buffer_type> (uint8_t(s[0])) << offset;
                                        p_iSum = (p_iSum & value_mask) + (p_iSum >> value_shift);
                                        break;

                                case 0: return true;
                            }
    
                            if (n <= 8)
                            {
                                p_iState = (p_iState + n) % byte_count;
                                return true;
                            }
                        
                            s += 8;
                            n -= 8;
                        }
                    };
    
                    value_type get () const
                    {
                        buffer_type result = p_iSum;
                        while (result >> value_shift)
                            result = (result & value_mask) + (result >> value_shift);
    
                        return ~result;
                    }
    
                protected:
                    buffer_type p_iSum {0};
                    unsigned int p_iState {0};
            };
        }

        /**
        * \brief Modular sum writer.
        *
        * Computes the 8*byte_count bit one's complement sum of the bytes received.  Support is provided
        * for big endian and little endian byte sequences.
        *
        * \tparam byte_count number of bytes to use in the sum
        * \tparam ByteOrder assumed input byte sequence endianness
        */
        template <std::size_t ByteCount = 2, framework::byte_order ByteOrder = framework::byte_order::big_endian>
        class modular_sum : private detail::modular_sum_impl <ByteCount, ByteOrder>
        {
            public:
                using detail::modular_sum_impl <ByteCount, ByteOrder>::write;
                using detail::modular_sum_impl <ByteCount, ByteOrder>::get;

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
                * \tparam N number of characters in the input stream
                */
                //template <std::size_t N>
                //bool write (char const* s)

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
                //bool write (char const* s, std::size_t n)

                /**
                * \brief Get checksum
                *
                * Gets the checksum value, defined as the complement of the 16 bit one's complement
                * sum of the input data.
                */
                //value_type get () const
        };
        
        template <framework::byte_order ByteOrder>
        class modular_sum <2, ByteOrder> : private detail::modular_sum_impl <2, ByteOrder>
        {
            private:
                using base = detail::modular_sum_impl <2, ByteOrder>;
                using buffer_type = typename base::buffer_type;

            public:
                using base::write;
                using base::get;

#ifdef FRAMEWORK_MODULAR_SUM_UNSAFE
    #if FRAMEWORK_MODULAR_SUM_UNSAFE == 0
        // Worst case this optimization will overflow the deferred carry buffer after summing on the order of 512TB worth of data
        #define FRAMEWORK_MODULAR_SUM_FOLD static_assert(sizeof(buffer_type) == 8, "failed")
    #elif FRAMEWORK_MODULAR_SUM_UNSAFE == 1
        // This optimization is reasonably safe, provided modular_sum_impl is conservative in it's overflow estimate
        #define FRAMEWORK_MODULAR_SUM_FOLD this->p_iSum = (this->p_iSum & base::value_mask) + (this->p_iSum >> base::value_shift)
    #elif FRAMEWORK_MODULAR_SUM_UNSAFE == 2
        // Worst case this optimization will overflow the deferred carry buffer after summing on the order of 8GB worth of data
        #define FRAMEWORK_MODULAR_SUM_FOLD static_assert(sizeof(buffer_type) == 8, "failed")
    #else
        #error "Unknown optimization type"
    #endif
#else
    #define FRAMEWORK_MODULAR_SUM_FOLD this->p_iSum = (this->p_iSum & base::value_mask) + (this->p_iSum >> base::value_shift)
#endif

                // An explicit override must be provided here
                template <typename T>
                bool write (T const& value)
                {
                    return base::template write <sizeof(T)> (reinterpret_cast <char const*> (&value));
                }

                bool write (uint8_t value)
                {
                    auto const offset = base::offset + base::multiplier*this->p_iState;
                    this->p_iState = (this->p_iState + 1) % 2;
                    this->p_iSum += static_cast <buffer_type> (value) << offset;
                    FRAMEWORK_MODULAR_SUM_FOLD;
                    return true;
                }

                bool write (uint16_t value)
                {
                    auto const offset = base::offset + base::multiplier*this->p_iState;
                    this->p_iSum += static_cast <buffer_type> (value) << offset;
                    FRAMEWORK_MODULAR_SUM_FOLD;
                    return true;
                }

                bool write (uint32_t value)
                {
                    auto const offset = base::offset + base::multiplier*this->p_iState;

#if FRAMEWORK_MODULAR_SUM_UNSAFE >= 1
                    this->p_iSum += static_cast <buffer_type> (value) << offset;
#else
                    auto const tmp = (value >> 16) + (value & 0xFFFF);
                    this->p_iSum += static_cast <buffer_type> (tmp) << offset;
#endif

                    FRAMEWORK_MODULAR_SUM_FOLD;
                    return true;
                }
                
                bool write (uint64_t value)
                {
                    auto const offset = base::offset + base::multiplier*this->p_iState;
                    auto const tmp1 = (value >> 32) + (value & 0xFFFFFFFF);

#if FRAMEWORK_MODULAR_SUM_UNSAFE >= 1
                    this->p_iSum += static_cast <buffer_type> (tmp1) << offset;
#else
                    auto const tmp2 = (tmp1 >> 16) + (tmp1 & 0xFFFF);
                    this->p_iSum += static_cast <buffer_type> (tmp2) << offset;
#endif

                    FRAMEWORK_MODULAR_SUM_FOLD;
                    return true;
                }
        };

        /**
        * \brief Internet checksum alias.
        */
        using internet_checksum = modular_sum <2, ::framework::byte_order::big_endian>;
    }
}

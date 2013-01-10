// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

#include <string.h>
#include <cstdint>
#include <cassert>

#include <framework/common/endian.hpp>
#include <framework/common/variadic_switch_fallthrough.hpp>
#include <framework/common/variadic_switch_return.hpp>

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

            // Selected based on observed modular_sum_overhead_large performance
            enum{ limit_unroll = 32 };

            // A higher limit could be selected here depending on the number of buffer bits, however the cost
            // of folding the buffer once over a large number of loops (32) was observed to be negligible in
            // modular_sum_overhead_large.  A limit of up to 254 can reasonably be used here regardless of the
            // number of input bytes.
            enum{ limit_loop = 254 };

            template <unsigned int State, std::size_t N>
            struct unroll <State, N, 0, typename std::enable_if <(N > limit_unroll) && (N <= limit_loop), void>::type>
            {
                template <typename T>
                static typename T::buffer_type run (T* this_ptr, char const* s)
                {
                    enum { loop_count = N / limit_unroll };
                    enum { remainder = N % limit_unroll };

                    typename T::buffer_type result {0};
                    for (std::size_t i=0; i < loop_count; ++i, s += limit_unroll)
                        result += unroll <State, limit_unroll>::run(this_ptr, s);

                    return result + unroll <State, remainder>::run(this_ptr, s);
                }
            };

            template <unsigned int State, std::size_t N>
            struct unroll <State, N, 0, typename std::enable_if <(N > limit_loop), void>::type>
            {
                template <typename T>
                static typename T::buffer_type run (T* this_ptr, char const* s)
                {
                    enum { loop_count = N / limit_loop };
                    enum { remainder = N % limit_loop };

                    typename T::buffer_type result {0};
                    for (std::size_t i=0; i < loop_count; ++i, s += limit_loop)
                    {
                        result += unroll <State, limit_loop>::run(this_ptr, s);
                        result = (result & T::value_mask) + (result >> T::value_shift);
                    }

                    return result + unroll <State, remainder>::run(this_ptr, s);
                }
            };

            /**
            * \brief Generic modular sum implementation.
            * \note
            * The write methods used here leave p_iSum with at most a value of one remaining in the
            * overflow buffer.
            */
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
                            ByteCount <= 1,
                            uint8_t,
                            typename std::conditional <
                                ByteCount <= 2,
                                uint16_t,
                                typename std::conditional <
                                    ByteCount <= 4,
                                    uint32_t,
                                    typename std::enable_if <
                                        ByteCount <= 8,
                                        uint64_t
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
                        // Process the first block - either clear the state or read in the required bytes
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

                        // Worst case the overflow buffer will support at least 254 operations between each fold
                        enum{ loop_1_size = 16*byte_count };
                        for (; n > loop_1_size; n-=loop_1_size, s+=loop_1_size)
                        {
                            p_iSum += unroll <0, loop_1_size>::run(this, s);
                            p_iSum = (p_iSum & value_mask) + (p_iSum >> value_shift);
                        }

                        // Process any remaining complete blocks
                        enum{ loop_2_size = byte_count };
                        for (; n > loop_2_size; n-=loop_2_size, s+=loop_2_size)
                        {
                            p_iSum += unroll <0, loop_2_size>::run(this, s);
                            p_iSum = (p_iSum & value_mask) + (p_iSum >> value_shift);
                        }

                        // Process the final block
                        switch (n)
                        {
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
                        }
    
                        p_iState = (p_iState + n) % byte_count;
                        return true;
                    };
    
                    value_type checksum () const
                    {
                        buffer_type result = p_iSum;
                        while (result >> value_shift)
                            result = (result & value_mask) + (result >> value_shift);
    
                        return ~result;
                    }

                    void reset ()
                    {
                        p_iSum = 0;
                        p_iState = 0;
                    }
    
                protected:
                    buffer_type p_iSum {0};
                    unsigned int p_iState {0};
            };
        }

        template <framework::byte_order ByteOrder>
        class modular_sum <2, ByteOrder> : private detail::modular_sum_impl <2, ByteOrder>
        {
            private:
                using base = detail::modular_sum_impl <2, ByteOrder>;
                using buffer_type = typename base::buffer_type;

                template <typename Type, std::size_t N = sizeof(Type)>
                Type create (char const* s)
                {
                    Type tmp {0};
                    memcpy(&tmp, s, N);
                    return tmp;
                }

            public:
                using base::write;
                using base::checksum;
                using base::reset;

#ifndef FRAMEWORK_MODULAR_SUM_SUPPRESS_OPTIMIZATIONS
    #if FRAMEWORK_HOST_ENDIANNESS == FRAMEWORK_LITTLE_ENDIAN
        #define FRAMEWORK_MODULAR_SUM_USE_OPTIMIZATIONS
                enum{ host_shift = 0 };
    #elif FRAMEWORK_HOST_ENDIANNESS == FRAMEWORK_BIG_ENDIAN
        #define FRAMEWORK_MODULAR_SUM_USE_OPTIMIZATIONS
                enum{ host_shift = 1 };
    #else
        #define FRAMEWORK_MODULAR_SUM_SUPPRESS_OPTIMIZATIONS
    #endif
#endif

#ifndef FRAMEWORK_MODULAR_SUM_SUPPRESS_OPTIMIZATIONS
    #ifdef FRAMEWORK_MODULAR_SUM_UNSAFE
        // Worst case this optimization will overflow the deferred carry buffer after summing on the order of 512TB worth of data
        #define FRAMEWORK_MODULAR_SUM_FOLD static_assert(sizeof(buffer_type) == 8, "This optimization requires sizeof(uint_fast16_t) == 8")
    #else
        #if defined(__GNUC__) && !defined(__clang__)
            #define FRAMEWORK_MODULAR_SUM_FOLD \
                if (static_cast <int> (this->p_iSum) < 0) \
                    this->p_iSum = (this->p_iSum & base::value_mask) + (this->p_iSum >> base::value_shift)
        #else
            #define FRAMEWORK_MODULAR_SUM_FOLD \
                this->p_iSum = (this->p_iSum & base::value_mask) + (this->p_iSum >> base::value_shift)
        #endif
    #endif

                template <std::size_t N>
                bool write (char const* s)
                {
                    switch (N)
                    {
                        case 0: return true;
                        case 1: return write(create <uint8_t> (s));
                        case 2: return write(create <uint16_t> (s));
                        case 3:
                        case 4: return write(create <uint32_t> (s));
                        case 5:
                        case 6:
                        case 7:
                        case 8: return write(create <uint64_t> (s));
                        default: return base::template write <N> (s);
                    }
                }

                bool write (uint8_t value)
                {
                    auto const offset = base::offset + base::multiplier*((this->p_iState + host_shift) % 2);
                    this->p_iState = (this->p_iState + 1) % 2;
                    this->p_iSum += static_cast <buffer_type> (value) << offset;
                    FRAMEWORK_MODULAR_SUM_FOLD;
                    return true;
                }

                bool write (uint16_t value)
                {
                    auto const offset = base::offset + base::multiplier*((this->p_iState + host_shift) % 2);
                    this->p_iSum += static_cast <buffer_type> (value) << offset;
                    FRAMEWORK_MODULAR_SUM_FOLD;
                    return true;
                }

                bool write (uint32_t value)
                {
                    auto const offset = base::offset + base::multiplier*((this->p_iState + host_shift) % 2);
                    auto const tmp = (value >> 16) + (value & 0xFFFF);
                    this->p_iSum += static_cast <buffer_type> (tmp) << offset;
                    FRAMEWORK_MODULAR_SUM_FOLD;
                    return true;
                }
                
                bool write (uint64_t value)
                {
                    auto const offset = base::offset + base::multiplier*((this->p_iState + host_shift) % 2);
                    auto const tmp1 = (value >> 32) + (value & 0xFFFFFFFF);
                    auto const tmp2 = (tmp1 >> 16) + (tmp1 & 0xFFFF);
                    this->p_iSum += static_cast <buffer_type> (tmp2) << offset;
                    FRAMEWORK_MODULAR_SUM_FOLD;
                    return true;
                }
#endif
        };
    }
}

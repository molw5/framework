// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

namespace framework
{
    namespace serializable
    {
        namespace detail
        {
            FRAMEWORK_ALWAYS_INLINE
            uint64_t fold_8 (char const* s)
            {
                return static_cast <uint8_t> (*s);
            }

            FRAMEWORK_ALWAYS_INLINE
            uint64_t fold_16 (char const* s)
            {
                uint16_t x;
                memcpy(&x, s, sizeof(x));
                return x;
            }

            FRAMEWORK_ALWAYS_INLINE
            uint64_t fold_32 (char const* s)
            {
                uint32_t x;
                memcpy(&x, s, sizeof(x));
                return (x & 0xFFFF) + (x >> 16);
            }

            FRAMEWORK_ALWAYS_INLINE
            uint64_t fold_64 (char const* s)
            {
                uint64_t x;
                memcpy(&x, s, sizeof(x));
                x = (x & 0xFFFFFFFF) + (x >> 32);
                return (x & 0xFFFF) + (x >> 16);
            }
        }

        template <byte_order Order>
        class modular_sum <2, Order>
        {
            static_assert(
                Order == byte_order::little_endian || 
                Order == byte_order::big_endian,
                "Specified byte order not implemented");

            private:
                template <std::size_t N, bool State>
                FRAMEWORK_ALWAYS_INLINE
                bool write_impl (char const* s)
                {
                    enum{ shift = State ? 8 : 0 };
        
                    switch (N)
                    {
                        case 0: break;
        
                        case 8: add(detail::fold_64(s) << shift); break;
                        case 4: add(detail::fold_32(s) << shift); break;
                        case 2: add(detail::fold_16(s) << shift); break;
                        case 1: add(detail::fold_8(s) << shift); 
                                p_bState = !p_bState; break;
        
                        case 3: return write_impl <1, State> (s) && write_impl <2, State> (s+1);
                        case 5: return write_impl <1, State> (s) && write_impl <4, State> (s+1);
                        case 6: return write_impl <2, State> (s) && write_impl <4, State> (s+2);
                        case 7: return write_impl <1, State> (s) && write_impl <6, State> (s+1);
        
                        default: return write(s, N);
                    }

                    return true;
                }

                template <bool State>
                FRAMEWORK_ALWAYS_INLINE
                bool write_impl (char const* s, std::size_t n)
                {
                    for (; n >= 8; n -= 8, s += 8)
                        write_impl <8, State> (s);
        
                    switch (n)
                    {
                        case 1: return write_impl <1, State> (s);
                        case 2: return write_impl <2, State> (s);
                        case 3: return write_impl <3, State> (s);
                        case 4: return write_impl <4, State> (s);
                        case 5: return write_impl <5, State> (s);
                        case 6: return write_impl <6, State> (s);
                        case 7: return write_impl <7, State> (s);
                    }
        
                    return true;
                }

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
                FRAMEWORK_ALWAYS_INLINE
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
                    if (p_iSum >> 63)
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

        // Note: => should be read 'interpreted as', not 'logically implies' here, <=> still used for equivalence
        // Original:
        //  Even sequence:
        //   p_bState is true:  abcd => dcba => (d,c) + (b,a)
        //   p_bState is false: abcd => dcba => (c,d) + (a,b) <=> (0,d) + (c,b) + (a,0)
        //  Odd sequence:
        //   p_bState is true:  abc => cba => (0,c) + (b,a)
        //   p_bState is false: abc => cba => (c,0) + (a,b) <=> (c,b) + (a,0)
        // Optimized:
        //  Even sequence:
        //   p_bState is true:  abcd => (b,a) + (d,c)
        //   p_bState is false: abcd => (a,b) + (c,d)
        //  Odd sequence:
        //   p_bState is true:  abc => (0,a) + (b,c)
        //   p_bState is false: abc => (a,0) + (c,b)

#if FRAMEWORK_HOST_ENDIANNESS == FRAMEWORK_LITTLE_ENDIAN
        template <typename T>
        typename std::enable_if <
            std::is_integral <T>::value,
            bool
        >::type write_dispatch (
            big_endian <T>*,
            T const& in, internet_checksum& out)
        {
            return out.write <sizeof(T), (sizeof(T) % 2) == 0> (reinterpret_cast <char const*> (&in));
        }
#endif

#if FRAMEWORK_HOST_FLOAT_ENDIANNESS == FRAMEWORK_LITTLE_ENDIAN
        template <typename T>
        typename std::enable_if <
            std::is_floating_point <T>::value,
            bool
        >::type write_dispatch (
            big_endian <T>*,        
            T const& in, internet_checksum& out)
        {
            return out.write <sizeof(T), (sizeof(T) % 2) == 0> (reinterpret_cast <char const*> (&in));
        }
#endif
    }
}

// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

namespace framework
{
    namespace serializable
    {
        namespace detail
        {
            uint64_t fold_8 (char const* s)
            {
                return static_cast <uint8_t> (*s);
            }

            uint64_t fold_16 (char const* s)
            {
                uint16_t x;
                memcpy(&x, s, sizeof(x));
                return x;
            }

            uint64_t fold_32 (char const* s)
            {
                uint32_t x;
                memcpy(&x, s, sizeof(x));
                return (x & 0xFFFF) + (x >> 16);
            }

            uint64_t fold_64 (char const* s)
            {
                uint64_t x;
                memcpy(&x, s, sizeof(x));
                x = (x & 0xFFFFFFFF) + (x >> 32);
                return (x & 0xFFFF) + (x >> 16);
            }
        }

        template <byte_order Order>
        template <std::size_t N, bool State>
        bool modular_sum <2, Order>::write_impl (char const* s)
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

                case 3: return write_impl <1, State> (s) && write_impl <3, State> (s+1);
                case 5: return write_impl <1, State> (s) && write_impl <4, State> (s+1);
                case 6: return write_impl <2, State> (s) && write_impl <4, State> (s+2);
                case 7: return write_impl <1, State> (s) && write_impl <6, State> (s+1);

                default: return write(s, N);
            }

            return true;
        }

        template <byte_order Order>
        template <bool State>
        bool modular_sum <2, Order>::write_impl (char const* s, std::size_t n)
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

#if FRAMEWORK_HOST_ENDIANNESS == FRAMEWORK_LITTLE_ENDIAN
        // Little endian machines may skip the byte-swap here, provided we reverse the meaning of 
        // internet_checksum's state flag.  Note that these handlers apply only to raw big_endian
        // wrappers around integral types, nested specifications (ie: big_endian <big_endian <uint32_t>>)
        // are unaffected.
        template <typename T>
        FRAMEWORK_DISPATCH_TAG
        bool dispatch_write (T const& in, internet_checksum& out, big_endian <T>*,
            typename std::enable_if <
                std::is_integral <T>::value,
                void
            >::type* = nullptr)
        {
            return out.write <sizeof(T), true> (reinterpret_cast <char const*> (&in));
        }
#endif

#if FRAMEWORK_HOST_FLOAT_ENDIANNESS == FRAMEWORK_LITTLE_ENDIAN
        template <typename T>
        FRAMEWORK_DISPATCH_TAG
        bool dispatch_write (T const& in, internet_checksum& out, big_endian <T>*,
            typename std::enable_if <
                std::is_floating_point <T>::value,
                void
            >::type* = nullptr)
        {
            return out.write <sizeof(T), true> (reinterpret_cast <char const*> (&in));
        }
#endif
    }
}

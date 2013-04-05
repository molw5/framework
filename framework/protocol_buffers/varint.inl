// Copyright (C) 2013 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Varint mutator.
*
* \copyright
* Copyright &copy; 2013 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

namespace framework
{
    namespace protocol_buffers
    {
        namespace detail
        {
            template <std::size_t Size, std::size_t Index = 0>
            struct varint_unroll
            {
                FRAMEWORK_ALWAYS_INLINE
                static bool read (char const* begin, char const* end, char const*& it, uint64_t& out)
                {
                    enum{ shift = 7*Index };

                    if (FRAMEWORK_EXPECT_FALSE(begin == end))
                        return false;

                    auto const block = static_cast <uint8_t> (*(begin++));
                    if (FRAMEWORK_EXPECT_TRUE(!(block & 0x80)))
                    {
                        out |= uint64_t(block) << shift;
                        it = begin;
                        return true;
                    }
                    
                    out |= uint64_t(block & 0x7F) << shift;
                    return varint_unroll <Size, Index+1>::read(begin, end, it, out);
                }

                template <typename Input>
                FRAMEWORK_ALWAYS_INLINE
                static bool read (Input& in, uint64_t& out)
                {
                    enum{ shift = 7*Index };

                    uint8_t block {};
                    if (FRAMEWORK_EXPECT_FALSE(!serializable::stream_read(in, &block, 1)))
                        return false;

                    if (FRAMEWORK_EXPECT_TRUE(!(block & 0x80)))
                    {
                        out |= uint64_t(block) << shift;
                        return true;
                    }

                    out |= uint64_t(block & 0x7F) << shift;
                    return varint_unroll <Size, Index+1>::read(in, out);
                }

                FRAMEWORK_ALWAYS_INLINE
                static bool write (uint64_t in, char* begin, char* end, char*& it)
                {
                    assert(begin != end);

                    auto const block = static_cast <uint8_t> (in);
                    if (FRAMEWORK_EXPECT_TRUE(!(in >>= 7)))
                    {
                        *(begin++) = static_cast <char> (block);
                        it = begin;
                        return true;
                    }

                    *(begin++) = static_cast <char> (block | 0x80);
                    return varint_unroll <Size, Index+1>::write(in, begin, end, it);
                }

                template <typename Output>
                FRAMEWORK_ALWAYS_INLINE
                static bool write (uint64_t in, Output& out)
                {
                    auto block = static_cast <uint8_t> (in);
                    if (FRAMEWORK_EXPECT_TRUE(!(in >>= 7)))
                    {
                        if (FRAMEWORK_EXPECT_FALSE(!serializable::stream_write(out, &block, 1)))
                            return false;

                        return true;
                    }
                    
                    block |= 0x80;
                    if (FRAMEWORK_EXPECT_FALSE(!serializable::stream_write(out, &block, 1)))
                        return false;

                    return varint_unroll <Size, Index+1>::write(in, out);
                }
            };

            template <std::size_t Size>
            struct varint_unroll <Size, Size>
            {
                FRAMEWORK_ALWAYS_INLINE
                static bool read (char const*, char const*, char const*&, uint64_t&)
                {
                    return false;
                }

                template <typename Input>
                FRAMEWORK_ALWAYS_INLINE
                static bool read (Input&, uint64_t&)
                {
                    return false;
                }

                FRAMEWORK_ALWAYS_INLINE
                static bool write (uint64_t, char*, char*, char*&)
                {
                    return false;
                }

                template <typename Output>
                FRAMEWORK_ALWAYS_INLINE
                static bool write (uint64_t, Output&)
                {
                    return false;
                }
            };

            template <std::size_t Size, typename Indices = make_indices <Size - 1>>
            struct fixed_length_impl;
            
            template <std::size_t Size, typename... Indices>
            struct fixed_length_impl <Size, pack_container <Indices...>>
            {
                template <typename Output>
                FRAMEWORK_ALWAYS_INLINE
                static bool run (std::size_t value, Output&& out)
                {
                    uint8_t const raw[Size] = {
                        static_cast <uint8_t> ((value >> (Indices::value*7)) | 0x80)...,
                        static_cast <uint8_t> ((value >> ((1+Size)*7)) & 0x7F)};

                    return serializable::stream_write <Size> (out, reinterpret_cast <char const*> (&raw[0]));
                }

                FRAMEWORK_ALWAYS_INLINE
                static bool run (std::size_t value, char* begin, char* end, char*& it)
                {
                    uint8_t const raw[Size] = {
                        static_cast <uint8_t> ((value >> (Indices::value*7)) | 0x80)...,
                        static_cast <uint8_t> ((value >> ((1+Size)*7)) & 0x7F)};

                    assert(static_cast <std::size_t> (end - begin) >= Size);
                    memcpy(begin, &raw[0], Size);
                    it = begin + Size;
                    return true;
                }
            };
        }

        template <std::size_t Size, typename Output>
        bool fixed_length (std::size_t value, Output&& out)
        {
            static_assert(Size > 0 && Size <= 10, "Invalid fixed varint size");
            enum{ max_size = (2ull << (7*Size)) - 1 };

            if (FRAMEWORK_EXPECT_FALSE(value > max_size))
                return false;

            return detail::fixed_length_impl <Size>::run(value, std::forward <Output> (out));
        }

        template <std::size_t Size>
        FRAMEWORK_ALWAYS_INLINE
        bool fixed_length (std::size_t value, char* begin, char* end, char*& it)
        {
            static_assert(Size > 0 && Size <= 10, "Invalid fixed varint size");
            enum{ max_size = (2ull << (7*Size)) - 1 };

            if (FRAMEWORK_EXPECT_FALSE(value > max_size))
                return false;

            return detail::fixed_length_impl <Size>::run(value, begin, end, it);
        }

        //--------------------------
        // Stream dispatch functions
        //--------------------------

        template <
            typename T,
            typename Input,
            typename Output>
        static bool read_dispatch (
            varint <T>*,
            Input&& in,
            Output&& out)
        {
            using serializable::type_extractor;
            enum{ max_size = std::is_signed <T>::value ? 10 : (8*sizeof(type_extractor <T>) + 6) / 7 };

            uint64_t x {};
            if (FRAMEWORK_EXPECT_FALSE(!detail::varint_unroll <max_size>::read(in, x)))
                return false;

            out = static_cast <type_extractor <T>> (x);
            return true;
        }        

        template <
            typename T,
            typename Input,
            typename Output>
        static typename std::enable_if <
            !std::is_same <Output, max_size_frame>::value,
            bool
        >::type write_dispatch (
            varint <T>*,
            Input&& in,
            Output&& out)
        {
            using serializable::type_extractor;
            enum{ max_size = std::is_signed <T>::value ? 10 : (8*sizeof(type_extractor <T>) + 6) / 7 };

            auto const x = static_cast <uint64_t> (in);
            return detail::varint_unroll <max_size>::write(x, out);
        }
        
        template <
            typename T,
            typename Input>
        FRAMEWORK_ALWAYS_INLINE
        bool write_dispatch (
            varint <T>*,
            Input&&,
            max_size_frame& out)
        {
            out.skip(10);
            return true;
        }

        template <
            typename T,
            typename Input,
            typename Output>
        static bool read_dispatch (
            zig_zag <T>*,
            Input&& in,
            Output&& out)
        {
            using serializable::type_extractor;
            enum{ max_size = (8*sizeof(type_extractor <T>) + 1 + 6) / 7 };

            uint64_t x{};
            if (FRAMEWORK_EXPECT_FALSE(!detail::varint_unroll <max_size>::read(in, x)))
                return false;

            out = x & 1 ? -static_cast <type_extractor <T>> (x >> 1) : static_cast <type_extractor <T>> (x >> 1);
            return true;
        }

        template <
            typename T,
            typename Input,
            typename Output>
        static bool write_dispatch (
            zig_zag <T>*,
            Input&& in,
            Output&& out)
        {
            using serializable::type_extractor;
            enum{ max_size = (8*sizeof(type_extractor <T>) + 1 + 6) / 7 };

            auto const x = in >= 0 ? (static_cast <uint64_t> (in) << 1) : ((static_cast <uint64_t> (-in) << 1) | 1);
            if (FRAMEWORK_EXPECT_FALSE(!detail::varint_unroll <max_size>::write(x, out)))
                return false;

            return true;
        }

        //--------------------------
        // Raw dispatch functions
        //--------------------------

        template <
            typename T,
            typename Output>
        FRAMEWORK_ALWAYS_INLINE
        static bool read_dispatch (
            varint <T>*,
            char const* begin, char const* end, char const*& it,
            Output&& out)
        {
            using serializable::type_extractor;
            enum{ max_size = std::is_signed <T>::value ? 10 : (8*sizeof(type_extractor <T>) + 6) / 7 };

            uint64_t x {};
            if (FRAMEWORK_EXPECT_FALSE(!detail::varint_unroll <max_size>::read(begin, end, it, x)))
                return false;

            out = static_cast <type_extractor <T>> (x);
            return true;
        }

        template <
            typename T,
            typename Input>
        FRAMEWORK_ALWAYS_INLINE
        static bool write_dispatch (
            varint <T>*,
            Input&& in,
            char* begin, char* end, char*& it)
        {
            using serializable::type_extractor;
            enum{ max_size = std::is_signed <T>::value ? 10 : (8*sizeof(type_extractor <T>) + 6) / 7 };

            auto const x = static_cast <uint64_t> (in);
            if (FRAMEWORK_EXPECT_FALSE(!detail::varint_unroll <max_size>::write(x, begin, end, begin)))
                return false;

            it = begin;
            return true;
        }

        template <
            typename T,
            typename Output>
        FRAMEWORK_ALWAYS_INLINE
        static bool read_dispatch (
            zig_zag <T>*,
            char const* begin, char const* end, char const*& it,
            Output&& out)
        {
            using serializable::type_extractor;
            enum{ max_size = (8*sizeof(type_extractor <T>) + 1 + 6) / 7 };

            uint64_t x{};
            if (FRAMEWORK_EXPECT_FALSE(!detail::varint_unroll <max_size>::read(begin, end, it, x)))
                return false;

            out = x & 1 ? -static_cast <type_extractor <T>> (x >> 1) : static_cast <type_extractor <T>> (x >> 1);
            return true;
        }

        template <
            typename T,
            typename Input>
        FRAMEWORK_ALWAYS_INLINE
        static bool write_dispatch (
            zig_zag <T>*,
            Input&& in,
            char* begin, char* end, char*& it)
        {
            using serializable::type_extractor;
            enum{ max_size = (8*sizeof(type_extractor <T>) + 1 + 6) / 7 };

            auto const x = in >= 0 ? (static_cast <uint64_t> (in) << 1) : ((static_cast <uint64_t> (-in) << 1) | 1);
            if (FRAMEWORK_EXPECT_FALSE(!detail::varint_unroll <max_size>::write(x, begin, end, it)))
                return false;

            return true;
        }
    }
}

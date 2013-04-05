namespace framework
{
    namespace serializable
    {
        // Fixed-size read handler

        template <typename Stream, typename Block>
        template <std::size_t N>
        struct bit_stream <Stream, Block>::read_handler_fixed
        {
            template <std::size_t I, typename Wrapper, typename Value>
            FRAMEWORK_ALWAYS_INLINE
            bool operator() (Wrapper& stream, Value& value)
            {
                return read_bits_impl <N, I>::read(stream, value);
            }

            template <typename Wrapper, typename Value>
            FRAMEWORK_ALWAYS_INLINE
            bool operator() (Wrapper&, Value&)
            {
                assert(false);
            }
        };

        // Default implementation - consumes min(N, Buffer) bits

        template <typename Stream, typename Block>
        template <std::size_t N, std::size_t Buffer, typename Enabler>
        struct bit_stream <Stream, Block>::read_bits_impl
        {
            template <typename Wrapper, typename Value>
            FRAMEWORK_ALWAYS_INLINE
            static bool read (Wrapper& stream, Value& value)
            {
                enum{ count = N <= Buffer ? N : Buffer };
                enum{ shift = Buffer - count };
                enum{ mask = Wrapper::block_mask >> (Wrapper::block_bits - count) };

                value = (value << count) | ((stream.p_iInputBlock >> shift) & mask);
                
                return read_bits_impl <N-count, Buffer-count>::read(stream, value);
            }
        };

        // Terminating cases, in order of precedence

        template <typename Stream, typename Block>
        template <std::size_t Buffer>
        struct bit_stream <Stream, Block>::read_bits_impl <0, Buffer, void>
        {
            template <typename Wrapper, typename Value>
            FRAMEWORK_ALWAYS_INLINE
            static bool read (Wrapper& stream, Value&)
            {
                stream.p_iInputBlockBits = Buffer;
                return true;
            }
        };

        template <typename Stream, typename Block>
        template <std::size_t N>
        struct bit_stream <Stream, Block>::read_bits_impl <N, 0, typename std::enable_if <(N > 0), void>::type>
        {
            template <typename Wrapper, typename Value>
            FRAMEWORK_ALWAYS_INLINE
            static bool read (Wrapper& stream, Value& value)
            {
                type_extractor <Block> block{};
                if (!dispatch_read <Block> (stream.get(), block))
                    return false;

                stream.p_iInputBlock = static_cast <typename Wrapper::buffer_type> (block);
                return read_bits_impl <N, Wrapper::block_bits>::read(stream, value);
            }
        };

        // Fixed-size write handler

        template <typename Stream, typename Block>
        template <std::size_t N>
        struct bit_stream <Stream, Block>::write_handler_fixed
        {
            template <std::size_t I, typename Wrapper, typename Value>
            FRAMEWORK_ALWAYS_INLINE
            bool operator() (Wrapper& stream, Value const& value)
            {
                return write_bits_impl <N, I>::write(stream, value);
            }

            template <typename Wrapper, typename Value>
            FRAMEWORK_ALWAYS_INLINE
            bool operator() (Wrapper&, Value const&)
            {
                assert(false);
            }
        };

        // Default implementation - consumes min(N, Buffer) bits

        template <typename Stream, typename Block>
        template <std::size_t N, std::size_t Buffer, typename Enabler>
        struct bit_stream <Stream, Block>::write_bits_impl
        {
            template <typename Wrapper, typename Value>
            FRAMEWORK_ALWAYS_INLINE
            static bool write (Wrapper& stream, Value const& value)
            {
                enum{ count = N <= Buffer ? N : Buffer };
                enum{ shift = N - count };
                enum{ position = Buffer - count };
                enum{ mask = Wrapper::block_mask >> (Wrapper::block_bits - count) };

                stream.p_iOutputBlock |= (static_cast <typename Wrapper::buffer_type> (value >> shift) & mask) << position;

                return write_bits_impl <N-count, Buffer-count>::write(stream, value);
            }
        };

        // Terminating cases, in order of precedence

        template <typename Stream, typename Block>
        template <std::size_t Buffer>
        struct bit_stream <Stream, Block>::write_bits_impl <0, Buffer, void>
        {
            template <typename Wrapper, typename Value>
            FRAMEWORK_ALWAYS_INLINE
            static bool write (Wrapper& stream, Value const&)
            {
                stream.p_iOutputBlockBits = Buffer;
                return true;
            }
        };

        template <typename Stream, typename Block>
        template <std::size_t N>
        struct bit_stream <Stream, Block>::write_bits_impl <N, 0, typename std::enable_if <(N > 0), void>::type>
        {
            template <typename Wrapper, typename Value>
            FRAMEWORK_ALWAYS_INLINE
            static bool write (Wrapper& stream, Value const& value)
            {
                auto const block = static_cast <type_extractor <Block>> (stream.p_iOutputBlock);
                if (!dispatch_write <Block> (block, stream.get()))
                    return false;

                stream.p_iOutputBlock = 0;
                return write_bits_impl <N, Wrapper::block_bits>::write(stream, value);
            }
        };

        // Dynamic-size read handler

        template <typename Stream, typename Block>
        struct bit_stream <Stream, Block>::read_handler_dynamic
        {
            template <std::size_t I>
            struct final_block
            {
                template <std::size_t N, typename Wrapper, typename Value>
                FRAMEWORK_ALWAYS_INLINE
                bool operator() (Wrapper& stream, Value& value)
                {
                    return read_bits_impl <N, I>::read(stream, value);
                }

                template <typename Wrapper, typename Value>
                FRAMEWORK_ALWAYS_INLINE
                bool operator() (Wrapper&, Value&)
                {
                    assert(false);
                }
            };

            template <std::size_t I, typename Wrapper, typename Value>
            FRAMEWORK_ALWAYS_INLINE
            bool operator() (Wrapper& stream, Value& value, std::size_t n)
            {
                using case_list = typename Wrapper::case_list;
                enum{ block_bits = Wrapper::block_bits };

                if (n <= I)
                    return variadic_switch_return <make_values <std::size_t, I+1>> (n, final_block <I> {}, stream, value);

                if (!write_bits_impl <I, I>::write(stream, value))
                    return false;

                assert(stream.p_iInputBlockBits == 0);

                for (n -= I; n >= block_bits; n -= block_bits)
                {
                    if (!read_bits_impl <Wrapper::block_bits, 0>::read(stream, value))
                        return false;

                    assert(stream.p_iInputBlockBits == 0);
                }

                return variadic_switch_return <case_list> (n, final_block <0> {}, stream, value);
            }

            template <typename Wrapper, typename Value>
            FRAMEWORK_ALWAYS_INLINE
            bool operator() (Wrapper&, Value&, std::size_t)
            {
                assert(false);
            }
        };

        // Dynamic-size write handler

        template <typename Stream, typename Block>
        struct bit_stream <Stream, Block>::write_handler_dynamic
        {
            template <std::size_t I>
            struct final_block
            {
                template <std::size_t N, typename Wrapper, typename Value>
                FRAMEWORK_ALWAYS_INLINE
                bool operator() (Wrapper& stream, Value const& value)
                {
                    return write_bits_impl <N, I>::write(stream, value);
                }

                template <typename Wrapper, typename Value>
                FRAMEWORK_ALWAYS_INLINE
                bool operator() (Wrapper&, Value&)
                {
                    assert(false);
                }
            };

            template <std::size_t I, typename Wrapper, typename Value>
            FRAMEWORK_ALWAYS_INLINE
            bool operator() (Wrapper& stream, Value const& value, std::size_t n)
            {
                using case_list = typename Wrapper::case_list;
                enum{ block_bits = Wrapper::block_bits };

                if (n <= I)
                    return variadic_switch_return <make_values <std::size_t, I+1>> (n, final_block <I> {}, stream, value);

                if (!write_bits_impl <I, I>::write(stream, value >> (n -= I)))
                    return false;

                assert(stream.p_iOutputBlockBits == 0);

                while (n >= Wrapper::block_bits)
                {
                    if (!write_bits_impl <block_bits, 0>::write(stream, value >> (n -= block_bits)))
                        return false;

                    assert(stream.p_iOutputBlockBits == 0);
                }

                return variadic_switch_return <case_list> (n, final_block <0> {}, stream, value);
            }

            template <typename Wrapper, typename Value>
            FRAMEWORK_ALWAYS_INLINE
            bool operator() (Wrapper&, Value const&, std::size_t)
            {
                assert(false);
            }
        };
    }
}

// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Bit field wrapper.
*
* \copyright
* Copyright &copy; 2012 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

#include <cstdint>
#include <cassert>
#include <type_traits>
#include <climits>

#include <framework/common/common_macros.hpp>
#include <framework/common/variadic_switch_return.hpp>
#include <framework/common/endian.hpp>

namespace framework
{
    namespace serializable
    {
        /**
        * \brief Bit stream.
        *
        * Provides bit-level access to the underlying stream.  Blocks of data are serialized according to
        * the provided \c Block specification - each block's bits are consumed from the most to the least
        * significant bit.  This wrapper provides traditional byte-level read/write methods with each
        * byte consuming the next \c CHAR_BIT bits in the stream, and type-sensitive read_bits/write_bits
        * methods which read a fixed number of bits into the provided container.
        *
        * Note that the interpretation of a byte-level method is dependent on the definition of the \c Block
        * type - streams should generally use big endian byte ordering here to avoid altering the implicit
        * definition of underlying types.  For example, consider the following field definitions:
        *
        * \code
        * using field = value <NAME("Field 1"), big_endian <uint32_t>>;
        * using ex1 = alias <bit_field <big_endian <uint16_t>, field>>;
        * using ex2 = alias <bit_field <little_endian <uint16_t>, field>>;
        * \endcode
        *
        * In the first example, \c ex1, the definition of \c field is unaltered - a byte-sequence (a,b,c,d) is
        * read into Field 1 as 0xabcd, as expected.  In the second example, on the other hand, a byte sequence
        * (a,b,c,d) is effectively interpreted as 0xbadc - that is, using PDP-11 byte ordering.
        *
        * \warning
        * The final operation following a series of writes to a bit stream should be a call to flush to ensure 
        * any remaining bits in the buffer are written to the underlying stream.  Any unused bits in the data
        * block are set to zero.
        *
        * \tparam Stream stream type
        * \tparam Block block type
        */
        template <typename Stream, typename Block>
        class bit_stream : private std::reference_wrapper <Stream>
        {
            private:
                using buffer_type = typename std::make_unsigned <type_extractor <Block>>::type;
                enum{ block_bytes = sizeof(buffer_type) };
                enum{ block_bits = CHAR_BIT*block_bytes };
                enum{ block_mask = static_cast <buffer_type> (~static_cast <buffer_type> (0)) };
                enum{ byte_mask = static_cast <unsigned char> (~static_cast <unsigned char> (0)) };
                using case_list = make_values <std::size_t, block_bits + 1>;

                template <std::size_t N>
                struct read_handler_fixed;

                template <std::size_t N, std::size_t Buffer, typename Enabler = void>
                struct read_bits_impl;

                template <std::size_t>
                struct write_handler_fixed;

                template <std::size_t N, std::size_t Buffer, typename Enabler = void>
                struct write_bits_impl;
                
                struct read_handler_dynamic;
                struct write_handler_dynamic;

            public:
                /**
                * \brief Wrapper constructor.
                *
                * \warning
                * The lifetime of stream must exceed this class; behaviour when this condition is not met is undefined.
                */
                bit_stream (Stream& s)
                    : std::reference_wrapper <Stream> (s)
                {
                }

                /**
                * \brief Read bytes.
                */
                template <std::size_t N>
                bool read (char* s)
                {
                    for (std::size_t i=0; i < N; ++i, ++s)
                        if (!read_bits <8> (*s))
                            return false;

                    return true;
                }

                /**
                * \brief Read bytes.
                */
                bool read (char* s, std::size_t n)
                {
                    for (std::size_t i=0; i < n; ++i, ++s)
                        if (!read_bits <8> (*s))
                            return false;

                    return true;
                }

                /**
                * \brief Read bytes.
                */
                template <std::size_t N>
                bool write (char const* s)
                {
                    for (std::size_t i=0; i < N; ++i, ++s)
                        if (!write_bits <8> (*s))
                            return false;

                    return true;
                }

                /**
                * \brief Read bytes.
                */
                bool write (char const* s, std::size_t n)
                {
                    for (std::size_t i=0; i < n; ++i, ++s)
                        if (!write_bits <8> (*s))
                            return false;

                    return true;
                }

                /**
                * \brief Read bits.
                *
                * Reads a sequence of bits into the low-order N bits of the provided value.
                *
                * \param t output container
                * \tparam N number of bits
                * \return true on success, false on failure
                */
                template <std::size_t N, typename T>
                bool read_bits (T& t)
                {
                    auto& value = reinterpret_cast <typename std::make_unsigned <T>::type&> (t);
                    return variadic_switch_return <case_list> (read_handler_fixed <N> {}, p_iInputBlockBits, *this, value);
                }

                /**
                * \brief Read bits.
                *
                * Reads a sequence of bits into the low-order n bits of the provided value.
                *
                * \param t output container
                * \param n number of bits
                * \return true on success, false on failure
                */
                template <typename T>
                bool read_bits (T& t, std::size_t n)
                {
                    auto& value = reinterpret_cast <typename std::make_unsigned <T>::type&> (t);
                    return variadic_switch_return <case_list> (read_handler_dynamic{}, p_iInputBlockBits, *this, value, n);
                }

                /**
                * \brief Write bits.
                *
                * Writes the low-order N bits of the provided value to the bit stream.
                *
                * \param t input container
                * \tparam N number of bits
                * \return true on success, false on failure
                */
                template <std::size_t N, typename T>
                bool write_bits (T const& t)
                {
                    auto const& value = reinterpret_cast <typename std::make_unsigned <T>::type const&> (t);
                    return variadic_switch_return <case_list> (write_handler_fixed <N> (), p_iOutputBlockBits, *this, value);
                }

                /**
                * \brief Write bits.
                *
                * Writes the low-order n bits of the provided value to the bit stream.
                *
                * \param t input container
                * \param n number of bits
                * \return true on success, false on failure
                */
                template <typename T>
                bool write_bits (T const& t, std::size_t n)
                {
                    auto const& value = reinterpret_cast <typename std::make_unsigned <T>::type const&> (t);
                    return variadic_switch_return <case_list> (write_handler_dynamic{}, p_iOutputBlockBits, *this, value, n);
                }

                /**
                * \brief Flush output stream.
                *
                * Writes any remaining bits in the output buffer to the underlying stream.
                *
                * \return true on success, false on failure
                */
                bool flush ()
                {
                    if (p_iOutputBlockBits < block_bits)
                    {
                        auto const value = static_cast <type_extractor <Block>> (p_iOutputBlock);
                        if (!dispatch_write <Block> (value, this->get()))
                            return false;

                        p_iOutputBlock = 0;
                        p_iOutputBlockBits = block_bits;
                    }

                    return true;
                }

            private:
                buffer_type p_iInputBlock {0};
                std::size_t p_iInputBlockBits {0};

                buffer_type p_iOutputBlock {0};
                std::size_t p_iOutputBlockBits {block_bits};
        };
    }
}

#include <framework/serializable/streams/bit_stream.inl>

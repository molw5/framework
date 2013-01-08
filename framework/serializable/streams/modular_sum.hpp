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

namespace framework
{
    namespace serializable
    {
        namespace detail
        {
            template <std::size_t ByteCount, framework::byte_order ByteOrder>
            class modular_sum_impl;
        }

        /**
        * \headerfile modular_sum.hpp <framework/serializable/streams/modular_sum.hpp>
        * \brief Modular sum writer.
        *
        * Computes the 8*ByteCount bit one's complement sum of the bytes written to this stream.
        *
        * \tparam byte_count number of bytes to use in the sum
        * \tparam ByteOrder input byte sequence endianness
        */
        template <std::size_t ByteCount = 2, framework::byte_order ByteOrder = framework::byte_order::big_endian>
        class modular_sum : private detail::modular_sum_impl <ByteCount, ByteOrder>
        {
            public:
                /**
                * \brief Write bytes.
                *
                * Writes a sequence of bytes to the checksum stream.
                */
                using detail::modular_sum_impl <ByteCount, ByteOrder>::write;

                /**
                * \brief Get checksum
                *
                * Gets the checksum, defined as the complement of the 16 bit one's complement sum of
                * the input data.
                */
                using detail::modular_sum_impl <ByteCount, ByteOrder>::get;
        };

        /**
        * \brief Internet checksum alias.
        */
        using internet_checksum = modular_sum <2, ::framework::byte_order::big_endian>;
    }
}

#include <framework/serializable/streams/modular_sum.inl>

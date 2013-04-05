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

#pragma once

#include <cassert>

#include <framework/serializable/mutator_type.hpp>
#include <framework/serializable/inline_object.hpp>
#include <framework/serializable/common_macros.hpp>
#include <framework/serializable/containers/value.hpp>
#include <framework/serializable/utility/common.hpp>
#include <framework/protocol_buffers/base_types.hpp>
#include <framework/protocol_buffers/frame.hpp>

namespace framework
{
    namespace protocol_buffers
    {
        /**
        * \headerfile varint.hpp <framework/protocol_buffers/varint.hpp>
        *
        * Serializes the child type T using the varint format specified in:
        *   https://developers.google.com/protocol-buffers/docs/encoding#varints
        *
        * \tparam T child type
        * \tparam Safe perform end-of-stream checks
        */
        template <typename T>
        struct varint : serializable::mutator_type <T> {};

        /**
        * \headerfile varint.hpp <framework/protocol_buffers/varint.hpp>
        *
        * Serializes the child type T using the zig-zag varint variant specified in:
        *   https://developers.google.com/protocol-buffers/docs/encoding#types
        *
        * \tparam T child type
        * \tparam Safe perform end-of-stream checks
        */
        template <typename T>
        struct zig_zag : serializable::mutator_type <T> {};

        /**
        * \headerfile varint.hpp <framework/protocol_buffers/varint.hpp>
        *
        * Writes a fixed-length varint value to an output stream.
        *
        * \param value input value
        * \param out output stream
        * \tparam Size varint size in bytes
        * \return true on success, false on failure
        */
        template <std::size_t Size, typename Output>
        bool fixed_length (std::size_t value, Output&& out);

        /**
        * \headerfile varint.hpp <framework/protocol_buffers/varint.hpp>
        *
        * Writes a fixed-length varint value to a raw character buffer.
        *
        * \param value input value
        * \param begin output buffer begin
        * \param end output buffer end
        * \param it final output iterator position
        * \tparam Size varint size in bytes
        * \return true on success, false on failure
        */
        template <std::size_t Size>
        FRAMEWORK_ALWAYS_INLINE
        bool fixed_length (std::size_t value, char* begin, char* end, char*& it);
    }
}

#include <framework/protocol_buffers/varint.inl>

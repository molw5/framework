// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Common endianness mutators.
*
* \copyright
* Copyright &copy; 2012 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

#include <framework/common/endian.hpp>
#include <framework/serializable/mutator_type.hpp>
#include <framework/serializable/base_types.hpp>

namespace framework
{
    namespace serializable
    {
        /**
        * \headerfile endian.hpp <framework/serializable/mutators/endian.hpp>
        *
        * Serializes the child type T using little endian byte ordering.  FRAMEWORK_HOST_ENDIANNESS
        * and FRAMEWORK_HOST_FLOAT_ENDIANNESS define the host endianness; see framework/common/endian.hpp.
        *
        * \tparam T child type
        */
        template <typename T>
        struct little_endian : public mutator_type <T> {};

        /**
        * \headerfile endian.hpp <framework/serializable/mutators/endian.hpp>
        *
        * Serializes the child type T using big endian byte ordering.  FRAMEWORK_HOST_ENDIANNESS
        * and FRAMEWORK_HOST_FLOAT_ENDIANNESS define the host endianness; see framework/common/endian.hpp.
        *
        * \tparam T child type
        */
        template <typename T>
        struct big_endian : public mutator_type <T> {};

        /**
        * \headerfile endian.hpp <framework/serializable/mutators/endian.hpp>
        *
        * Serializes the child type T using PDP-11 (middle-endian) byte ordering.  FRAMEWORK_HOST_ENDIANNESS
        * and FRAMEWORK_HOST_FLOAT_ENDIANNESS define the host endianness; see framework/common/endian.hpp.
        *
        * \tparam T child type
        */
        template <typename T>
        struct pdp_endian : public mutator_type <T> {};

        /**
        * \headerfile little_endian.inl <framework/serializable/mutators/endian.hpp>
        * \brief Read overload.
        *
        * Reads a value of type T from an input stream using little endian byte ordering.
        *
        * \param in input stream
        * \param out output value
        */
        template <
            typename T,
            typename Input, 
            typename Output>
        FRAMEWORK_ALWAYS_INLINE
        bool read_dispatch (
            little_endian <T>*,
            Input& in, Output& out)
        {
            type_extractor <T> value;
            if (!dispatch_read <T> (in, value))
                return false;

            out = little_to_host(value);
            return true;
        }

        /**
        * \headerfile little_endian.inl <framework/serializable/mutators/endian.hpp>
        * \brief Write overload.
        *
        * Writes a value of type T to an output stream using little endian byte ordering.
        *
        * \param in input value
        * \param out output stream
        */
        template <
            typename T,
            typename Input, 
            typename Output>
        FRAMEWORK_ALWAYS_INLINE
        bool write_dispatch (
            little_endian <T>*,
            Input const& in, Output& out)
        {
            return !!(dispatch_write <T> (host_to_little(in), out));
        }

        /**
        * \headerfile endian.hpp <framework/serializable/mutators/endian.hpp>
        * \brief Read overload.
        *
        * Reads a value of type T from an input stream using big endian byte ordering.
        *
        * \param in input stream
        * \param out output value
        */
        template <
            typename T,
            typename Input, 
            typename Output>
        bool read_dispatch (
            big_endian <T>*,
            Input& in, Output& out)
        {
            type_extractor <T> value;
            if (!dispatch_read <T> (in, value))
                return false;

            out = big_to_host(value);
            return true;
        }

        /**
        * \headerfile endian.hpp <framework/serializable/mutators/endian.hpp>
        * \brief Write overload.
        *
        * Writes a value of type T to an output stream using big endian byte ordering.
        *
        * \param in input value
        * \param out output stream
        */
        template <
            typename T,
            typename Input, 
            typename Output>
        bool write_dispatch (
            big_endian <T>*,
            Input const& in, Output& out)
        {
            return !!(dispatch_write <T> (host_to_big(in), out));
        }

        /**
        * \headerfile endian.hpp <framework/serializable/mutators/endian.hpp>
        * \brief Read overload.
        *
        * Reads a value of type T from an input stream using pdp endian byte ordering.
        *
        * \param in input stream
        * \param out output value
        */
        template <
            typename T,
            typename Input, 
            typename Output>
        bool read_dispatch (
            pdp_endian <T>*,
            Input& in, Output& out)
        {
            type_extractor <T> value;
            if (!dispatch_read <T> (in, value))
                return false;

            out = pdp_to_host(value);
            return true;
        }

        /**
        * \headerfile endian.hpp <framework/serializable/mutators/endian.hpp>
        * \brief Write overload.
        *
        * Writes a value of type T to an output stream using pdp endian byte ordering.
        *
        * \param in input value
        * \param out output stream
        */
        template <
            typename T,
            typename Input, 
            typename Output>
        bool write_dispatch (
            pdp_endian <T>*,
            Input const& in, Output& out)
        {
            return !!(dispatch_write <T> (host_to_pdp(in), out));
        }
    }
}

// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Host to big endian.
*
* \copyright
* Copyright &copy; 2012 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

#include <framework/serializable/mutator_type.hpp>
#include <framework/serializable/base_types.hpp>

namespace framework
{
    namespace serializable
    {
        namespace big_endian_impl
        {
            template <typename T, typename Enabler = void>
            struct to_host;

            template <typename T, typename Enabler = void>
            struct from_host;
        }

        /**
        * \headerfile big_endian.hpp <framework/serializable/mutators/big_endian.hpp>
        *
        * Serializes the child type T using big endian byte ordering.  FRAMEWORK_HOST_ENDIANNESS
        * and FRAMEWORK_HOST_FLOAT_ENDIANNESS define the host endianness; see framework/common/endian.hpp.
        *
        * \note
        * T is required to be a fundamental type as defined by \c std::is_fundamental 
        *
        * \tparam T child type
        */
        template <typename T>
        struct big_endian : public mutator_type <T>
        {
        };

        /**
        * \headerfile big_endian.inl <framework/serializable/mutators/big_endian.hpp>
        * \brief Read overload.
        *
        * Reads a value of type T from an input stream using big endian byte ordering.
        *
        * \param in input stream
        * \param out output value
        */
        template <
            typename Input, 
            typename Output, 
            typename T>
        bool dispatch_read (Input& in, Output& out, big_endian <T>*,
            typename std::enable_if <
                std::is_same <
                    type_extractor <T>,
                    Output
                >::value,
                void
            >* = nullptr)
        {
            using value_type = type_extractor <T>;

            type_extractor <T> value;
            if (!dispatch_read <T> (in, value))
                return false;

            out = big_endian_impl::to_host <decltype(value)>::run(value);
            return true;
        }

        /**
        * \headerfile big_endian.inl <framework/serializable/mutators/big_endian.hpp>
        * \brief Write overload.
        *
        * Writes a value of type T to an output stream using big endian byte ordering.
        *
        * \param in input value
        * \param out output stream
        */
        template <
            typename Input, 
            typename Output, 
            typename T>
        bool dispatch_write (Input const& in, Output& out, big_endian <T>*,
            typename std::enable_if <
                std::is_same <
                    type_extractor <T>,
                    Input
                >::value,
                void
            >* = nullptr)
        {
            if (!dispatch_write <T> (big_endian_impl::from_host <type_extractor <T>>::run(in), out))
                return false;

            return true;
        }
    }
}

#include <framework/serializable/mutators/big_endian.inl>

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
        namespace detail
        {
            template <typename T, typename Enabler = void>
            struct big_endian_impl;
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
        * \headerfile big_endian.hpp <framework/serializable/mutators/big_endian.hpp>
        * \brief \c big_endian serialization.
        */
        template <typename T>
        struct serializable_specification <big_endian <T>>
        {
            private:
                using value_type = typename type_extractor <T>::type;
                
            public:
                /**
                * \brief Reads the value.
                *
                * Reads a value of type T from the input stream using big endian 
                * byte ordering.
                *
                * \param in input stream
                * \param out output value
                */
                template <typename Input>
                static bool read (Input& in, value_type& out)
                {
                    value_type value;
                    if (!serializable_specification <T>::read(in, value))
                        return false;

                    detail::big_endian_impl <value_type>::to_host(value);
                    out = std::move(value);
                    return true;
                }

                /**
                * \brief Writes the value.
                *
                * Writes a value of type T to the output stream using big endian
                * byte ordering.
                *
                * \param in input stream
                * \param out output value
                */
                template <typename Output>
                static bool write (value_type const& in, Output& out)
                {
                    value_type value = in;
                    detail::big_endian_impl <value_type>::from_host(value);

                    if (!serializable_specification <T>::write(value, out))
                        return false;
                    
                    return true;
                }
        };
    }
}

#include <framework/serializable/mutators/big_endian.inl>

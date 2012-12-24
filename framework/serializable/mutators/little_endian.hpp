// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Host to little endian.
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
            struct little_endian_impl;
        }

        /**
        * \headerfile little_endian.hpp <framework/serializable/mutators/little_endian.hpp>
        *
        * Serializes the child type T using little endian byte ordering.  FRAMEWORK_HOST_ENDIANNESS
        * and FRAMEWORK_HOST_FLOAT_ENDIANNESS define the host endianness; see framework/common/endian.hpp.
        *
        * \note
        * T is required to be a fundamental type as defined by \c std::is_fundamental 
        *
        * \tparam T child type
        */
        template <typename T>
        struct little_endian : public mutator_type <T>
        {
        };

        /**
        * \headerfile little_endian.inl <framework/serializable/mutators/little_endian.hpp>
        * \brief \c little_endian serialization.
        */
        template <typename T>
        struct serializable_specification <little_endian <T>>
        {
            private:
                /**
                * \brief Underlying value type definition.
                */
                using value_type = typename type_extractor <T>::type;
                
            public:
                /**
                * Reads a value of type T from the input stream using little endian
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

                    detail::little_endian_impl <value_type>::to_host(value);
                    out = std::move(value);
                    return true;
                }

                /**
                * Writes a value of type T to the output stream using little endian 
                * byte ordering.
                *
                * \param in input stream
                * \param out output value
                */
                template <typename Output>
                static bool write (value_type const& in, Output& out)
                {
                    value_type value = in;
                    detail::little_endian_impl <value_type>::from_host(value);

                    if (!serializable_specification <T>::write(value, out))
                        return false;
                    
                    return true;
                }
        };
    }
}

#include <framework/serializable/mutators/little_endian.inl>

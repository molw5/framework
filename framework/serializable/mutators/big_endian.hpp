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
        * \brief \c big_endian serialization.
        */
        template <typename T>
        struct serializable_specification <big_endian <T>>
        {
            private:
                /**
                * \brief Underlying value type definition.
                */
                using value_type = typename type_extractor <T>::type;
                
            public:
                /**
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

                    out = big_endian_impl::to_host <value_type>::run(value);
                    return true;
                }

                /**
                * Writes a value of type T to the output stream using big endian 
                * byte ordering.
                *
                * \param in input stream
                * \param out output value
                */
                template <typename Output>
                static bool write (value_type const& in, Output& out)
                {
                    if (!serializable_specification <T>::write(big_endian_impl::from_host <value_type>::run(in), out))
                        return false;
                    
                    return true;
                }
        };
    }
}

#include <framework/serializable/mutators/big_endian.inl>

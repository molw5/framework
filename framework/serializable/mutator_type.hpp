// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Mutator base type.
*
* \copyright
* Copyright &copy; 2012 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

namespace framework
{
    namespace serializable
    {
        namespace detail
        {
            template <typename T, typename Enabler = void>
            struct type_extractor_impl;
            
            template <typename T, typename Enabler = void>
            struct is_mutator_type_impl;
        }

        /**
        * \headerfile mutator_type.hpp <framework/serializable/mutator_type.hpp>
        *
        * Provides a common interface used to define the types required to properly
        * interpret a mutator type.  Unlike \c container_type and \c value_type, this
        * base is not intended to interact with a derived object directly - rather,
        * a mutator type interacts with the underlying
        * \code
        * typename type_extractor <T>::type
        * \endcode
        * type.  For example, during the serialization of the following object
        * \code
        * class Object : serializable <Object
        *   value <NAME("Field 1"), little_endian <uint32_t>>,
        *   value <NAME("Field 2"), big_endian <float>>>
        * {
        * };
        * \endcode
        * \c serializable and \c value's readers both accept an output object of type
        * Object while \c little_endian and big_endian's readers accept an unsigned
        * integer and a float respectively.
        *
        * \note
        * No default serialization method is provided here - it is not clear what role
        * such a default would perform.
        *
        * \tparam Child wrapped child type
        */
        template <typename Child>
#ifndef DOXYGEN
        struct mutator_type;
#else
        struct mutator_type
        {
        };
#endif

        /**
        * \headerfile mutator_type.hpp <framework/serializable/mutator_type.hpp>
        *
        * Type trait testing whether or not T is a \c mutator_type.
        * Inherits std::true_type or std::false_type.
        */
        template <typename T>
        struct is_mutator_type :
            std::conditional <
                detail::is_mutator_type_impl <T>::value,
                std::true_type,
                std::false_type
            >::type
        {
        };

        /**
        * \headerfile mutator_type.hpp <framework/serializable/mutator_type.hpp>
        *
        * Retrieves a mutator's child type, storing the result in \c type.
        *
        * \pre is_mutator_type <T>::value == true
        */
        template <typename T>
        struct get_mutator_child
        {
            /**
            * \brief Result.
            */
            using type =
                typename std::enable_if <
                    is_mutator_type <T>::value,
                    typename T::mutator_child
                >::type;
        };

        /**
        * \headerfile mutator_type.hpp <framework/serializable/mutator_type.hpp>
        * \brief Mutator type extractor.
        *
        * Extracts the underlying type, defined recursively as
        * \code
        * typename type_extractor <typename get_mutator_child <T>::type>::type
        * \endcode
        * if \T is a mutator type, as defined by \c is_mutator_type, and \c T otherwise.
        */
        template <typename T>
        struct type_extractor
        {
            /**
            * \brief Result.
            */
            using type = typename detail::type_extractor_impl <T>::type;
        };
    }
}

#include <framework/serializable/mutator_type.inl>

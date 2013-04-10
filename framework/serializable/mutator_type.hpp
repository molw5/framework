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

#include <type_traits>

#include <framework/common/common_macros.hpp>

namespace framework
{
    namespace serializable
    {
        namespace detail
        {
            template <typename T, typename Enabler = void>
            struct is_mutator_type_impl;

            template <typename T>
            struct mutator_child_impl;

            template <typename T, typename Enabler = void>
            struct type_extractor_impl;
        }

        /**
        * \headerfile mutator_type.hpp <framework/serializable/mutator_type.hpp>
        *
        * Provides a common interface used to define the types required to properly
        * interpret a mutator type.  Unlike \c container_type and \c value_type, this
        * base is not intended to interact with a derived object directly - rather,
        * a mutator type interacts with the underlying
        *
        * \code
        * type_extractor <T>
        * \endcode
        *
        * type.  For example, during the deserialization of the following object
        *
        * \code
        * class Object : serializable <Object
        *   value <NAME("Field 1"), little_endian <uint32_t>>,
        *   value <NAME("Field 2"), big_endian <float>>>
        * {
        * };
        * \endcode
        *
        * the read methods associated with \c serializable and \c value both accept an output 
        * object of type \c Object while those associated with \c little_endian and big_endian's 
        * accept an unsigned integer and a float respectively.
        *
        * \note
        * No default serialization method is provided here - it is not clear what role
        * such a default would perform.
        *
        * \tparam Child wrapped child type
        */
        template <typename Child>
        struct mutator_type
        {
// GCC bug workaround - www.open-std.org/jtc1/sc22/wg21/docs/cwg_defects.html#580
#if MAX_GNUC_VERSION(4, 8, 0)
            public:
#else
            template <typename T, typename Enabler>
            friend struct detail::is_mutator_type_impl;
            
            template <typename T>
            friend struct detail::mutator_child_impl;

            private:
#endif
                using serializable_mutator_type_enabler = void;
                using serializable_mutator_child = Child;

                mutator_type () = delete;
                ~mutator_type () = delete;
        };

        /**
        * \headerfile container_type.hpp <framework/serializable/container_type.hpp>
        *
        * Type trait testing whether or not T is a \c container_type, equivalent to either
        * std::true_type or std::false_type.
        */
        template <typename T>
        using is_mutator_type = typename detail::is_mutator_type_impl <T>::type;

        /**
        * \headerfile mutator_type.hpp <framework/serializable/mutator_type.hpp>
        *
        * Retrieves a mutator's child type.
        *
        * \pre is_mutator_type \<T\>\::value == true
        */
        template <typename T>
        using mutator_child = typename detail::mutator_child_impl <T>::type;

        /**
        * \headerfile mutator_type.hpp <framework/serializable/mutator_type.hpp>
        * \brief Mutator type extractor.
        *
        * Extracts the underlying type, defined recursively as
        * \code
        * typename type_extractor <typename get_mutator_child <T>::type>::type
        * \endcode
        * if \c T is a mutator type, and \c T otherwise.
        */
        template <typename T>
        using type_extractor = typename detail::type_extractor_impl <T>::type;
    }
}

#include <framework/serializable/mutator_type.inl>

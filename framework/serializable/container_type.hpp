// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Container base type.
*
* \copyright
* Copyright &copy; 2012 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

#include <framework/serializable/base_types.hpp>

namespace framework
{
    namespace serializable
    {
        namespace detail
        {
            template <typename T>
            struct recursive_serializable_specification;
            
            template <typename T, typename Enabler = void>
            struct is_container_type_impl;

            template <typename T, typename Enabler = void>
            struct is_default_container_serializable_impl;
        }

        /**
        * \headerfile container_type.hpp <framework/serializable/container_type.hpp>
        *
        * Provides a common interface used to define the types required to properly
        * interpret a container type.  A container provides two lists of child types - 
        * \c Specification, or a formal list of child types, and \c Children or a logical
        * list of child types.  This division is useful in processing a specification -
        * implementation details may be hidden in \c Specification to remove them from
        * consideration when comparing two types, printing the fields present in an
        * object, and so on.  See the documentation in \c serializable for additional
        * information on how this division is used in practice in the construction of
        * an object.
        *
        * The default specification provided for this type processes each specification
        * in \c Specification's \c pack_container in the order it appears.  Derived types
        * may suppress this default by setting \c Default to false, forcing the
        * generation of a compile time error if an appropriate \c serializable_specification
        * specialization is not found.
        *
        * \tparam Specification pack_container specifying the formal children of the container
        * \tparam Children pack_container specifying the logical children of the container
        * \tparam Default flag enabling/disabling default serialization of the container
        */
        template <
            typename Specification,
            typename Children,
            bool Default = true>
#ifndef DOXYGEN
        struct container_type;
#else
        struct container_type
        {
        };
#endif
        
        /**
        * \headerfile container_type.hpp <framework/serializable/container_type.hpp>
        *
        * Type trait testing whether or not T is a default serializable \c container_type.
        * Derives from std::true_type or std::false_type.
        *
        * \note 
        * The following relationships always hold
        * \code
        * !is_default_container_serializable <T>::value || is_container_type <T>::value
        * !is_default_container_serializable <T>::value || is_default_serializable <T>::value
        * \endcode
        */
        template <typename T>
        struct is_default_container_serializable :
            std::conditional <
                detail::is_default_container_serializable_impl <T>::value,
                std::true_type,
                std::false_type
            >::type
        {
        };

        /**
        * \headerfile container_type.hpp <framework/container_type/container_type.hpp>
        *
        * Required specialization of is_default_serializable.
        */
        template <typename T>
        struct is_default_serializable <T,
            typename std::enable_if <
                is_default_container_serializable <T>::value,
                void
            >::type> : std::true_type
        {
        };
        
        /**
        * \headerfile container_type.hpp <framework/serializable/container_type.hpp>
        *
        * Type trait testing whether or not T is a \c container_type.
        * Inherits std::true_type or std::false_type.
        */
        template <typename T>
        struct is_container_type :
            std::conditional <
                detail::is_container_type_impl <T>::value,
                std::true_type,
                std::false_type
            >::type
        {
        };

        /**
        * \headerfile container_type.hpp <framework/serializable/container_type.hpp>
        * \brief Get specification.
        *
        * Retrieves a container type's specification, storing the result in \c type.
        *
        * \pre is_container_type <T>::value == true
        */
        template <typename T>
        struct get_container_specification
        {
            /**
            * \brief Result.
            */
            using type = 
                typename std::enable_if <
                    is_container_type <T>::value,
                    typename T::container_specification
                >::type;
        };

        /**
        * \headerfile container_type.hpp <framework/serializable/container_type.hpp>
        * \brief Get children.
        *
        * Retrieves a container type's children, storing the result in \c type.
        *
        * \pre is_container_type <T>::value == true
        */
        template <typename T>
        struct get_container_children
        {
            /**
            * \brief Result.
            */
            using type = 
                typename std::enable_if <
                    is_container_type <T>::value,
                    typename T::container_children
                >::type;
        };

        /**
        * \headerfile container_type.hpp <framework/serializable/container_type.hpp>
        * \brief Default \c container_type specification.
        *
        * Serializes each element of
        * \code
        * typename get_container_children <T>::type
        * \endcode
        * in the order it appears.
        * 
        * \note
        * The methods provided here access the implementation's method directly though
        * the reference provided by \c interface.
        */
        template <typename T>
        struct serializable_default_specification <T,
            typename std::enable_if <
                is_default_container_serializable <T>::value,
                void
            >::type>
        {
            /**
            * \param in input stream
            * \param out output container
            * \return true on success, false on failure
            */
            template <typename Input, typename Output>
            static bool read (Input& in, Output& out)
            {
                return detail::recursive_serializable_specification <
                    typename get_container_specification <T>::type>::read(in, out);
            }

            /**
            * \param in input container
            * \param out output stream
            * \return true on success, false on failure
            */
            template <typename Input, typename Output>
            static bool write (Input const& in, Output& out)
            {
                return detail::recursive_serializable_specification <
                    typename get_container_specification <T>::type>::write(in, out);
            }
        };
    }
}

#include <framework/serializable/container_type.inl>

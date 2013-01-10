// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Serializable interface header.
*
* \copyright
* Copyright &copy; 2012 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

#include <framework/serializable/container_type.hpp>
#include <framework/serializable/containers/alias.hpp>
#include <framework/serializable/base_types.hpp>

namespace framework
{
    namespace serializable
    {
        namespace detail
        {
            template <typename T, typename Enabler = void>
            struct is_object_type_impl;

            template <typename T, typename Enabler = void>
            struct is_object_default_serializable_impl;

            template <typename T, typename Name>
            struct object_base_impl;

            template <typename T>
            struct object_names_impl;

            template <typename T, template <typename> class Path, typename Enabler = void>
            struct extract_values_impl;
        }

        /**
        * \headerfile serializable.hpp <framework/serializable/serializable.hpp>
        * \brief Serializable implementation.
        *
        * Implements the serializable object defined by the provided parameters.  Object layout
        * is defined by \c Inherited - each value type in this container is instantiated as
        *
        * \code
        * typename get_value_implementation <T, Derived>::type
        * \endcode
        *
        * and publicly inherited in the order they appear.
        * 
        * \tparam Derived derived class
        * \tparam Specification used to serialize the object
        * \tparam Inherited \c pack_container of value types to inherit
        * \tparam Constructed \c pack_container of value types participating in the base object constructor
        * \tparam Visible value \c pack_container of value types exposed through \c get_visible_names
        */
        template <
            typename Derived,
            typename Specification,
            typename Inherited,
            typename Constructed,
            typename Visible,
            bool Default = true>
#ifndef DOXYGEN
        class serializable_implementation;
#else
        class serializable_implementation
        {
            protected:
                /**
                * \brief Default destructor.
                */
                ~serializable_implementation () = default;
                
                /**
                * \brief Default constructor.
                */
                serializable_implementation () = default;

                /**
                * \brief Base object constructor.
                *
                * Passes the argument list to the inherited types specified by \c Constructed
                * in the order they appear in the \c pack_container.
                */
                template <typename... Args>
                serializable_implementation (Args&&... args);
                
                /**
                * Convenience typedef provided to derived classes to refer to this base type.
                */
                typedef serializable_base;
        };
#endif

        /**
        * \headerfile serializable.hpp <framework/serializable/serializable.hpp>
        *
        * Type trait testing whether or not T derives from \c serializable_implementation,
        * equivalent to either std::true_type or std::false_type.
        */
        template <typename T>
        using is_object_type = typename detail::is_object_type_impl <T>::type;

        /**
        * \headerfile serializable.hpp <framework/serializable/serializable.hpp>
        *
        * Type trait testing whether or not T is a default serializable 
        * \c serializable_implementation, equivalent to either std::true_type or std::false_type.
        */
        template <typename T>
        using is_object_default_serializable = typename detail::is_object_default_serializable_impl <T>::type;

        /**
        * \headerfile serializable.hpp <framework/serializable/serializable.hpp>
        *
        * Gets the base type associated with \c Name in \c T, storing the result in \c type.
        *
        * \pre is_derived_type <T>::value == true
        */
        template <typename T, typename Name>
        using object_base = typename detail::object_base_impl <T, Name>::type;

        /**
        * \headerfile serializable.hpp <framework/serializable/serializable.hpp>
        *
        * Constructs a \c pack_container of every visible value name in \c T.
        *
        * \pre is_derived_type <T>::value == true
        */
        template <typename T>
        using object_names = typename detail::object_names_impl <T>::type;

        /**
        * \headerfile serializable.hpp <framework/serializable/serializable.hpp>
        * \brief Extracts value types.
        *
        * Generates a \c pack_container of all value types accessible from \c T, using
        * the \c Path template to define the child nodes of container types.  For example,
        * the following two result types are equivalent:
        *
        * \code
        * using result = 
        *     typename extract_values <
        *         alias <
        *             value <NAME("Field 1"), int>,
        *             alias <
        *                 value <NAME("Field 2"), float>,
        *                 value <NAME("Field 3"), double>>,
        *             value <NAME("Field 4"), short>>,
        *         get_container_children
        *     >::type;
        *
        * using result = pack_container <
        *     value <NAME("Field 1"), int>,
        *     value <NAME("Field 2"), float>,
        *     value <NAME("Field 3"), double>,
        *     value <NAME("Field 4"), short>>;
        * \endcode
        *
        * \tparam T container type
        * \tparam Path path to follow through the tree
        */
        template <typename T, template <typename> class Path>
        using extract_values = typename detail::extract_values_impl <T, Path>::type;

        /**
        * \headerfile serializable.hpp <framework/serializable/serializable.hpp>
        * \brief Implementation alias.
        *
        * Default serializable_implementation alias.
        *
        * \tparam Derived derived class
        * \tparam Specification child types
        */
        template <typename Derived, typename... Specification>
        using serializable = serializable_implementation <
            Derived, 
            alias <Specification...>,
            extract_values <alias <Specification...>, container_specification>,
            extract_values <alias <Specification...>, container_children>,
            extract_values <alias <Specification...>, container_children>>;
    }
}

#include <framework/serializable/serializable.inl>

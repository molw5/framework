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
            template <typename Pack, template <typename> class Path>
            struct extract_values_impl;
            
            template <typename T, typename Enabler = void>
            struct is_derived_type_impl;
        }

        /**
        * \headerfile serializable.hpp <framework/serializable/serializable.hpp>
        * \brief Serializable implementation.
        *
        * Implements the serializable object defined by the provided parameters.  Object layout
        * is defined by \c Inherited - each value type in this container is instantiated as
        * \code
        * typename get_value_implementation <T, Derived>::type
        * \endcode
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
            typename Visible>
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
        template <
            typename T,
            template <typename> class Path,
            typename Enabler = void>
#ifndef DOXYGEN
        struct extract_values;
#else
        struct extract_values
        {
            /**
            * \brief Result.
            */
            typedef type;
        };
#endif

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
            typename extract_values <alias <Specification...>, get_container_specification>::type,
            typename extract_values <alias <Specification...>, get_container_children>::type,
            typename extract_values <alias <Specification...>, get_container_children>::type>;

        /**
        * \headerfile serializable.hpp <framework/serializable/serializable.hpp>
        *
        * Type trait testing whether or not T derives from \c serializable_implementation.
        * Inherits std::true_type or std::false_type.
        */
        template <typename T>
        struct is_derived_type : 
            std::conditional <
                detail::is_derived_type_impl <T>::value,
                std::true_type,
                std::false_type
            >::type
        {
        };

        /**
        * \headerfile serializable.hpp <framework/serializable/serializable.hpp>
        *
        * Type trait testing whether or not T is a default serializable derived object type.
        * Derives from std::true_type or std::false_type.
        *
        * \note 
        * The following relationships always hold
        * \code
        * !is_default_derived_serializable <T>::value || is_derived_type <T>::value
        * !is_default_derived_serializable <T>::value || is_default_serializable <T>::value
        * \endcode
        */
        template <typename T>
        struct is_default_derived_serializable :
            std::conditional <
                is_derived_type <T>::value,
                std::true_type,
                std::false_type
            >::type
        {
        };

        /**
        * \headerfile serializable.hpp <framework/serializable/serializable.hpp>
        *
        * Required specialization of is_default_serializable.
        */
        template <typename T>
        struct is_default_serializable <T,
            typename std::enable_if <
                is_default_derived_serializable <T>::value,
                void
            >::type> : std::true_type
        {
        };
        
        /**
        * \headerfile serializable.hpp <framework/serializable/serializable.hpp>
        * \brief Gets base type.
        *
        * Gets the base type associated with \c Name in \c T, storing the result in \c type.
        *
        * \pre is_derived_type <T>::value == true
        */
        template <typename T, typename Name>
        struct get_base
        {
            private:
                template <typename U>
                using filter = std::is_same <typename U::first_type, Name>;

            public:
                /**
                * \brief Result.
                */
                using type =
                    typename std::enable_if <
                        is_derived_type <T>::value,
                        typename unique_filter_pack <
                            typename T::serializable_derived_base_map,
                            filter
                        >::type::second_type
                    >::type;
        };

        /**
        * \headerfile serializable.hpp <framework/serializable/serializable.hpp>
        * \brief Get visible names.
        *
        * Constructs a \c pack_container of every visible value name in \c T, storing the
        * result in \c type.
        *
        * \pre is_derived_type <T>::value == true
        */
        template <typename T>
        struct get_visible_names
        {
            /**
            * \brief Result.
            */
            using type =
                typename std::enable_if <
                    is_derived_type <T>::value,
                    typename T::serializable_derived_visible_names
                >::type;
        };

        /**
        * \headerfile serializable.hpp <framework/serializable/serializable.hpp>
        * \brief Default \c derived type specification.
        *
        * Serializes a dervied container type using the \c Specification parameter provided
        * to serializable_implementation.
        */
        template <typename T>
        struct serializable_default_specification <T, 
            typename std::enable_if <
                is_default_derived_serializable <T>::value,
                void
            >::type>
        {
            /**
            * \param in input stream
            * \param out output object
            * \return true on success, false on failure
            */
            template <typename Input, typename Output>
            static bool read (Input& in, Output& out)
            {
                return serializable_specification <typename T::serializable_derived_specification>::read(in, out);
            }
            
            /**
            * \param in input object
            * \param out output stream
            * \return true on success, false on failure
            */
            template <typename Input, typename Output>
            static bool write (Input const& in, Output& out)
            {
                return serializable_specification <typename T::serializable_derived_specification>::write(in, out);
            }
        };
    }
}

#include <framework/serializable/serializable.inl>

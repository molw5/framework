// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Value base type.
*
* \copyright
* Copyright &copy; 2012 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

#include <framework/serializable/base_types.hpp>
#include <framework/serializable/utility/interface.hpp>
#include <framework/serializable/mutator_type.hpp>

namespace framework
{
    namespace serializable
    {
        namespace detail
        {
            template <typename T, typename Enabler = void>
            struct is_value_type_impl;
            
            template <typename T, typename Enabler = void>
            struct is_default_value_serializable_impl;
        }

        /**
        * \headerfile value_type.hpp <framework/serializable/value_type.hpp>
        *
        * Provides a common interface used to define the types required to properly
        * interpret a value type.  Value types replace an object's member variables
        * in \c serializable types - \c Implementation is inherited by the derived
        * type and associated with the unique identifier \c Name - see \c serializable
        * for more information.
        *
        * The default specification provided for this type serializes the field
        * \c Name of the input/output object using the specification provided in
        * \c Specification, see the specialization of serializable_default_specification
        * for more information.  Derived types may suppress this default by setting 
        * \c Default to false, forcing the generation of a compile time error if an 
        * appropriate \c serializable_specification specialization is not found.
        *
        * \tparam Name unique identifying typename
        * \tparam Specification specification used to serialize the value
        * \tparam Implementation implementation required by the value
        */
        template <
            typename Name, 
            typename Specification,
            template <typename> class Interface,
            bool Default = true>
#ifndef DOXYGEN
        struct value_type;
#else
        struct value_type
        {
        };
#endif

        /**
        * \headerfile value_type.hpp <framework/serializable/value_type.hpp>
        * \brief Determines if \c T is a value type.
        *
        * Type trait testing whether or not T is a \c value_type.
        * Inherits std::true_type or std::false_type.
        */
        template <typename T>
        struct is_value_type :
            std::conditional <
                detail::is_value_type_impl <T>::value,
                std::true_type,
                std::false_type
            >::type
        {
        };

        /**
        * \headerfile value_type.hpp <framework/serializable/value_type.hpp>
        * \brief Get name.
        *
        * Retrieves a value type's name, storing the result in \c type.
        *
        * \pre is_value_type <T>::value == true
        */
        template <typename T>
        struct get_value_name
        {
            using type = 
                typename std::enable_if <
                    is_value_type <T>::value,
                    typename T::value_name
                >::type;
        };

        /**
        * \headerfile value_type.hpp <framework/serializable/value_type.hpp>
        * \brief Get specification.
        *
        * Retrieves a value type's specification, storing the result in \c type.
        *
        * \pre is_value_type <T>::value == true
        */
        template <typename T>
        struct get_value_specification
        {
            using type = 
                typename std::enable_if <
                    is_value_type <T>::value,
                    typename T::value_specification
                >::type;
        };

        /**
        * \headerfile value_type.hpp <framework/serializable/value_type.hpp>
        * \brief Get implementation.
        *
        * Instantiates a value type's implementation with the parameter \c U,
        * storing the result in \c type.
        *
        * \pre is_value_type <T>::value == true
        */
        template <typename T, typename U>
        struct get_value_implementation
        {
            using type = 
                typename std::enable_if <
                    is_value_type <T>::value,
                    typename T::template value_implementation <U>
                >::type;
        };

        /**
        * \headerfile value_type.hpp <framework/serializable/value_type.hpp>
        *
        * Type trait testing whether or not T is a default serializable \c value_type.
        * Derives from std::true_type or std::false_type.
        *
        * \note 
        * The following relationships always hold
        * \code
        * !is_default_value_serializable <T>::value || is_value_type <T>::value
        * !is_default_value_serializable <T>::value || is_default_serializable <T>::value
        * \endcode
        */
        template <typename T>
        struct is_default_value_serializable :
            std::conditional <
                detail::is_default_value_serializable_impl <T>::value,
                std::true_type,
                std::false_type
            >::type
        {
        };
        
        /**
        * \headerfile value_type.hpp <framework/serializable/value_type.hpp>
        *
        * Required specialization of is_default_serializable.
        */
        template <typename T>
        struct is_default_serializable <T,
            typename std::enable_if <
                is_default_value_serializable <T>::value,
                void
            >::type> : std::true_type
        {
        };
        
        /**
        * \headerfile value_type.hpp <framework/serializable/value_type.hpp>
        * \brief Default \c value_type specification.
        *
        * Serializes the field identified by
        * \code
        * get_value_name <T>::type
        * \endcode
        * using the specification given by
        * \code
        * get_value_specification <T>::type
        * \endcode
        * The specification is assumed to identify the field's underlying value type, as
        * specified by
        * \code
        * typename type_extractor <typename get_value_specification <T>::type>::type
        * \endcode
        * 
        * \note
        * The methods provided here access the implementation's method directly though
        * the reference provided by \c interface.
        */
        template <typename T>
        struct serializable_default_specification <T,
            typename std::enable_if <
                is_default_value_serializable <T>::value,
                void
            >::type>
        {
            /**
            * \note 
            * The field's underlying value type must be default constructable with an
            * associated implementation supporting move semantics through it's set method.
            *
            * \post the call succeeds or out is not altered
            * \param in input stream
            * \param out output container
            * \return true on success, false on failure
            */
            template <typename Input, typename Output>
            static bool read (Input& in, Output& out)
            {
                value_type value;
                if (!serializable_specification <value_specification>::read(in, value))
                    return false;

                interface <value_name> (out).set(std::move(value));
                return true;
            }

            /**
            * \param in input container
            * \param out output stream
            * \return true on success, false on failure
            */
            template <typename Input, typename Output>
            static bool write (Input const& in, Output& out)
            {
                auto const& value = interface <value_name> (in).get();
                return serializable_specification <value_specification>::write(value, out);
            }

            private:
                using value_name =
                    typename get_value_name <T>::type;

                using value_specification =
                    typename get_value_specification <T>::type;

                using value_type =
                    typename type_extractor <value_specification>::type;
        };
    }
}

#include <framework/serializable/value_type.inl>

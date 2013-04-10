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
#include <framework/serializable/mutator_type.hpp>

namespace framework
{
    namespace serializable
    {
        namespace detail
        {
            template <typename T, typename Enabler = void>
            struct is_value_type_impl;

            template <typename T>
            struct is_value_default_serializable_impl;

            template <typename T>
            struct value_name_impl;

            template <typename T>
            struct value_specification_impl;

            template <typename T, typename Derived>
            struct value_implementation_impl;
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
        * The default serialization methods provided for this type serialize the field
        * \c Name of the input/output object using the specification provided in
        * \c Specification.  Derived types may suppress this default by setting 
        * \c Default to false, forcing the generation of a compile time error if an 
        * appropriate read or write overload is not present.
        *
        * \tparam Name unique identifying typename
        * \tparam Specification specification used to serialize the value
        * \tparam Implementation implementation inherited by the derived class
        */
        template <
            typename Name, 
            typename Specification,
            template <typename> class Interface,
            bool Default = true>
        struct value_type
        {
// GCC bug workaround - www.open-std.org/jtc1/sc22/wg21/docs/cwg_defects.html#580
#if MAX_GNUC_VERSION(4, 8, 0)
            public:
#else
            template <typename T, typename Enabler>
            friend struct detail::is_value_type_impl;

            template <typename T>
            friend struct detail::is_value_default_serializable_impl;

            template <typename T>
            friend struct detail::value_name_impl;

            template <typename T>
            friend struct detail::value_specification_impl;

            template <typename T, typename Derived>
            friend struct detail::value_implementation_impl;

            private:
#endif
                using serializable_value_enabler = void;
                using serializable_value_name = Name;
                using serializable_value_specification = Specification;
                enum{ serializable_value_default = Default };

                template <typename T>
                using serializable_value_implementation = Interface <T>;

                value_type () = delete;
                ~value_type () = delete;
        };

        /**
        * \headerfile value_type.hpp <framework/serializable/value_type.hpp>
        *
        * Type trait testing whether or not T is a \c value_type, equivalent to either
        * std::true_type or std::false_type.
        */
        template <typename T>
        using is_value_type = typename detail::is_value_type_impl <T>::type;

        /**
        * \headerfile value_type.hpp <framework/serializable/value_type.hpp>
        *
        * Type trait testing whether or not T is a default serializable \c value_type,
        * equivalent to either std::true_type or std::false_type.
        */
        template <typename T>
        using is_value_default_serializable = typename detail::is_value_type_impl <T>::type;

        /**
        * \headerfile value_type.hpp <framework/serializable/value_type.hpp>
        *
        * Retrieves a value type's name.
        *
        * \pre is_value_type \<T\>\::value == true
        */
        template <typename T>
        using value_name = typename detail::value_name_impl <T>::type;

        /**
        * \headerfile value_type.hpp <framework/serializable/value_type.hpp>
        *
        * Retrieves a value type's specification.
        *
        * \pre is_value_type \<T\>\::value == true
        */
        template <typename T>
        using value_specification = typename detail::value_specification_impl <T>::type;

        /**
        * \headerfile value_type.hpp <framework/serializable/value_type.hpp>
        *
        * Instantiates a value type's implementation with the parameter \c Derived,
        * storing the result in \c type.
        *
        * \pre is_value_type \<T\>\::value == true
        */
        template <typename T, typename Derived>
        using value_implementation = typename detail::value_implementation_impl <T, Derived>::type;

        /**
        * \headerfile value_type.hpp <framework/serializable/value_type.hpp>
        * \brief Default read overload.
        *
        * \note 
        * The field's underlying value type must be default constructable with an
        * associated implementation supporting move semantics through it's set method.
        *
        * \post the call succeeds or out is not altered
        * \param in input stream
        * \param out output container
        * \return true on success, false on failure
        */
        template <
            typename Name, 
            typename Specification,
            template <typename> class Interface,
            typename Input,
            typename Output>
        bool read_dispatch (
            value_type <Name, Specification, Interface, true>*,
            Input&& in, Output&& out);

        /**
        * \headerfile value_type.hpp <framework/serializable/value_type.hpp>
        * \brief Default write overload.
        * \post the call succeeds or out is not altered
        * \param in input stream
        * \param out output container
        * \return true on success, false on failure
        */
        template <
            typename Name, 
            typename Specification,
            template <typename> class Interface,
            typename Input,
            typename Output>
        bool write_dispatch (
            value_type <Name, Specification, Interface, true>*,
            Input&& in, Output&& out);
    }
}

#include <framework/serializable/value_type.inl>

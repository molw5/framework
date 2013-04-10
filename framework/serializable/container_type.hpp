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
#include <framework/common/common_macros.hpp>
#include <framework/common/pack_container.hpp>

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
            struct is_container_default_serializable_impl;

            template <typename T>
            struct container_specification_impl;

            template <typename T>
            struct container_children_impl;
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
        * generation of a compile time error if an appropriate read or write overload
        * is not present.
        *
        * \tparam Specification pack_container specifying the formal children of the container
        * \tparam Children pack_container specifying the logical children of the container
        * \tparam Default flag enabling/disabling default serialization of the container
        */
        template <
            typename Specification,
            typename Children,
            bool Default = true>
        struct container_type
        {
// GCC bug workaround - www.open-std.org/jtc1/sc22/wg21/docs/cwg_defects.html#580
#if MAX_GNUC_VERSION(4, 8, 0)
            public:
#else
            template <typename T, typename Enabler>
            friend struct detail::is_container_type_impl;
            
            template <typename T, typename Enabler>
            friend struct detail::is_container_default_serializable_impl;

            template <typename T>
            friend struct detail::container_specification_impl;
            
            template <typename T>
            friend struct detail::container_children_impl;
            
            private:
#endif
                using serializable_container_enabler = void;
                using serializable_container_specification = typename std::enable_if <is_pack_container <Specification>::value, Specification>::type;
                using serializable_container_children = typename std::enable_if <is_pack_container <Children>::value, Children>::type;
                enum{ serializable_container_default = Default };

                container_type () = delete;
                ~container_type () = delete;
        };
        
        /**
        * \headerfile container_type.hpp <framework/serializable/container_type.hpp>
        *
        * Type trait testing whether or not T is a \c container_type, equivalent to either
        * std::true_type or std::false_type.
        */
        template <typename T>
        using is_container_type = typename detail::is_container_type_impl <T>::type;

        /**
        * \headerfile container_type.hpp <framework/serializable/container_type.hpp>
        *
        * Type trait testing whether or not T is a default serializable \c contaier_type,
        * equivalent to either std::true_type or std::false_type.
        */
        template <typename T>
        using is_container_default_serializable = typename detail::is_container_default_serializable_impl <T>::type;

        /**
        * \headerfile container_type.hpp <framework/serializable/container_type.hpp>
        *
        * Retrieves a container type's specification.
        *
        * \pre is_container_type \<T\>\::value == true
        */
        template <typename T>
        using container_specification = typename detail::container_specification_impl <T>::type;

        /**
        * \headerfile container_type.hpp <framework/serializable/container_type.hpp>
        *
        * Retrieves a container type's children.
        *
        * \pre is_container_type \<T\>\::value == true
        */
        template <typename T>
        using container_children = typename detail::container_children_impl <T>::type;

        /**
        * \headerfile container_type.hpp <framework/serializable/container_type.hpp>
        * \brief Default read overload.
        *
        * Serializes each element of container_children \<Specification\> in the order it
        * appears.
        *
        * \param args deserialization arguments, usually an input stream followed by an output container
        * \return true on success, false on failure
        */
        template <
            typename Specification,
            typename Children,
            typename... Args>
        FRAMEWORK_ALWAYS_INLINE
        bool read_dispatch (
            container_type <Specification, Children, true>*,
            Args&&... args)
        {
            return detail::recursive_serializable_specification <Specification>::read(std::forward <Args> (args)...);
        }

        /**
        * \headerfile container_type.hpp <framework/serializable/container_type.hpp>
        * \brief Default write overload.
        *
        * Serializes each element of container_children \<Specification\> in the order it
        * appears.
        *
        * \param args serialization arguments, usually an input container followed by an output stream
        * \return true on success, false on failure
        */
        template <
            typename Specification,
            typename Children,
            typename... Args>
        FRAMEWORK_ALWAYS_INLINE
        bool write_dispatch (
            container_type <Specification, Children, true>*,
            Args&&... args)
        {
            return detail::recursive_serializable_specification <Specification>::write(std::forward <Args> (args)...);
        }
    }
}

#include <framework/serializable/container_type.inl>

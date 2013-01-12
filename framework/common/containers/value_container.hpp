// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Value container.
*
* \copyright
* Copyright &copy; 2012 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

#include <framework/common/containers/index_container.hpp>

namespace framework
{
    namespace detail
    {
        template <typename T>
        struct is_value_container_impl;

        template <typename... Containers>
        struct merge_values_impl;

        template <typename Type, std::size_t Size>
        struct make_values_impl;
    }

    /**
    * \headerfile value_container.hpp <framework/common/containers/value_container.hpp>
    * \brief Value container.
    * 
    * Wraps a list of compile-time constants of type T, used to provide variadic switch templates
    * with a list of suitable values.
    */
    template <typename T, T... Values>
    struct value_container
    {
        using type = T;
        enum { size = sizeof... (Values) };
    };

    /**
    * \headerfile value_container.hpp <framework/common/containers/value_container.hpp>
    *
    * Type trait testing whether or not T is a \c value_container.
    */
    template <typename T>
    using is_value_container = typename detail::is_value_container_impl <T>::type;

    /**
    * \headerfile value_container.hpp <framework/common/containers/value_container.hpp>
    * \brief Merge value containers.
    *
    * Constructs a \c value_container containing all elements of the provided containers,
    * in the order they appear.  For example, the following are equivalent:
    *
    * \code
    * using results = merge_values <value_container <std::size_t, 1, 2, 3>, value_container <std::size_t <4, 5, 6>>>;
    * using results = merge_values <value_container <std::size_t, 1, 2, 3, 4, 5, 6>>;
    * \endcode
    *
    * \note
    * The types of the provided value_containers must match.
    *
    * \tparam Containers input \c value_container types
    */
    template <typename... Containers>
    using merge_values = typename detail::merge_values_impl <Containers...>::type;

    /**
    * \headerfile value_container.hpp <framework/common/containers/value_container.hpp>
    * \brief Basic case container constructor.
    *
    * Creates a value_container containing the elements [0, Size).  The following are equivalent:
    *
    * \code
    * using result = make_values <std::size_t, 3>;
    * using result = value_container <std::size_t, 0, 1, 2>;
    * \endcode
    */
    template <typename Type, std::size_t Size>
    using make_values = typename detail::make_values_impl <Type, Size>::type;
}

#include <framework/common/containers/value_container.inl>

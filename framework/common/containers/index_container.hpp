// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Variadic index container.
*
* \copyright
* Copyright &copy; 2012 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

namespace framework
{
    namespace detail
    {
        template <typename T>
        struct is_index_container_impl;

        template <std::size_t Size>
        struct make_indices_impl;

        template <typename... Indices>
        struct merge_indices_impl;
    }

    /**
    * \headerfile index_container.hpp <framework/common/containers/index_container.hpp>
    * \brief Variadic index container.
    */
    template <std::size_t... Indices>
    struct index_container
    {
        /**
        * \brief Index container size.
        */
        enum { size = sizeof... (Indices) };
    };

    /**
    * \headerfile index_container.hpp <framework/common/containers/index_container.hpp>
    *
    * Type trait testing whether or not T is an \c index_container.
    */
    template <typename T>
    using is_index_container = typename detail::is_index_container_impl <T>::type;

    /**
    * \headerfile index_container.hpp <framework/common/containers/index_container.hpp>
    * \brief Merge index containers.
    *
    * Constructs an \c index_container containing all elements of the provided containers,
    * in the order they appear.  For example, the following are equivalent:
    *
    * \code
    * using results = merge_indices <index_container <1, 2, 3>, index_container <4, 5, 6>>;
    * using results = index_container <1, 2, 3, 4, 5, 6>;
    * \endcode
    *
    * \tparam Containers input \c index_container types
    */
    template <typename... Containers>
    using merge_indices = typename detail::merge_indices_impl <Containers...>::type;

    /**
    * \headerfile index_container.hpp <framework/common/containers/index_container.hpp>
    * \brief Index container constructor.
    *
    * Creates an index_container containing the elements [0, Size).  The following are equivalent:
    *
    * \code
    * using result = make_indices <3>;
    * using result = index_container <0, 1, 2>;
    * \endcode
    */
    template <std::size_t Size>
    using make_indices = typename detail::make_indices_impl <Size>::type;
}

#include <framework/common/containers/index_container.inl>

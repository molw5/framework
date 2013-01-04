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
        template <std::size_t Size, typename Indices = make_indices <Size>>
        struct make_value_indices_impl;
    }

    /**
    * \headerfile value_container.hpp <framework/common/containers/value_container.hpp>
    * \brief Case container.
    */
    template <typename T, T... Values>
    struct value_container
    {
        using type = T;
        enum { size = sizeof... (Values) };
    };

    /**
    * \headerfile value_container.hpp <framework/common/containers/value_container.hpp>
    * \brief Basic case container constructor.
    *
    * Creates a value_container containing the elements [0, Size).  The following are equivalent:
    *
    * \code
    * using result = make_value_indices <3>;
    * using result = value_container <std::size_t, 0, 1, 2>;
    * \endcode
    */
    template <std::size_t Size>
    using make_value_indices = typename detail::make_value_indices_impl <Size>::type;
}

#include <framework/common/containers/value_container.inl>

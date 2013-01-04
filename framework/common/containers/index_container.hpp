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
        template <std::size_t Size>
        struct make_indices_impl;
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

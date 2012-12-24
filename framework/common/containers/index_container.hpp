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
    * Creates a index container suitable for instantiating the elements of a tuple.  The following
    * are equivalent:
    *
    * \code
    * using result = typename make_indicies <3>::type;
    * using result = index_container <0, 1, 2>;
    * \endcode
    */
    template <std::size_t Size>
#ifndef DOXYGEN
    struct make_indices;
#else
    struct make_indices
    {
        /**
        * \brief Result.
        */
        typedef type;
    };
#endif
}

#include <framework/common/containers/index_container.inl>

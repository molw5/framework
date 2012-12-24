// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Type string container.
*
* \copyright
* Copyright &copy; 2012 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

namespace framework
{
    /**
    * \struct type_string type_string.hpp <framework/common/containers/type_string.hpp>
    * \brief Type string.
    *
    * Provides a simple wrapper around a sequence of characters.
    */
    template <char... Array>
    struct type_string
    {
        /**
        * \brief Type string size.
        */
        enum { size = sizeof... (Array) };
    };

    /**
    * \struct make_type_string type_string.hpp <framework/common/containers/type_string.hpp>
    * \brief Make type string.
    *
    * Constructs a type string from a \c '\0' terminated sequence of input characters,
    * providing a reduced type string in \c type.  The following are equivalent:
    *
    * \code
    * using result = typename make_type_string <'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!', '\0', '\0'>::type;
    * using result = type_string <'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!'>;
    * \endcode
    */
    template <char... Array>
#ifndef DOXYGEN
    struct make_type_string;
#else
    struct make_type_string
    {
        /**
        * \brief Result.
        */
        typedef type;
    };
#endif
}

#include <framework/common/containers/type_string.inl>

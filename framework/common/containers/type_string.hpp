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
    namespace detail
    {
        template <typename T>
        struct is_type_string_impl;

        template <char... Array>
        struct make_type_string_impl;

        template <typename... Strings>
        struct merge_strings_impl;
    }

    /**
    * \headerfile type_string.hpp <framework/common/containers/type_string.hpp>
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
    * \headerfile type_string.hpp <framework/common/containers/type_string.hpp>
    *
    * Type trait testing whether or not T is a \c type_string.
    */
    template <typename T>
    using is_type_string = typename detail::is_type_string_impl <T>::type;

    /**
    * \headerfile type_string.hpp <framework/common/containers/type_string.hpp>
    * \brief Make type string.
    *
    * Constructs a type string from a \c '\0' terminated sequence of input characters,
    * providing a reduced type string in \c type.  The following are equivalent:
    *
    * \code
    * using result = make_type_string <'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!', '\0', '\0'>;
    * using result = type_string <'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!'>;
    * \endcode
    */
    template <char... Array>
    using make_type_string = typename detail::make_type_string_impl <Array...>::type;

    /**
    * \headerfile type_string.hpp <framework/common/containers/type_string.hpp>
    * \brief Merge type strings.
    */
    template <typename... Strings>
    using merge_strings = typename detail::merge_strings_impl <Strings...>::type;
}

#include <framework/common/containers/type_string.inl>

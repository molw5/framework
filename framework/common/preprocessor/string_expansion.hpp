// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Template string expansion macros.
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
        template <unsigned int N>
        constexpr char extract_char (char const (&string) [N], unsigned int index)
        {
            return index < N ? string[index] : '\0';
        }
    }
}

#ifndef FRAMEWORK_NO_BOOST
    #include <boost/preprocessor/repetition/repeat.hpp>
    #include <boost/preprocessor/punctuation/comma_if.hpp>

    #define EXPAND_STRING_N_IMPL(z, n, data) \
            BOOST_PP_COMMA_IF(n) framework::detail::extract_char(data, n)

    /**
    * \headerfile string_expansion.hpp <framework/common/preprocessor/string_expansion.hpp>
    * \brief Expand string literal to char... pack.
    *
    * Expands a string literal to a sequence of \c LENGTH characters suitable for use in 
    * \c char... template parameter pack.  Characters beyond the end of the string are
    * converted to \c '\0' characters.  For example, the following are equivalent:
    * \code
    * template <char...>
    * struct S;
    *
    * S <'H', 'e', 'l', 'l', 'o', ',', 'w', 'o', 'r', 'l', 'd', '!'> x;
    * S <EXPAND_STRING("Hello World!", 12)> x;
    *
    * \brief LENGTH maximum string length
    * \brief STRING string literal
    */
    #define EXPAND_STRING_N(STRING, LENGTH) \
            BOOST_PP_REPEAT(LENGTH, EXPAND_STRING_N_IMPL, STRING)
#else
    /**
    * \headerfile string_expansion.hpp <framework/common/preprocessor/string_expansion.hpp>
    * \brief Expand string literal to char... pack.
    *
    * Provides an alternative to EXPAND_STRING_N in the absence of boost.  Equivalent to
    * \code
    * EXPAND_STRING_N(STRING, 100)
    * \endcode
    *
    * \brief STRING string literal
    */
    #define EXPAND_STRING(STRING) \
            framework::detail::extract_char(STRING, 0), \
            framework::detail::extract_char(STRING, 1), \
            framework::detail::extract_char(STRING, 2), \
            framework::detail::extract_char(STRING, 3), \
            framework::detail::extract_char(STRING, 4), \
            framework::detail::extract_char(STRING, 5), \
            framework::detail::extract_char(STRING, 6), \
            framework::detail::extract_char(STRING, 7), \
            framework::detail::extract_char(STRING, 8), \
            framework::detail::extract_char(STRING, 9), \
            framework::detail::extract_char(STRING, 10), \
            framework::detail::extract_char(STRING, 11), \
            framework::detail::extract_char(STRING, 12), \
            framework::detail::extract_char(STRING, 13), \
            framework::detail::extract_char(STRING, 14), \
            framework::detail::extract_char(STRING, 15), \
            framework::detail::extract_char(STRING, 16), \
            framework::detail::extract_char(STRING, 17), \
            framework::detail::extract_char(STRING, 18), \
            framework::detail::extract_char(STRING, 19), \
            framework::detail::extract_char(STRING, 20), \
            framework::detail::extract_char(STRING, 21), \
            framework::detail::extract_char(STRING, 22), \
            framework::detail::extract_char(STRING, 23), \
            framework::detail::extract_char(STRING, 24), \
            framework::detail::extract_char(STRING, 25), \
            framework::detail::extract_char(STRING, 26), \
            framework::detail::extract_char(STRING, 27), \
            framework::detail::extract_char(STRING, 28), \
            framework::detail::extract_char(STRING, 29), \
            framework::detail::extract_char(STRING, 30), \
            framework::detail::extract_char(STRING, 31), \
            framework::detail::extract_char(STRING, 32), \
            framework::detail::extract_char(STRING, 33), \
            framework::detail::extract_char(STRING, 34), \
            framework::detail::extract_char(STRING, 35), \
            framework::detail::extract_char(STRING, 36), \
            framework::detail::extract_char(STRING, 37), \
            framework::detail::extract_char(STRING, 38), \
            framework::detail::extract_char(STRING, 39), \
            framework::detail::extract_char(STRING, 40), \
            framework::detail::extract_char(STRING, 41), \
            framework::detail::extract_char(STRING, 42), \
            framework::detail::extract_char(STRING, 43), \
            framework::detail::extract_char(STRING, 44), \
            framework::detail::extract_char(STRING, 45), \
            framework::detail::extract_char(STRING, 46), \
            framework::detail::extract_char(STRING, 47), \
            framework::detail::extract_char(STRING, 48), \
            framework::detail::extract_char(STRING, 49), \
            framework::detail::extract_char(STRING, 50), \
            framework::detail::extract_char(STRING, 51), \
            framework::detail::extract_char(STRING, 52), \
            framework::detail::extract_char(STRING, 53), \
            framework::detail::extract_char(STRING, 54), \
            framework::detail::extract_char(STRING, 55), \
            framework::detail::extract_char(STRING, 56), \
            framework::detail::extract_char(STRING, 57), \
            framework::detail::extract_char(STRING, 58), \
            framework::detail::extract_char(STRING, 59), \
            framework::detail::extract_char(STRING, 60), \
            framework::detail::extract_char(STRING, 61), \
            framework::detail::extract_char(STRING, 62), \
            framework::detail::extract_char(STRING, 63), \
            framework::detail::extract_char(STRING, 64), \
            framework::detail::extract_char(STRING, 65), \
            framework::detail::extract_char(STRING, 66), \
            framework::detail::extract_char(STRING, 67), \
            framework::detail::extract_char(STRING, 68), \
            framework::detail::extract_char(STRING, 69), \
            framework::detail::extract_char(STRING, 70), \
            framework::detail::extract_char(STRING, 71), \
            framework::detail::extract_char(STRING, 72), \
            framework::detail::extract_char(STRING, 73), \
            framework::detail::extract_char(STRING, 74), \
            framework::detail::extract_char(STRING, 75), \
            framework::detail::extract_char(STRING, 76), \
            framework::detail::extract_char(STRING, 77), \
            framework::detail::extract_char(STRING, 78), \
            framework::detail::extract_char(STRING, 79), \
            framework::detail::extract_char(STRING, 80), \
            framework::detail::extract_char(STRING, 81), \
            framework::detail::extract_char(STRING, 82), \
            framework::detail::extract_char(STRING, 83), \
            framework::detail::extract_char(STRING, 84), \
            framework::detail::extract_char(STRING, 85), \
            framework::detail::extract_char(STRING, 86), \
            framework::detail::extract_char(STRING, 87), \
            framework::detail::extract_char(STRING, 88), \
            framework::detail::extract_char(STRING, 89), \
            framework::detail::extract_char(STRING, 90), \
            framework::detail::extract_char(STRING, 91), \
            framework::detail::extract_char(STRING, 92), \
            framework::detail::extract_char(STRING, 93), \
            framework::detail::extract_char(STRING, 94), \
            framework::detail::extract_char(STRING, 95), \
            framework::detail::extract_char(STRING, 96), \
            framework::detail::extract_char(STRING, 97), \
            framework::detail::extract_char(STRING, 98), \
            framework::detail::extract_char(STRING, 99)
#endif

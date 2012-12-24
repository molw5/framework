// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Common STL string aliases.
*
* \copyright
* Copyright &copy; 2012 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

#pragma once

#include <string>

#include <framework/serializable/mutators/variable_container.hpp>
#include <framework/serializable/mutators/terminated.hpp>

namespace framework
{
    namespace serializable
    {
        /**
        * \brief Common STL string specification.
        */
        template <typename Size>
        using stl_string = variable_container <Size, char, std::string>;

        /**
        * \brief Common STL string specification, null terminated.
        */
        using stl_null_string = terminated <char, '\0', std::string>;

        /**
        * \brief Common STL wstring specification.
        *
        * \note 
        * This template uses little endian byte ordering to store individual characters -
        * allowing this parameter to be user-specified here (ie: stl_wstring <\c Size, \c Value>)
        * would introduce an implicit constraints or unusual mutator syntax. If this default is
        * inadequate the \c stl_basic_string template should be used instead.
        */
        template <typename Size>
        using stl_wstring = variable_container <Size, little_endian <wchar_t>, std::wstring>;

        /**
        * \brief Common STL wstring specification, null terminated.
        *
        * \note 
        * This template uses little endian byte ordering to store individual characters -
        * allowing this parameter to be user-specified here (ie: stl_wstring <\c Size, \c Value>)
        * would introduce an implicit constraints or unusual mutator syntax. If this default is
        * inadequate the \c stl_null_basic_string template should be used instead.
        */
        using stl_null_wstring = terminated <little_endian <wchar_t>, L'\0', std::wstring>;

        /**
        * \brief Common STL basic string specification.
        */
        template <
            typename Size, 
            typename CharT,
            typename Traits = std::char_traits <typename type_extractor <CharT>::type>,
            typename Allocator = std::allocator <typename type_extractor <CharT>::type>>
        using stl_basic_string = variable_container <
            Size,
            typename type_extractor <CharT>::type,
            std::basic_string <typename type_extractor <CharT>::type, Traits, Allocator>>;

        /**
        * \brief Common STL basic string specification, null terminated.
        */
        template <
            typename CharT,
            typename Traits = std::char_traits <typename type_extractor <CharT>::type>,
            typename Allocator = std::allocator <typename type_extractor <CharT>::type>>
        using stl_null_basic_string = terminated <
            typename type_extractor <CharT>::type,
            static_cast <typename type_extractor <CharT>::type> (0),
            std::basic_string <typename type_extractor <CharT>::type, Traits, Allocator>>;
    }
}

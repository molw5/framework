// Copyright (C) 2013 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Compile time tag helpers.
*
* \copyright
* Copyright &copy; 2013 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

#include <cassert>
#include <type_traits>
#include <string.h>

#include <framework/common/endian.hpp>

namespace framework
{
    namespace protocol_buffers
    {
        /**
        * \brief Compile time tag definition.
        */
        template <unsigned long ID, unsigned char WireType, typename Enabler = void>
        struct fixed_tag;

        /**
        * \brief Write tag.
        */
        template <typename Tag, typename Output>
        FRAMEWORK_ALWAYS_INLINE
        bool write_tag (Output& out);

        /**
        * \brief Write tag.
        */
        template <typename Tag>
        FRAMEWORK_ALWAYS_INLINE
        bool write_tag (char* begin, char* end, char*& it);

        /**
        * \brief Expect tag.
        */
        template <typename Tag>
        FRAMEWORK_ALWAYS_INLINE
        bool expect_tag (char const* begin, char const* end, char const*& it);
    }
}

#include <framework/protocol_buffers/fixed_tag.inl>

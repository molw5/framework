// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Word swap definitions.
*
* \copyright
* Copyright &copy; 2012 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

namespace framework
{
    /**
    * \brief Common 32-bit word swap implementation.
    */
    template <typename T>
    FRAMEWORK_ALWAYS_INLINE
    T wswap32 (T&& in)
    {
        static_assert(sizeof(T) == 4, "32-bit type expected");
        uint32_t const x = static_cast <uint32_t> (in);
        return static_cast <T> ((x >> 16) | (x << 16));
    }

    /**
    * \brief Common 64-bit word swap implementation.
    */
    template <typename T>
    FRAMEWORK_ALWAYS_INLINE
    T wswap64 (T&& in)
    {
        static_assert(sizeof(T) == 8, "64-bit type expected");
        uint64_t x = static_cast <uint64_t> (in);
        x = ((x & 0xFFFF0000FFFF0000) >> 16) | ((x & 0x0000FFFF0000FFFF) << 16);
        return static_cast <T> ((x >> 32) | (x << 32));
    }
}
    
#define FRAMEWORK_WORDSWAP8(x) x
#define FRAMEWORK_WORDSWAP16(x) x
#define FRAMEWORK_WORDSWAP32(x) ::framework::wswap32(x)
#define FRAMEWORK_WORDSWAP64(x) ::framework::wswap64(x)

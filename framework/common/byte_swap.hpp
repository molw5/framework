// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Byte swap definitions.
*
* \copyright
* Copyright &copy; 2012 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

namespace framework
{
    /**
    * \brief Common 16-bit byte swap implementation.
    */
    template <typename T>
    T bswap16 (T const& in)
    {
        static_assert(sizeof(T) == 2, "16-bit type expected");
        uint16_t x = static_cast <uint16_t> (in);
        x = (x >> 8) | (x << 8);
        return x;
    }

    /**
    * \brief Common 32-bit byte swap implementation.
    */
    template <typename T>
    T bswap32 (T const& in)
    {
        static_assert(sizeof(T) == 4, "32-bit type expected");
        uint32_t x = static_cast <uint32_t> (in);
        x = ((x & 0xFF00FF00) >>  8) | ((x & 0x00FF00FF) <<  8);
        x = ((x & 0xFFFF0000) >> 16) | ((x & 0x0000FFFF) << 16);
        return x;
    }

    /**
    * \brief Common 64-bit byte swap implementation.
    */
    template <typename T>
    T bswap64 (T const& in)
    {
        static_assert(sizeof(T) == 8, "64-bit type expected");
        uint64_t x = static_cast <uint64_t> (in);
        x = ((x & 0xFF00FF00FF00FF00) >>  8) | ((x & 0x00FF00FF00FF00FF) <<  8);
        x = ((x & 0xFFFF0000FFFF0000) >> 16) | ((x & 0x0000FFFF0000FFFF) << 16);
        x = ((x & 0xFFFFFFFF00000000) >> 32) | ((x & 0x00000000FFFFFFFF) << 32);
        return x;
    }
}

#if defined(__clang__)
    #define FRAMEWORK_BYTESWAP8(x) x
    #define FRAMEWORK_BYTESWAP16(x) __builtin_bswap16(x)
    #define FRAMEWORK_BYTESWAP32(x) __builtin_bswap32(x)
    #define FRAMEWORK_BYTESWAP64(x) __builtin_bswap64(x)
#elif defined(__GNUC__)
    #define FRAMEWORK_BYTESWAP8(x) x
    #define FRAMEWORK_BYTESWAP16(x) ::framework::bswap16(x)
    #define FRAMEWORK_BYTESWAP32(x) __builtin_bswap32(x)
    #define FRAMEWORK_BYTESWAP64(x) __builtin_bswap64(x)
#else
    #define FRAMEWORK_BYTESWAP8(x) x
    #define FRAMEWORK_BYTESWAP16(x) ::framework::bswap16(x)
    #define FRAMEWORK_BYTESWAP32(x) ::framework::bswap32(x)
    #define FRAMEWORK_BYTESWAP64(x) ::framework::bswap64(x)
#endif

// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Endianness definitions.
*
* \todo 
* A common runtime test to verify the endianness of the machine actually matches the
* definitions provided here should be added.
*
* \copyright
* Copyright &copy; 2012 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

#include <framework/common/byte_swap.hpp>
#include <framework/common/word_swap.hpp>

/**
* \headerfile endian.hpp <framework/common/endian.hpp>
* \brief Big endian constant.
*/
#define FRAMEWORK_BIG_ENDIAN 1

/**
* \brief Little endian constant.
*/
#define FRAMEWORK_LITTLE_ENDIAN 2

/**
* \brief PDP (middle) endian constant.
*/
#define FRAMEWORK_PDP_ENDIAN 3

#ifdef DOXYGEN
    #error "Documentation only"
    /**
    * \headerfile endian.hpp <framework/common/endian.hpp>
    * \brief Host endianness.
    *
    * Defines the host endianness assumed by this framework.  This value may be defined explicitly 
    * or deduced from a platform-specific package like \c endian.h.  Must be set to one of the 
    * \c FRAMEWORK_*_ENDIAN macro values provides in this documentation.
    */
    #define FRAMEWORK_HOST_ENDIANNESS
    
    /**
    * \headerfile endian.hpp <framework/common/endian.hpp>
    * \brief Host float endianness.
    *
    * Defines the host floating point endianness assumed by this framework.  This value may be 
    * defined explicitly or deduced from a platform-specific package like \c endian.h.  Must be set 
    * to one of the \c FRAMEWORK_*_ENDIAN macro values provides in this documentation.
    */
    #define FRAMEWORK_HOST_FLOAT_ENDIANNESS
#endif

#ifndef FRAMEWORK_HOST_ENDIANNESS
    // TODO: This test is not sufficint to guarentee the existance to endian.h - improve if possible
    #ifdef __unix__
        #include <endian.h>
        #if __BYTE_ORDER == __LITTLE_ENDIAN
            #define FRAMEWORK_HOST_ENDIANNESS FRAMEWORK_LITTLE_ENDIAN
        #elif __BYTE_ORDER == __BIG_ENDIAN
            #define FRAMEWORK_HOST_ENDIANNESS FRAMEWORK_BIG_ENDIAN
        #elif __BYTE_ORDER == __PDP_ENDIAN
            #define FRAMEWORK_HOST_ENDIANNESS FRAMEWORK_PDP_ENDIAN
        #else
            #error "Host endianness not supported"
        #endif
    #else
        #error "Platform not supported";
    #endif
#endif

#ifndef FRAMEWORK_HOST_FLOAT_ENDIANNESS
    // TODO: This test is not sufficint to guarentee the existance to endian.h - improve if possible
    #ifdef __unix__
        #include <endian.h>
        #if __FLOAT_WORD_ORDER == __LITTLE_ENDIAN
            #define FRAMEWORK_HOST_FLOAT_ENDIANNESS FRAMEWORK_LITTLE_ENDIAN
        #elif __FLOAT_WORD_ORDER == __BIG_ENDIAN
            #define FRAMEWORK_HOST_FLOAT_ENDIANNESS FRAMEWORK_BIG_ENDIAN
        #elif __FLOAT_WORD_ORDER == __PDP_ENDIAN
            #define FRAMEWORK_HOST_FLOAT_ENDIANNESS FRAMEWORK_PDP_ENDIAN
        #else
            #error "Host endianness not supported"
        #endif
    #else
        #error "Platform not supported";
    #endif
#endif

// A compatible FRAMEWORK_HOST_ENDIANNESS value must be provided
#if FRAMEWORK_HOST_ENDIANNESS == FRAMEWORK_BIG_ENDIAN
#elif FRAMEWORK_HOST_ENDIANNESS == FRAMEWORK_LITTLE_ENDIAN
#elif FRAMEWORK_HOST_ENDIANNESS == FRAMEWORK_PDP_ENDIAN
#else
    #error "Invalid FRAMEWORK_HOST_ENDIANNESS definition"
#endif

// A compatible FRAMEWORK_HOST_FLOAT_ENDIANNESS value must be provided
#if FRAMEWORK_HOST_FLOAT_ENDIANNESS == FRAMEWORK_BIG_ENDIAN
#elif FRAMEWORK_HOST_FLOAT_ENDIANNESS == FRAMEWORK_LITTLE_ENDIAN
#elif FRAMEWORK_HOST_FLOAT_ENDIANNESS == FRAMEWORK_PDP_ENDIAN
#else
    #error "Invalid FRAMEWORK_HOST_FLOAT_ENDIANNESS definition"
#endif

namespace framework
{
    /**
    * \brief Byte order alias.
    *
    * Provides a series of enum values that may be used in place of the preprocessor
    * definitions above.
    */
    enum class byte_order
    {
        little_endian = FRAMEWORK_LITTLE_ENDIAN,
        big_endian = FRAMEWORK_BIG_ENDIAN,
        pdp_endian = FRAMEWORK_PDP_ENDIAN,
        host_endian = FRAMEWORK_HOST_ENDIANNESS
    };

    namespace detail
    {
        template <typename T, typename Enabler = void>
        struct little_to_host_impl;

        template <typename T, typename Enabler = void>
        struct host_to_little_impl;

        template <typename T, typename Enabler = void>
        struct big_to_host_impl;

        template <typename T, typename Enabler = void>
        struct host_to_big_impl;

        template <typename T, typename Enabler = void>
        struct pdp_to_host_impl;

        template <typename T, typename Enabler = void>
        struct host_to_pdp_impl;

        template <typename T>
        using reduce = typename std::remove_cv <typename std::remove_reference <T>::type>::type;
    }

    template <typename T>
    detail::reduce <T> little_to_host (T&& x)
    {
        return detail::little_to_host_impl <detail::reduce <T>>::run(std::forward <T> (x));
    }

    template <typename T>
    detail::reduce <T> host_to_little (T&& x)
    {
        return detail::host_to_little_impl <detail::reduce <T>>::run(std::forward <T> (x));
    }

    template <typename T>
    detail::reduce <T> big_to_host (T&& x)
    {
        return detail::big_to_host_impl <detail::reduce <T>>::run(std::forward <T> (x));
    }

    template <typename T>
    detail::reduce <T> host_to_big (T&& x)
    {
        return detail::host_to_big_impl <detail::reduce <T>>::run(std::forward <T> (x));
    }

    template <typename T>
    detail::reduce <T> pdp_to_host (T&& x)
    {
        return detail::pdp_to_host_impl <detail::reduce <T>>::run(std::forward <T> (x));
    }

    template <typename T>
    detail::reduce <T> host_to_pdp (T&& x)
    {
        return detail::host_to_pdp_impl <detail::reduce <T>>::run(std::forward <T> (x));
    }
}

#include <framework/common/endian.inl>

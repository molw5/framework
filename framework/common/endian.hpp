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

/**
* \headerfile endian.hpp <framework/common/endian.hpp>
* \brief Big endian constant.
*/
#define FRAMEWORK_BIG_ENDIAN 1

/**
* \brief Little endian constant.
*/
#define FRAMEWORK_LITTLE_ENDIAN 2

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
#else
    #error "Invalid FRAMEWORK_HOST_ENDIANNESS definition"
#endif

// A compatible FRAMEWORK_HOST_FLOAT_ENDIANNESS value must be provided
#if FRAMEWORK_HOST_FLOAT_ENDIANNESS == FRAMEWORK_BIG_ENDIAN
#elif FRAMEWORK_HOST_FLOAT_ENDIANNESS == FRAMEWORK_LITTLE_ENDIAN
#else
    #error "Invalid FRAMEWORK_HOST_FLOAT_ENDIANNESS definition"
#endif

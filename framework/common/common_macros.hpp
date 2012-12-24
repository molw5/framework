// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Common macros.
*
* \copyright
* Copyright &copy; 2012 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

/**
* \headerfile common_macros.hpp <framework/common/common_macros.hpp>
*
* Tests whether or not the GCC version is less than or equal to that
* specified by the arguments.
*/
#define MAX_GNUC_VERSION(major, minor, patchlevel) \
        defined(__GNUC__) && \
            (__GNUC__ < major || (__GNUC__ == major && \
                (__GNUC_MINOR__ < minor || (__GNUC_MINOR__ == minor && \
                    __GNUC_PATCHLEVEL__ <= patchlevel))))

/**
* \headerfile common_macros.hpp <framework/common/common_macros.hpp>
*
* Tests whether or not the GCC version is greater than or equal to that
* specified by the arguments.
*/
#define MIN_GNUC_VERSION(major, minor, patchlevel) \
        defined(__GNUC__) && \
            (__GNUC__ > major || (__GNUC__ == major && \
                (__GNUC_MINOR__ > minor || (__GNUC_MINOR__ == minor && \
                    __GNUC_PATCHLEVEL__ >= patchlevel))))

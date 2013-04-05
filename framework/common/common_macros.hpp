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
* Tests whether or not GCC's version is less than or equal to that
* specified by the arguments.
*/
#define MAX_GNUC_VERSION(major, minor, patchlevel) \
        defined(__GNUC__) && !defined(__clang__) && \
            (__GNUC__ < major || (__GNUC__ == major && \
                (__GNUC_MINOR__ < minor || (__GNUC_MINOR__ == minor && \
                    __GNUC_PATCHLEVEL__ <= patchlevel))))

/**
* \headerfile common_macros.hpp <framework/common/common_macros.hpp>
*
* Tests whether or not GCC's version is greater than or equal to that
* specified by the arguments.
*/
#define MIN_GNUC_VERSION(major, minor, patchlevel) \
        defined(__GNUC__) && !defined(__clang__) && \
            (__GNUC__ > major || (__GNUC__ == major && \
                (__GNUC_MINOR__ > minor || (__GNUC_MINOR__ == minor && \
                    __GNUC_PATCHLEVEL__ >= patchlevel))))

/**
* \headerfile common_macros.hpp <framework/common/common_macros.hpp>
*
* Tests whether or not clang's version is less than or equal to that
* specified by the arguments.
*/
#define MAX_CLANG_VERSION(major, minor, patchlevel) \
        defined(__clang__) && \
            (__clang__ < major || (__clang__ == major && \
                (__clang_minor__ < minor || (__clang_minor__ == minor && \
                    __clang_patchlevel__ <= patchlevel))))

/**
* \headerfile common_macros.hpp <framework/common/common_macros.hpp>
*
* Tests whether or not clang's version is greater than or equal to that
* specified by the arguments.
*/
#define MIN_CLANG_VERSION(major, minor, patchlevel) \
        defined(__clang__) && \
            (__clang__ > major || (__clang__ == major && \
                (__clang_minor__ > minor || (__clang_minor__ == minor && \
                    __clang_patchlevel__ >= patchlevel))))

/**
* \headerfile common_macros.hpp <framework/common/common_macros.hpp>
*
* Compiler specific built-in expect.
*/
#ifndef FRAMEWORK_DISABLE_EXPECT
    #if defined(__GNUC__)
        #define FRAMEWORK_EXPECT_TRUE(...) __builtin_expect(!!(__VA_ARGS__), 1)
        #define FRAMEWORK_EXPECT_FALSE(...) __builtin_expect(!!(__VA_ARGS__), 0)
    #else
        #define FRAMEWORK_EXPECT_TRUE(...) !!(__VA_ARGS__)
        #define FRAMEWORK_EXPECT_FALSE(...) !!(__VA_ARGS__)
    #endif
#else
    #define FRAMEWORK_EXPECT_TRUE(...) !!(__VA_ARGS__)
    #define FRAMEWORK_EXPECT_FALSE(...) !!(__VA_ARGS__)
#endif

/**
* \headerfile common_macros.hpp <framework/common/common_macros.hpp>
*
* Compiler specific always-inline attributes.
*/
#ifndef FRAMEWORK_DISABLE_ALWAYS_INLINE
    #if defined(__GNUC__)
        #define FRAMEWORK_ALWAYS_INLINE __attribute__ ((always_inline)) inline
    #elif defined(_MSC_VER)
        #define FRAMEWORK_ALWAYS_INLINE __inline
    #else
        #define FRAMEWORK_ALWAYS_INLINE inline
    #endif
#else
    #define FRAMEWORK_ALWAYS_INLINE
#endif

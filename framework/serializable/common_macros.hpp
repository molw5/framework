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

#include <framework/common/containers/type_string.hpp>
#include <framework/common/preprocessor/string_expansion.hpp>

#ifndef FRAMEWORK_NO_BOOST
    // Name macro definitions and helpers
    #ifndef FRAMEWORK_SERIALIZABLE_NAME_LENGTH 
        /**
        * Maximum name length specifier.  Note that names exceeding this limit will generate a compile
        * time error.
        */
        #define FRAMEWORK_SERIALIZABLE_NAME_LENGTH 100
    #endif

    #ifndef FRAMEWORK_SERIALIZABLE_NAME
        /**
        * Generates a unique typename from a string literal.
        */
        #define FRAMEWORK_SERIALIZABLE_NAME(s) \
                ::framework::make_type_string <EXPAND_STRING_N(s, FRAMEWORK_SERIALIZABLE_NAME_LENGTH)>
    #else
        #error "FRAMEWORK_SERIALIZABLE_NAME macro already defined";
    #endif
#else
    #define FRAMEWORK_SERIALIZABLE_NAME(s) \
            ::framework::make_type_string <EXPAND_STRING(s)>
#endif

/**
* FRAMEWORK_SERIALIZABLE_NAME alias
*/
#ifndef FRAMEWORK_SERIALIZABLE_NAME_MACRO_DISABLE
    #ifndef NAME
        #define NAME(s) FRAMEWORK_SERIALIZABLE_NAME(s)
    #else
        #error "NAME macro already defined - FRAMEWORK_SERIALIZABLE_NAME_MACRO_DISABLE suppresses this definition";
    #endif
#endif

#ifndef DEFINE_BASE_TEMPLATE
    /**
    * Provides a common get_base alias.  The following are equivalent:
    *
    * \code
    * DEFINE_BASE_TEMPLATE(derived);
    * 
    * using field_1 = typename get_base <derived, NAME("Field 1")>::type;
    * using field_1 = base <NAME("Field 1")>;
    * \endcode
    */   
    #define DEFINE_BASE_TEMPLATE(derived) \
            template <typename T> \
            using base = ::framework::serializable::object_base <derived, T>
#else
    #error "DEFINE_BASE_TEMPLATE macro already defined";
#endif

// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Variadic switch (fall-through).
*
* \todo
* Support for the FRAMEWORK_NO_BOOST macro needs to be added - a reasonable fixed switch limit
* should be selected and the relevant macros should be invoked directly.
*
* \copyright
* Copyright &copy; 2012 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

#ifdef FRAMEWORK_NO_BOOST
    #error "Not implemented"
#endif

#include <utility>

#include <boost/preprocessor/config/limits.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/iteration/local.hpp>

#include <framework/common/pack_container.hpp>
#include <framework/common/common_macros.hpp>

#ifndef FRAMEWORK_VARIADIC_SWITCH_LIMIT
    #define FRAMEWORK_VARIADIC_SWITCH_LIMIT 20
#else
    #if FRAMEWORK_VARIADIC_SWITCH_LIMIT > BOOST_PP_LIMIT_REPEAT
        #error "Variadic switch limit cannot exceed BOOST_PP_LIMIT_REPEAT"
    #endif
#endif

#if FRAMEWORK_VARIADIC_SWITCH_LIMIT <= 0
    #error "Invalid variadic switch limit"
#endif

namespace framework
{
    namespace detail
    {
        template <typename CaseList>
        struct variadic_switch_fallthrough_impl;
    }

    /**
    * \brief Variadic switch (fall-through).
    *
    * Expands a variadic case list into a switch statement in groups of 
    * \c FRAMEWORK_VARIADIC_SWITCH_LIMIT elements.  This variant falls-through subsequent cases
    * on success - for example, given a limit of 5 or higher, the following call to
    * \c variadic_switch_fallthrough:
    *
    * \code
    * variadic_switch_fallthrough <make_indices <5>> (f, i);
    * \endcode
    *
    * is equivalent to the following switch statement:
    *
    * \code
    * switch (i)
    * {
    *     case 0: f.operator() <0> ();
    *     case 1: f.operator() <1> ();
    *     case 2: f.operator() <2> ();
    *     case 3: f.operator() <3> ();
    *     case 4: f.operator() <4> ();
    *       break;
    *
    *     default: f();
    * }
    * \endcode
    *
    * \tparam CaseList switch case list
    * \tparam Handler handler functor type
    * \tparam Args argument types
    * \param index switch statement index
    * \param handler handler functor
    * \param args functor arguments
    */
    template <typename CaseList, typename Handler, typename Index, typename... Args>
    FRAMEWORK_ALWAYS_INLINE
    void variadic_switch_fallthrough (Handler&& handler, Index&& index, Args&&... args)
    {
        detail::variadic_switch_fallthrough_impl <CaseList>::run(
            std::forward <Handler> (handler),
            std::forward <Index> (index),
            std::forward <Args> (args)...);
    }
}

#include <framework/common/variadic_switch_fallthrough.inl>

// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Variadic switch (return).
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

#include <framework/common/containers/value_container.hpp>

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
        struct variadic_switch_return_impl;
    }

    /**
    * \brief Variadic switch (return).
    *
    * Expands a variadic case list into a switch statement in groups of 
    * FRAMEWORK_VARIADIC_SWITCH_LIMIT elements.  This variant returns the result of the
    * appropriate function call on success - for example, given a limit of 2 the following
    * call to variadic_switch_return:
    *
    * \code
    * return variadic_switch_return <value_list <std::size_t, 0, 1, 2, 3, 4>> (i, f);
    * \endcode
    *
    * is equivalent to the following series of switch statements:
    *
    * \code
    * switch (i)
    * {
    *     case 0: return f.operator() <0> ();
    *     case 1: return f.operator() <1> ();
    *     default:
    *       switch (i)
    *       {
    *           case 2: return f.operator() <2> ();
    *           case 3: return f.operator() <3> ();
    *           default:
    *               switch (i)
    *               {
    *                   case 4: return f.operator <4> ();
    *                   default: return f();
    *               }
    *       }
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
    template <
        typename CaseList,
        typename Handler, 
        typename... Args>
    auto variadic_switch_return (
        typename CaseList::type const& index, 
        Handler&& handler, 
        Args&&... args) ->
    decltype(handler(std::forward <Args> (args)...))
    {
        return detail::variadic_switch_return_impl <CaseList>::run(
            index,
            std::forward <Handler> (handler),
            std::forward <Args> (args)...);
    }
};

#include <framework/common/variadic_switch_return.inl>

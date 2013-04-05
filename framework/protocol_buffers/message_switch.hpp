// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Message switch implementation.
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
#include <string>
#include <array>

#include <boost/preprocessor/config/limits.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/iteration/local.hpp>

#include <framework/common/pack_container.hpp>
#include <framework/common/common_macros.hpp>

#ifndef FRAMEWORK_PROTOCOL_BUFFERS_SWITCH_LIMIT
    /**
    * \brief Switch limit.
    *
    * Defines the maximum number of switch cases to expand - remaining cases are handled recursively.
    */
    #define FRAMEWORK_PROTOCOL_BUFFERS_SWITCH_LIMIT 40
#else
    #if FRAMEWORK_VARIADIC_SWITCH_LIMIT > BOOST_PP_LIMIT_REPEAT
        #error "Variadic switch limit cannot exceed BOOST_PP_LIMIT_REPEAT"
    #endif
#endif

#if FRAMEWORK_PROTOCOL_BUFFERS_SWITCH_LIMIT <= 0
    #error "Invalid variadic switch limit"
#endif

namespace framework
{
    namespace protocol_buffers
    {
        namespace detail
        {
            template <typename RequiredPack, typename SwitchPack>
            struct message_switch;
        }
    }
}

#include <framework/protocol_buffers/message_switch.inl>

// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Common accessors aliases.
*
* \copyright
* Copyright &copy; 2012 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

#include <framework/serializable/utility/interface.hpp>

namespace framework
{
    namespace serializable
    {
        /**
        * \brief Get.
        */
        template <typename Name, typename Object, typename... Args>
        FRAMEWORK_ALWAYS_INLINE
        auto get (Object const& t, Args&&... args) -> decltype(interface <Name> (t).get(std::forward <Args> (args)...))
        {
            return interface <Name> (t).get(std::forward <Args> (args)...);
        }
        
        /**
        * \brief Set.
        */
        template <typename Name, typename Object, typename... Args>
        FRAMEWORK_ALWAYS_INLINE
        auto set (Object& t, Args&&... args) -> decltype(interface <Name> (t).set(std::forward <Args> (args)...))
        {
            return interface <Name> (t).set(std::forward <Args> (args)...);
        }
        
        /**
        * \brief Check.
        */
        template <typename Name, typename Object, typename... Args>
        FRAMEWORK_ALWAYS_INLINE
        auto check (Object const& t, Args&&... args) -> decltype(interface <Name> (t).check(std::forward <Args> (args)...))
        {
            return interface <Name> (t).check(std::forward <Args> (args)...);
        }
        
        /**
        * \brief Clear.
        */
        template <typename Name, typename Object, typename... Args>
        FRAMEWORK_ALWAYS_INLINE
        auto clear (Object& t, Args&&... args) -> decltype(interface <Name> (t).clear(std::forward <Args> (args)...))
        {
            return interface <Name> (t).clear(std::forward <Args> (args)...);
        }
    }
}

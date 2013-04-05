// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Interface helper.
*
* \copyright
* Copyright &copy; 2012 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

#include <framework/serializable/serializable.hpp>

namespace framework
{
    namespace serializable
    {
        /**
        * Returns a reference to the implementation associated with \c Name in \c Object.  This
        * method should be preferred over the get/set/... free functions when marshalling data;
        * implementations may need to protect necessary methods.
        */
        template <typename Name, typename Object>
        FRAMEWORK_ALWAYS_INLINE
        auto interface (Object& t) -> object_base <Object, Name>&
        {
            return static_cast <object_base <Object, Name>&> (t);
        }
 
        /**
        * Returns a constant reference to the implementation associated with \c Name in \c Object.  This
        * method should be preferred over the get/set/... free functions when marshalling data;
        * implementations may need to protect necessary methods.
        */
        template <typename Name, typename Object>
        FRAMEWORK_ALWAYS_INLINE
        auto interface (Object const& t) -> object_base <Object, Name> const&
        {
            return static_cast <object_base <Object, Name> const&> (t);
        }
    }
}

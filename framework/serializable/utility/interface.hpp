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
        * method should be preferred over the get/set/... free functions in \c serializable_specification;
        * implementations may need to protect necessary methods outside of \c serializable_specification or
        * \c serializable_default_specification.
        */
        template <typename Name, typename Object>
        auto interface (Object& t) -> typename get_base <Object, Name>::type&
        {
            return static_cast <typename get_base <Object, Name>::type&> (t);
        }
 
        /**
        * Returns a constant reference to the implementation associated with \c Name in \c Object.  This
        * method should be preferred over the get/set/... free functions in \c serializable_specification;
        * implementations may need to protect necessary methods outside of \c serializable_specification or
        * \c serializable_default_specification.
        */
        template <typename Name, typename Object>
        auto interface (Object const& t) -> typename get_base <Object, Name>::type const&
        {
            return static_cast <typename get_base <Object, Name>::type const&> (t);
        }
    }
}

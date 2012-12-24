// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Serializable alias wrapper.
*
* Commonly used to forward processing to the default implementation:
*
* \code
* template <typename... Specification>
* struct serializable_specification <Wrapper <Specification...>>
* {
*     static bool read (Input& in, Output& out)
*     {
*         // wrapper implementation
*         ...
*
*         // forward to the default serializer
*         return serializable_specification <alias <Specification...>>::read(in, out);
*     }
* }
* \endcode
*
* May also be used to define partial class schematics.  For example, the following
* \c Object definitions are equivalent:
*
* \code
* using partial = alias <
*     value <NAME("Field 1"), int>,
*     value <NAME("Field 2"), float>;
*
* class Object : public serializable <Object,
*     partial,
*     value <NAME("Field 3"), double>>
* {
* };
*
* class Object : public serializable <Object,
*     value <NAME("Field 1"), int>,
*     value <NAME("Field 2"), float>
*     value <NAME("Field 3"), double>>
* {
* };
* \endcode
*
* \copyright
* Copyright &copy; 2012 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

namespace framework
{
    namespace serializable
    {
        /**
        * \struct alias alias.hpp <framework/serializable/alias.hpp>
        * \brief Serializable alias wrapper.
        *
        * Provides a trivial default-serialized wrapper around a specification.
        */
        template <typename... Specification>
        struct alias : public container_type <
            pack_container <Specification...>,
            pack_container <Specification...>>
        {
        };
    }
}

// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Common STL tuple alias.
*
* \copyright
* Copyright &copy; 2012 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

#pragma once

#include <tuple>

#include <framework/serializable/mutators/tuple_container.hpp>

namespace framework
{
    namespace serializable
    {
        /**
        * \brief Common STL tuple specification.
        */
        template <typename... Types>
        using stl_tuple = tuple_container <
            pack_container <Types...>,
            std::tuple <typename type_extractor <Types>::type...>>;
    }
}

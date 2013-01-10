// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Common STL deque alias.
*
* \copyright
* Copyright &copy; 2012 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

#pragma once

#include <deque>

#include <framework/serializable/mutators/variable_container.hpp>

namespace framework
{
    namespace serializable
    {
        /**
        * \brief Common STL deque specification.
        */
        template <typename Size, typename Value>
        using stl_deque = variable_container <Size, Value, std::deque <type_extractor <Value>>>;
    }
}

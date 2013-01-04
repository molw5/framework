// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Common STL vector alias.
*
* \copyright
* Copyright &copy; 2012 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

#include <vector>

#include <framework/serializable/mutators/variable_container.hpp>

namespace framework
{
    namespace serializable
    {
        /**
        * \brief Common STL vector specification.
        */
        template <typename Size, typename Value>
        using stl_vector = variable_container <Size, Value, std::vector <typename type_extractor <Value>::type>, true>;
    }
}

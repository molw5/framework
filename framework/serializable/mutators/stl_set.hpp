// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Common STL set aliases.
*
* \copyright
* Copyright &copy; 2012 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

#pragma once

#include <set>

#include <framework/serializable/mutators/variable_container.hpp>

namespace framework
{
    namespace serializable
    {
        /**
        * \brief Common STL set specification.
        */
        template <typename Size, typename Value>
        using stl_set = variable_container <
            Size, 
            Value,
            std::set <type_extractor <Value>>>;
        
        /**
        * \brief Common STL multiset specification.
        */
        template <typename Size, typename Value>
        using stl_multiset = variable_container <
            Size, 
            Value,
            std::multiset <type_extractor <Value>>>;
    }
}

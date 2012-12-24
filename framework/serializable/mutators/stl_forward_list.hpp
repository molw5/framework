// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Common STL forward list alias.
*
* \copyright
* Copyright &copy; 2012 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

#pragma once

#include <forward_list>

#include <framework/serializable/mutators/variable_container.hpp>

namespace framework
{
    namespace serializable
    {
        /**
        * \brief Common STL forward list specification.
        */
        template <typename Size, typename Value>
        using stl_forward_list = variable_container <Size, Value, std::forward_list <typename type_extractor <Value>::type>>;
    }
}

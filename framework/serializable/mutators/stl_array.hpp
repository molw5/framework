// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Common STL array alias.
*
* \copyright
* Copyright &copy; 2012 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

#include <array>

#include <framework/serializable/mutators/fixed_container.hpp>

namespace framework
{
    namespace serializable
    {
        /**
        * \brief Common STL array specification.
        */
        template <std::size_t Size, typename Value>
        using stl_array = fixed_container <Value, std::array <typename type_extractor <Value>::type, Size>>;
    }
}

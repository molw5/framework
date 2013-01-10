// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Common STL pair alias.
*
* \copyright
* Copyright &copy; 2012 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

#pragma once

#include <utility>

#include <framework/serializable/mutators/tuple_container.hpp>

namespace framework
{
    namespace serializable
    {
        /**
        * \brief Common STL pair specification.
        */
        template <typename T1, typename T2>
        using stl_pair = tuple_container <
            pack_container <T1, T2>, 
            std::pair <type_extractor <T1>, type_extractor <T2>>>;
    }
}

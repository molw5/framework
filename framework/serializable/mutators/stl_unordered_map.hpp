// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Common STL map aliases.
*
* \copyright
* Copyright &copy; 2012 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

#pragma once

#include <unordered_map>

#include <framework/serializable/mutators/variable_container.hpp>
#include <framework/serializable/mutators/stl_pair.hpp>

namespace framework
{
    namespace serializable
    {
        /**
        * \brief Common STL map specification.
        */
        template <typename Size, typename T1, typename T2>
        using stl_unordered_map = variable_container <
            Size, 
            stl_pair <T1, T2>, 
            std::unordered_map <
                typename type_extractor <T1>::type,
                typename type_extractor <T2>::type>>;
        
        /**
        * \brief Common STL multimap specification.
        */
        template <typename Size, typename T1, typename T2>
        using stl_unordered_multimap = variable_container <
            Size, 
            stl_pair <T1, T2>, 
            std::unordered_multimap <
                typename type_extractor <T1>::type,
                typename type_extractor <T2>::type>>;
    }
}

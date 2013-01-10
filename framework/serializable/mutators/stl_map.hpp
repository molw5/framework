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

#include <map>

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
        using stl_map = variable_container <
            Size, 
            stl_pair <T1, T2>, 
            std::map <type_extractor <T1>, type_extractor <T2>>>;
        
        /**
        * \brief Common STL multimap specification.
        */
        template <typename Size, typename T1, typename T2>
        using stl_multimap = variable_container <
            Size, 
            stl_pair <T1, T2>, 
            std::multimap <type_extractor <T1>, type_extractor <T2>>>;
    }
}

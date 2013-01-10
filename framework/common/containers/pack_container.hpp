// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Variadic pack container.
*
* \copyright
* Copyright &copy; 2012 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

namespace framework
{
    namespace detail
    {
        template <typename... Packs>
        struct merge_packs_impl;

        template <typename T, template <typename> class Macher>
        struct filter_pack_impl;
        
        template <typename Pack>
        struct unique_filter_pack_impl;
    }

    /**
    * \headerfile pack_container.hpp <framework/common/containers/pack_container.hpp>
    * \brief Variadic pack container.
    *
    * Simple wrapper around a varidaic template's parameter pack.
    */
    template <typename... Types>
    struct pack_container
    {
        /**
        * \brief Pack container size.
        */
        enum { size = sizeof... (Types) };
    };

    /**
    * \headerfile pack_container.hpp <framework/common/containers/pack_container.hpp>
    * \brief Merge parameter packs.
    *
    * Constructs a parameter pack containing all elements of the pack containers provided
    * in \c Packs, in the order they appear.  For example, the following are equivalent:
    *
    * \code
    * using results = merge_packs <pack_container <int, float>, pack_container <double>>;
    * using results = pack_container <int, float, double>;
    * \endcode
    *
    * \tparam Packs input \c pack_container types
    */
    template <typename... Packs>
    using merge_packs = typename detail::merge_packs_impl <Packs...>::type;

    /**
    * \headerfile pack_container.hpp <framework/common/containers/pack_container.hpp>
    * \brief Parameter pack filter.
    * 
    * Constructs a \c pack_container of elements \c T of \c Pack satisfying
    *
    * \code
    * Matcher <T>::value == true
    * \endcode
    *
    * storing the result int \c type.  For example, the following are equivalent:
    *
    * \code
    * using results = filter_pack <pack_container <int, float, short, double, int>, std::is_integral>;
    * using results = pack_container <int, short, int>;
    * \endcode
    */
    template <typename Pack, template <typename> class Matcher>
    using filter_pack = typename detail::filter_pack_impl <Pack, Matcher>::type;

    /**
    * \headerfile pack_container.hpp <framework/common/containers/pack_container.hpp>
    * \brief Unique parameter pack filter.
    *
    * Obtains the unique element \c T of \c Pack satisfying
    * \code
    * Match <T>::value == true
    * \endcode
    *
    * storing the result in \c type.  For example, the following are equivalent:
    *
    * \code
    * using result = unique_filter_pack <pack_container <short, double, int, long>, std::is_floating_point>;
    * using result = double;
    * \endcode
    */
    template <typename Values, template <typename> class Matcher>
    using unique_filter_pack = typename detail::unique_filter_pack_impl <filter_pack <Values, Matcher>>::type;
}

#include <framework/common/containers/pack_container.inl>

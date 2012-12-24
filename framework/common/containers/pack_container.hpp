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
        template <typename Values>
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
    * using results = typename merge_packs <pack_container <int, float>, pack_container <double>>::type;
    * using results = pack_container <int, float, double>;
    * \endcode
    *
    * \tparam Packs input \c pack_container types
    */
    template <typename... Packs>
#ifndef DOXYGEN
    struct merge_packs;
#else
    struct merge_packs
    {
        /**
        * \brief Result.
        */
        typedef type;
    };
#endif

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
    * using results = typename filter_pack <pack_container <int, float, short, double, int>, std::is_integral>::type;
    * using results = pack_container <int, short, int>;
    * \endcode
    */
    template <typename Pack, template <typename> class Macher>
#ifndef DOXYGEN
    struct filter_pack;
#else
    struct filter_pack
    {
        /**
        * \brief Result.
        */
        typedef type;
    }
#endif

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
    * using result = typename unique_filter_pack <pack_container <short, double, int, long>, std::is_floating_point>::type;
    * using result = double;
    * \endcode
    */
    template <typename Values, template <typename> class Matcher>
    struct unique_filter_pack
    {
        /**
        * \brief Result.
        */
        using type = 
            typename detail::unique_filter_pack_impl <
                typename filter_pack <Values, Matcher>::type
            >::type;
    };
}

#include <framework/common/containers/pack_container.inl>

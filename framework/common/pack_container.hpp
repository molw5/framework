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

#include <cstddef>
#include <type_traits>

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

        template <typename Pack, template <typename> class Matcher, std::size_t Position, std::size_t Index = 0>
        struct find_pack_impl;

        template <typename T>
        struct is_pack_container;

        template <typename Type, std::size_t Size>
        struct make_values_impl;

        template <typename Pack, template <typename, typename> class LessThan>
        struct sort_pack_impl;
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
    *
    * Type trait testing whether or not T is a pack_container.
    */
    template <typename T>
    using is_pack_container = typename detail::is_pack_container <T>::type;

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

    /**
    * \headerfile pack_container.hpp <framework/common/containers/pack_container.hpp>
    * \brief Find type.
    * 
    * Obtains the index of the first element of Pack satisfying:
    *
    * \code
    * index >= Position && Matcher <T>::value == true
    * \endcode
    *
    * storing the result in value.  If no such element exists, std::basic_string::npos is returned instead.
    */
    template <typename Pack, template <typename> class Matcher, std::size_t Position = 0>
    using find_pack = std::integral_constant <std::size_t, detail::find_pack_impl <Pack, Matcher, Position>::value>;

    namespace detail
    {
        template <typename Lhs, typename Rhs>
        struct value_less_than_impl
        {
            using type =
                typename std::conditional <
                    (Lhs::value < Rhs::value),
                    std::true_type,
                    std::false_type
                >::type;
        };
    }

    /**
    * \headerfile pack_container.hpp <framework/common/containers/pack_container.hpp>
    * \brief Value less than.
    *
    * The following are equivalent:
    * \code
    * enum{ result = value_less_than <Lhs, Rhs>::value };
    * enum{ result = Lhs::value < Rhs::value ? true : false };
    * \endcode
    */
    template <typename Lhs, typename Rhs>
    using value_less_than = typename detail::value_less_than_impl <Lhs, Rhs>::type;

    /**
    * \headerfile pack_container.hpp <framework/common/containers/pack_container.hpp>
    * \brief Sort pack.
    *
    * Sorts the pack using the provided less than operator.
    */
    template <typename Pack, template <typename, typename> class LessThan = value_less_than>
    using sort_pack = typename detail::sort_pack_impl <Pack, LessThan>::type;

    /**
    * \headerfile pack_container.hpp <framework/common/containers/pack_container.hpp>
    * \brief Basic case container constructor.
    *
    * Creates a value_container containing the elements [0, Size).  The following are equivalent:
    *
    * \code
    * using result = make_values <std::size_t, 3>;
    * using result = pack_container <
    *     integral_constant <std::size_t, 0>, 
    *     integral_constant <std::size_t, 1>, 
    *     integral_constant <std::size_t, 2>>;
    * \endcode
    */
    template <typename Type, std::size_t Size>
    using make_values = typename detail::make_values_impl <Type, Size>::type;

    /**
    * \headerfile pack_container.hpp <framework/common/containers/pack_container.hpp>
    * \brief Index container constructor.
    *
    * Creates an index_container containing the elements [0, Size).  The following are equivalent:
    *
    * \code
    * using result = make_indices <3>;
    * using result = make_values <std::size_t, 3>;
    * \endcode
    */
    template <std::size_t Size>
    using make_indices = typename detail::make_values_impl <std::size_t, Size>::type;
}

#include <framework/common/pack_container.inl>

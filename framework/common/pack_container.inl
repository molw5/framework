// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

#pragma once

namespace framework
{
    namespace detail
    {
        template <>
        struct merge_packs_impl <>
        {
            using type = pack_container <>;
        };

        template <typename... LhsPack, typename... RhsPack>
        struct merge_packs_impl <pack_container <LhsPack...>, pack_container <RhsPack...>>
        {
            using type = pack_container <LhsPack..., RhsPack...>;
        };

        template <typename Head, typename... Tail>
        struct merge_packs_impl <Head, Tail...>
        {
            using type = merge_packs <Head, merge_packs <Tail...>>;
        };

        template <typename T, template <typename> class Matcher>
        struct filter_pack_impl
        {
            using type =
                typename std::conditional <
                    Matcher <T>::value,
                    pack_container <T>,
                    pack_container <>
                >::type;
        };

        template <typename... Values, template <typename> class Matcher>
        struct filter_pack_impl <pack_container <Values...>, Matcher>
        {
            using type = merge_packs <filter_pack <Values, Matcher>...>;
        };

        template <typename T>
        struct unique_filter_pack_impl <pack_container <T>>
        {
            using type = T;
        };

        template <
            typename Head,
            typename... Tail,
            template <typename> class Matcher,
            std::size_t Position,
            std::size_t Index>
        struct find_pack_impl <
            pack_container <Head, Tail...>,
            Matcher,
            Position,
            Index>
        {
            enum{ next = find_pack_impl <pack_container <Tail...>, Matcher, Position, Index+1>::value };
            enum{ match = Index >= Position && Matcher <Head>::value };
            enum{ value = match ? Index : next };
        };

        template <
            template <typename> class Matcher, 
            std::size_t Position, 
            std::size_t Index>
        struct find_pack_impl <
            pack_container <>,
            Matcher,
            Position,
            Index>
        {
            enum{ value = static_cast <std::size_t> (-1) };
        };

        template <typename T>
        struct is_pack_container
        {
            using type = std::false_type;
        };

        template <typename... Values>
        struct is_pack_container <pack_container <Values...>>
        {
            using type = std::true_type;
        };

        template <typename Type, std::size_t Size>
        struct make_values_impl
        {
            using type = merge_packs <make_values <Type, Size-1>, pack_container <std::integral_constant <Type, Size-1>>>;
        };

        template <typename Type>
        struct make_values_impl <Type, 0>
        {
            using type = pack_container <>;
        };

        template <typename Pack, template <typename, typename> class LessThan>
        struct minimum;

        template <typename Head, typename... Tail, template <typename, typename> class LessThan>
        struct minimum <pack_container <Head, Tail...>, LessThan>
        {
            using next = minimum <pack_container <Tail...>, LessThan>;
            enum{ smallest = LessThan <Head, typename next::type>::value };

            using type = 
                typename std::conditional <
                    smallest,
                    Head,
                    typename next::type
                >::type;

            using remaining =
                typename std::conditional <
                    smallest,
                    merge_packs <pack_container <typename next::type>, typename next::remaining>,
                    merge_packs <pack_container <Head>, typename next::remaining>
                >::type;
        };

        template <typename Head, template <typename, typename> class LessThan>
        struct minimum <pack_container <Head>, LessThan>
        {
            using type = Head;
            using remaining = pack_container <>;
        };

        template <typename Pack, template <typename, typename> class LessThan>
        struct sort_pack_impl
        {
            using current = minimum <Pack, LessThan>;
            using type = merge_packs <
                pack_container <typename current::type>,
                sort_pack <typename current::remaining, LessThan>>;
        };

        template <template <typename, typename> class LessThan>
        struct sort_pack_impl <pack_container <>, LessThan>
        {
            using type = pack_container <>;
        };
    }
}

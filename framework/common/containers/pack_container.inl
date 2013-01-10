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
    }
}

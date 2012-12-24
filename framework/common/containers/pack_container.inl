// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

#pragma once

namespace framework
{
    namespace detail
    {
        template <typename Value, template <typename> class Matcher, bool Result = Matcher <Value>::value>
        struct filter_pack_impl;

        template <typename Value, template <typename> class Matcher>
        struct filter_pack_impl <Value, Matcher, false>
        {
            using type = pack_container <>;
        };

        template <typename Value, template <typename> class Matcher>
        struct filter_pack_impl <Value, Matcher, true>
        {
            using type = pack_container <Value>;
        };

        template <typename Result>
        struct unique_filter_pack_impl <pack_container <Result>>
        {
            using type = Result;
        };
    }

    template <>
    struct merge_packs <>
    {
        using type = pack_container <>;
    };

    template <typename... LhsPack, typename... RhsPack>
    struct merge_packs <pack_container <LhsPack...>, pack_container <RhsPack...>>
    {
        using type = pack_container <LhsPack..., RhsPack...>;
    };

    template <typename Head, typename... Tail>
    struct merge_packs <Head, Tail...>
    {
        using type = typename merge_packs <Head, typename merge_packs <Tail...>::type>::type;
    };

    template <typename... Values, template <typename> class Matcher>
    struct filter_pack <pack_container <Values...>, Matcher>
    {
        using type = typename merge_packs <typename detail::filter_pack_impl <Values, Matcher>::type...>::type;
    };
}

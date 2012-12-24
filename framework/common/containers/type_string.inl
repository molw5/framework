// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

#include <type_traits>

namespace framework
{
    namespace detail
    {
        template <typename... Strings>
        struct merge_type_strings;

        template <char... LhsString, char... RhsString>
        struct merge_type_strings <type_string <LhsString...>, type_string <RhsString...>>
        {
            using type = type_string <LhsString..., RhsString...>;
        };

        template <typename Head, typename... Tail>
        struct merge_type_strings <Head, Tail...>
        {
            using type = 
                typename merge_type_strings <
                    Head, 
                    typename merge_type_strings <Tail...>::type
                >::type;
        };
    }

    template <char Head, char... Tail>
    struct make_type_string <Head, Tail...>
    {
        using type =
            typename detail::merge_type_strings <
                type_string <Head>,
                typename make_type_string <Tail...>::type
            >::type;
    };

    template <char... Tail>
    struct make_type_string <'\0', Tail...>
    {
        using type = type_string <>;
    };

    template <>
    struct make_type_string <>
    {
        static_assert(std::is_same <int, int>::value, "Input character array must be '\0' terminated");
    };
}

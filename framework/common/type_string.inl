// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

#include <type_traits>

namespace framework
{
    namespace detail
    {
        template <typename T>
        struct is_type_string_impl
        {
            using type = std::false_type;
        };

        template <char... Array>
        struct is_type_string_impl <type_string <Array...>>
        {
            using type = std::true_type;
        };

        template <char... Array>
        struct make_type_string_impl;

        template <>
        struct make_type_string_impl <>
        {
            static_assert(std::is_same <int, int>::value, "Input character array must be null terminated");
        };

        template <char... Tail>
        struct make_type_string_impl <'\0', Tail...>
        {
            using type = type_string <>;
        };

        template <char Head, char... Tail>
        struct make_type_string_impl <Head, Tail...>
        {
            using type = merge_strings <type_string <Head>, make_type_string <Tail...>>;
        };

        template <typename... Strings>
        struct merge_strings_impl;

        template <>
        struct merge_strings_impl <>
        {
            using type = type_string <>;
        };

        template <char... LhsString, char... RhsString>
        struct merge_strings_impl <type_string <LhsString...>, type_string <RhsString...>>
        {
            using type = type_string <LhsString..., RhsString...>;
        };

        template <typename Head, typename... Tail>
        struct merge_strings_impl <Head, Tail...>
        {
            using type = merge_strings <Head, merge_strings <Tail...>>;
        };
    }
}

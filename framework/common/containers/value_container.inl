// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

namespace framework
{
    namespace detail
    {
        template <typename T>
        struct is_value_container_impl
        {
            using type = std::false_type;
        };

        template <typename Type, Type... Values>
        struct is_value_container_impl <value_container <Type, Values...>>
        {
            using type = std::true_type;
        };

        template <typename Type, Type... Values>
        struct merge_values_impl <value_container <Type, Values...>>
        {
            using type = value_container <Type, Values...>;
        };

        template <typename Type, Type... LhsValues, Type... RhsValues>
        struct merge_values_impl <value_container <Type, LhsValues...>, value_container <Type, RhsValues...>>
        {
            using type = value_container <Type, LhsValues..., RhsValues...>;
        };

        template <typename Head, typename... Tail>
        struct merge_values_impl <Head, Tail...>
        {
            using type = merge_values <Head, merge_values <Tail...>>;
        };

        template <typename Type, std::size_t Size>
        struct make_values_impl
        {
            using type = merge_values <make_values <Type, Size-1>, value_container <Type, Size-1>>;
        };

        template <typename Type>
        struct make_values_impl <Type, 0>
        {
            using type = value_container <Type>;
        };
    }
}

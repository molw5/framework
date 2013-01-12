// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

#pragma once

namespace framework
{
    namespace detail
    {
        template <typename T>
        struct is_index_container_impl
        {
            using type = std::false_type;
        };

        template <std::size_t... Indices>
        struct is_index_container_impl <index_container <Indices...>>
        {
            using type = std::true_type;
        };

        template <>
        struct merge_indices_impl <>
        {
            using type = index_container <>;
        };

        template <std::size_t... LhsIndices, std::size_t... RhsIndices>
        struct merge_indices_impl <index_container <LhsIndices...>, index_container <RhsIndices...>>
        {
            using type = index_container <LhsIndices..., RhsIndices...>;
        };

        template <typename Head, typename... Tail>
        struct merge_indices_impl <Head, Tail...>
        {
            using type = merge_indices <Head, merge_indices <Tail...>>;
        };

        template <std::size_t Size>
        struct make_indices_impl
        {
            using type = merge_indices <make_indices <Size-1>, index_container <Size-1>>;
        };

        template <>
        struct make_indices_impl <0>
        {
            using type = index_container <>;
        };
    }
}

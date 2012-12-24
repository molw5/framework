// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

#pragma once

namespace framework
{
    namespace detail
    {
        template <typename Lhs, typename Rhs>
        struct merge_indices;
    
        template <std::size_t... LhsIndices, std::size_t... RhsIndices>
        struct merge_indices <index_container <LhsIndices...>, index_container <RhsIndices...>>
        {
            using type = index_container <LhsIndices..., RhsIndices...>;
        };
    }

    template <std::size_t Size>
    struct make_indices
    {
        using type =
            typename detail::merge_indices <
                typename make_indices <Size-1>::type,
                index_container <Size-1>
            >::type;
    };

    template <>
    struct make_indices <0>
    {
        using type = index_container <>;
    };
}

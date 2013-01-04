// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

namespace framework
{
    namespace detail
    {
        template <std::size_t Size, std::size_t... Indices>
        struct make_value_indices_impl <Size, index_container <Indices...>>
        {
            using type = value_container <std::size_t, Indices...>;
        };
    }
}

// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

namespace framework
{
    namespace serializable
    {
        namespace detail
        {
            template <typename... Pack>
            struct make_alias_impl <pack_container <Pack...>>
            {
                using type = alias <Pack...>;
            };
        }
    }
}

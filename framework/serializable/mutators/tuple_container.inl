// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

namespace framework
{
    namespace serializable
    {
        namespace detail
        {
            template <std::size_t Index, typename... Types>
            struct tuple_container_impl;

            template <std::size_t Index>
            struct tuple_container_impl <Index>
            {
                template <typename Input, typename Output>
                static bool read (Input&, Output&)
                {
                    return true;
                }

                template <typename Input, typename Output>
                static bool write (Input const&, Output&)
                {
                    return true;
                }
            };

            template <std::size_t Index, typename Head, typename... Tail>
            struct tuple_container_impl <Index, Head, Tail...>
            {
                template <typename Input, typename Output>
                static bool read (Input& in, Output& out)
                {
                    if (!serializable_specification <Head>::read(in, std::get <Index> (out)))
                        return false;

                    return tuple_container_impl <Index+1, Tail...>::read(in, out);
                }

                template <typename Input, typename Output>
                static bool write (Input const& in, Output& out)
                {
                    if (!serializable_specification <Head>::write (std::get <Index> (in), out))
                        return false;

                    return tuple_container_impl <Index+1, Tail...>::write(in, out);
                }
            };
        }
    }
}

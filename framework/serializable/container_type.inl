// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

namespace framework
{
    namespace serializable
    {
        namespace detail
        {
            template <>
            struct recursive_serializable_specification <pack_container <>>
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

            template <typename Head, typename... Tail>
            struct recursive_serializable_specification <pack_container <Head, Tail...>>
            {
                template <typename Input, typename Output>
                static bool read (Input& in, Output& out)
                {
                    using ::framework::serializable::dispatch_read;
                    if (!dispatch_read <Head> (in, out))
                        return false;

                    return recursive_serializable_specification <pack_container <Tail...>>::read(in, out);
                }

                template <typename Input, typename Output>
                static bool write (Input const& in, Output& out)
                {
                    using ::framework::serializable::dispatch_write;
                    if (!dispatch_write <Head> (in, out))
                        return false;

                    return recursive_serializable_specification <pack_container <Tail...>>::write(in, out);
                }
            };

            template <typename T, typename Enabler>
            struct is_container_type_impl
            {
                using type = std::false_type;
            };

            template <typename T>
            struct is_container_type_impl <T, typename T::serializable_container_enabler>
            {
                using type = std::true_type;
            };

            template <typename T, typename Enabler>
            struct is_container_default_serializable_impl
            {
                using type = std::false_type;
            };

            template <typename T>
            struct is_container_default_serializable_impl <T,
                typename std::enable_if <
                    is_container_type <T>::value,
                    void
                >::type>
            {
                using type =
                    typename std::conditional <
                        T::serilaizable_container_default,
                        std::true_type,
                        std::false_type
                    >::type;
            };

            template <typename T>
            struct container_specification_impl
            {
                using type =
                    typename std::enable_if <
                        is_container_type <T>::value,
                        typename T::serializable_container_specification
                    >::type;
            };

            template <typename T>
            struct container_children_impl
            {
                using type =
                    typename std::enable_if <
                        is_container_type <T>::value,
                        typename T::serializable_container_children
                    >::type;
            };
        }
    }
}

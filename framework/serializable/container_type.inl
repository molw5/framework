// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

#include <framework/common/common_macros.hpp>
#include <framework/common/containers/pack_container.hpp>

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
                    if (!serializable_specification <Head>::read(in, out))
                        return false;

                    return recursive_serializable_specification <pack_container <Tail...>>::read(in, out);
                }

                template <typename Input, typename Output>
                static bool write (Input const& in, Output& out)
                {
                    if (!serializable_specification <Head>::write(in, out))
                        return false;

                    return recursive_serializable_specification <pack_container <Tail...>>::write(in, out);
                }
            };
            
            template <typename T, typename Enabler>
            struct is_container_type_impl : std::false_type
            {
            };

            template <typename T>
            struct is_container_type_impl <T, typename T::serializable_container_enabler> : std::true_type
            {
            };

            template <typename T, typename Enabler>
            struct is_default_container_serializable_impl : std::false_type
            {
            };

            template <typename T>
            struct is_default_container_serializable_impl <T,
                typename std::enable_if <
                    is_container_type <T>::value &&
                    T::container_default,
                    void
                >::type> : std::true_type
            {
            };
        }
        
        template <
            typename... Specification,
            typename... Children>
        struct container_type <
            pack_container <Specification...>,
            pack_container <Children...>,
            true>
        {
// GCC bug workaround - www.open-std.org/jtc1/sc22/wg21/docs/cwg_defects.html#580
#if MAX_GNUC_VERSION(4, 8, 0)
            public:
#else
            template <typename T, typename Enabler>
            friend struct detail::is_container_type_impl;
            
            template <typename T, typename Enabler>
            friend struct detail::is_default_container_serializable_impl;

            template <typename T>
            friend struct is_default_container_serializable;
            
            template <typename T>
            friend struct get_container_specification;
            
            template <typename T>
            friend struct get_container_children;
            
            private:
#endif
                using serializable_container_enabler = void;
                using container_specification = pack_container <Specification...>;
                using container_children = pack_container <Children...>;
                enum {container_default = true};

            private:
                container_type () = default;
                ~container_type () = default;
        };

        template <
            typename... Specification,
            typename... Children>
        struct container_type <
            pack_container <Specification...>,
            pack_container <Children...>,
            false>
        {
// GCC bug workaround - www.open-std.org/jtc1/sc22/wg21/docs/cwg_defects.html#580
#if MAX_GNUC_VERSION(4, 8, 0)
            public:
#else
            template <typename T, typename Enabler>
            friend struct detail::is_container_type_impl;
            
            template <typename T, typename Enabler>
            friend struct detail::is_default_container_serializable_impl;

            template <typename T>
            friend struct is_default_container_serializable;
            
            template <typename T>
            friend struct get_container_specification;
            
            template <typename T>
            friend struct get_container_children;
            
            private:
#endif
                using serializable_container_enabler = void;
                using container_specification = pack_container <Specification...>;
                using container_children = pack_container <Children...>;
                enum {container_default = false};

            private:
                container_type () = default;
                ~container_type () = default;
        };
    }
}

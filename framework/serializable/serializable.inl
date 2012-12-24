// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

#include <framework/serializable/value_type.hpp>
#include <framework/common/common_macros.hpp>

namespace framework
{
    namespace serializable
    {
        namespace detail
        {
            template <template <typename> class Path>
            struct extract_values_impl <pack_container <>, Path>
            {
                using type = pack_container <>;
            };

            template <typename... Children, template <typename> class Path>
            struct extract_values_impl <pack_container <Children...>, Path>
            {
                using type = typename merge_packs <typename extract_values <Children, Path>::type...>::type;
            };

            template <typename T, typename Enabler>
            struct is_derived_type_impl : std::false_type
            {
            };

            template <typename T>
            struct is_derived_type_impl <T, typename T::serializable_derived_enabler> : std::true_type
            {
            };
        }

        template <
            typename Derived,
            typename Specification,
            typename... Inherited,
            typename... Constructed,
            typename... Visible>
        class serializable_implementation <
                Derived,
                Specification,
                pack_container <Inherited...>,
                pack_container <Constructed...>,
                pack_container <Visible...>> : public get_value_implementation <Inherited, Derived>::type...
        {
// GCC bug workaround - www.open-std.org/jtc1/sc22/wg21/docs/cwg_defects.html#580
#if MAX_GNUC_VERSION(4, 8, 0)
            public:
#else
            template <typename T, typename Enabler>
            friend struct detail::is_derived_type_impl;
        
            template <typename T, typename Enabler>
            friend struct serializable_default_specification;

            template <typename T, typename Name>
            friend struct get_base;

            template <typename T>
            friend struct get_visible_names;
            
            private:
#endif
                using serializable_derived_enabler = void;
                using serializable_derived_specification = Specification;
                using serializable_derived_visible_names = pack_container <typename get_value_name <Visible>::type...>;
                
                using serializable_derived_base_map = 
                    pack_container <
                        std::pair <
                            typename get_value_name <Inherited>::type,
                            typename get_value_implementation <Inherited, Derived>::type
                        >...>;

            protected:
                using serializable_base = serializable_implementation <
                    Derived,
                    Specification,
                    pack_container <Inherited...>,
                    pack_container <Constructed...>,
                    pack_container <Visible...>>;
            
            protected:
                ~serializable_implementation () = default;
                serializable_implementation () = default;

                template <typename... Args>
                serializable_implementation (Args&&... args)
                    : get_value_implementation <Constructed, Derived>::type (std::forward <Args> (args))...
                {
                }
        };

        template <typename T, template <typename> class Path>
        struct extract_values <T, Path,
            typename std::enable_if <
                is_container_type <T>::value,
                void
            >::type>
        {
            using type =
                typename detail::extract_values_impl <
                    typename Path <T>::type,
                    Path
                >::type;
        };

        template <typename T, template <typename> class Path>
        struct extract_values <T, Path,
            typename std::enable_if <
                is_value_type <T>::value,
                void
            >::type>
        {
            using type = pack_container <T>;
        };
    }
}

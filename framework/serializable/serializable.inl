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
            template <typename T, typename Enabler>
            struct is_object_type_impl
            {
                using type = std::false_type;
            };

            template <typename T>
            struct is_object_type_impl <T, typename T::serializable_object_enabler>
            {
                using type = std::true_type;
            };

            template <typename T, typename Enabler>
            struct is_object_default_serializable_impl
            {
                using type = std::false_type;
            };

            template <typename T>
            struct is_object_default_serializable_impl <T,
                typename std::enable_if <
                    is_object_type <T>::value,
                    void
                >::type>
            {
                using type = std::true_type;
            };

            template <typename T, typename Name>
            struct object_base_impl
            {
                private:
                    template <typename U>
                    using filter = std::is_same <typename U::first_type, Name>;

                public:
                    using type =
                        typename std::enable_if <
                            is_object_type <T>::value,
                            typename unique_filter_pack <
                                typename T::serializable_object_base_map,
                                filter
                            >::second_type
                        >::type;
            };

            template <typename T>
            struct object_names_impl
            {
                using type =
                    typename std::enable_if <
                        is_object_type <T>::value,
                        typename T::serializable_object_visible_names
                    >::type;
            };

            template <typename T>
            struct object_specification_impl
            {
                using type =
                    typename std::enable_if <
                        is_object_type <T>::value,
                        typename T::serializable_object_specification
                    >::type;
            };

            template <typename T, template <typename> class Path>
            struct extract_values_impl <T, Path,
                typename std::enable_if <
                    is_value_type <T>::value,
                    void
                >::type>
            {
                using type = pack_container <T>;
            };

            template <typename T, template <typename> class Path>
            struct extract_values_impl <T, Path,
                typename std::enable_if <
                    is_container_type <T>::value,
                    void
                >::type>
            {
                using type = typename extract_values_impl <Path <T>, Path>::type;
            };

            template <template <typename> class Path>
            struct extract_values_impl <pack_container <>, Path, void>
            {
                using type = pack_container <>;
            };

            template <typename... Children, template <typename> class Path>
            struct extract_values_impl <pack_container <Children...>, Path, void>
            {
                using type = merge_packs <extract_values <Children, Path>...>;
            };

            template <typename Derived, bool Default>
            struct marshal;

            template <typename Derived>
            struct marshal <Derived, false>
            {
            };

            template <typename Derived>
            struct marshal <Derived, true>
            {
                template <typename Input>
                friend bool custom_read (Input& in, Derived& out)
                {
                    return dispatch_read <object_specification <Derived>> (in, out);
                }

                template <typename Output>
                friend bool custom_write (Derived const& in, Output& out)
                {
                    return dispatch_write <object_specification <Derived>> (in, out);
                }
            };
        }

        template <
            typename Derived,
            typename Specification,
            typename... Inherited,
            typename... Constructed,
            typename... Visible,
            bool Default>
        class serializable_implementation <
                Derived,
                Specification,
                pack_container <Inherited...>,
                pack_container <Constructed...>,
                pack_container <Visible...>,
                Default> 
            : public detail::marshal <Derived, Default>,
              public value_implementation <Inherited, Derived>...
        {
// GCC bug workaround - www.open-std.org/jtc1/sc22/wg21/docs/cwg_defects.html#580
#if MAX_GNUC_VERSION(4, 8, 0)
            public:
#else
            template <typename T, typename Enabler>
            friend struct detail::is_object_type_impl;

            template <typename T, typename Enabler>
            friend struct detail::is_object_default_serializable_impl;

            template <typename T, typename Name>
            friend struct detail::object_base_impl;

            template <typename T>
            friend struct detail::object_names_impl;

            template <typename T>
            friend struct detail::object_specification_impl;

            private:
#endif
                using serializable_object_enabler = void;
                using serializable_object_specification = Specification;
                using serializable_object_visible_names = pack_container <value_name <Visible>...>;
                
                using serializable_object_base_map = 
                    pack_container <
                        std::pair <
                            value_name <Inherited>,
                            value_implementation <Inherited, Derived>
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
                    : value_implementation <Constructed, Derived> (std::forward <Args> (args))...
                {
                }
        };
    }
}

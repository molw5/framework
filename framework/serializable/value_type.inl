// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

#include <framework/common/common_macros.hpp>

namespace framework
{
    namespace serializable
    {
        namespace detail
        {
            template <typename T, typename Enabler>
            struct is_value_type_impl
            {
                using type = std::false_type;
            };

            template <typename T>
            struct is_value_type_impl <T, typename T::serializable_value_enabler>
            {
                using type = std::true_type;
            };

            template <typename T>
            struct is_value_default_serializable_impl
            {
                using type =
                    typename std::enable_if <
                        is_value_type <T>::value,
                        typename std::conditional <
                            T::value_default,
                            std::true_type,
                            std::false_type
                        >::type
                    >::type;
            };

            template <typename T>
            struct value_name_impl
            {
                using type = 
                    typename std::enable_if <
                        is_value_type <T>::value,
                        typename T::value_name
                    >::type;
            };

            template <typename T>
            struct value_specification_impl
            {
                using type =
                    typename std::enable_if <
                        is_value_type <T>::value,
                        typename T::value_specification
                    >::type;
            };

            template <typename T, typename Derived>
            struct value_implementation_impl
            {
                using type =
                    typename std::enable_if <
                        is_value_type <T>::value,
                        typename T::template value_implementation <Derived>
                    >::type;
            };
        }

        template <
            typename Name, 
            typename Specification,
            template <typename> class Interface,
            bool Default>
        struct value_type
        {
// GCC bug workaround - www.open-std.org/jtc1/sc22/wg21/docs/cwg_defects.html#580
#if MAX_GNUC_VERSION(4, 8, 0)
            public:
#else
            template <typename T, typename Enabler>
            friend struct detail::is_value_type_impl;

            template <typename T>
            friend struct detail::is_value_default_serializable_impl;

            template <typename T>
            friend struct detail::value_name_impl;

            template <typename T>
            friend struct detail::value_specification_impl;

            template <typename T, typename Derived>
            friend struct detail::value_implementation_impl;

            private:
#endif
                using serializable_value_enabler = void;
                using value_name = Name;
                using value_specification = Specification;
                enum {value_default = Default};

                template <typename T>
                using value_implementation = Interface <T>;

                value_type () = delete;
                ~value_type () = delete;
        };
    }
}

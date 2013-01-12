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
                            T::serializable_value_default,
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
                        typename T::serializable_value_name
                    >::type;
            };

            template <typename T>
            struct value_specification_impl
            {
                using type =
                    typename std::enable_if <
                        is_value_type <T>::value,
                        typename T::serializable_value_specification
                    >::type;
            };

            template <typename T, typename Derived>
            struct value_implementation_impl
            {
                using type =
                    typename std::enable_if <
                        is_value_type <T>::value,
                        typename T::template serializable_value_implementation <Derived>
                    >::type;
            };
        }
    }
}

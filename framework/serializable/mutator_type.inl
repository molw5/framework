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
            struct is_mutator_type_impl
            {
                using type = std::false_type;
            };

            template <typename T>
            struct is_mutator_type_impl <T, typename T::serializable_mutator_type_enabler>
            {
                using type = std::true_type;
            };

            template <typename T>
            struct mutator_child_impl
            {
                using type =
                    typename std::enable_if <
                        is_mutator_type <T>::value,
                        typename T::mutator_child
                    >::type;
            };

            template <typename T, typename Enabler>
            struct type_extractor_impl
            {
                using type = T;
            };

            template <typename T>
            struct type_extractor_impl <T,
                typename std::enable_if <
                    is_mutator_type <T>::value,
                    void
                >::type>
            {
                using type =
                    typename type_extractor_impl <
                        mutator_child <T>
                    >::type;
            };
        }
    }
}

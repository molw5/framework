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
                    typename type_extractor <
                        typename get_mutator_child <T>::type
                    >::type;
            };

            template <typename T, typename Enabler>
            struct is_mutator_type_impl : std::false_type
            {
            };

            template <typename T>
            struct is_mutator_type_impl <T,
                typename T::serializable_mutator_type_enabler> :
                    std::true_type
            {
            };
        }

        template <typename Child>
        struct mutator_type
        {
// GCC bug workaround - www.open-std.org/jtc1/sc22/wg21/docs/cwg_defects.html#580
#if MAX_GNUC_VERSION(4, 8, 0)
            public:
#else
            template <typename T, typename Enabler>
            friend struct detail::is_mutator_type_impl;
            
            template <typename T>
            friend struct get_mutator_child;

            private:
#endif
                using serializable_mutator_type_enabler = void;
                using mutator_child = Child;

            private:
                mutator_type () = default;
                ~mutator_type () = default;
        };
    }
}

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
            struct is_value_type_impl : std::false_type
            {
            };

            template <typename T>
            struct is_value_type_impl <T,
                typename T::serializable_value_enabler> :
                    std::true_type
            {
            };
            
            template <typename T, typename Enabler>
            struct is_default_value_serializable_impl : std::false_type
            {
            };

            template <typename T>
            struct is_default_value_serializable_impl <T,
                typename std::enable_if <
                    is_value_type <T>::value,
                    void
                >::type>
            {
                enum { value = T::value_default };
            };
        }

        template <
            typename Name, 
            typename Specification,
            template <typename> class Interface>
        struct value_type <
            Name, 
            Specification,
            Interface, 
            true>
        {
// GCC bug workaround - www.open-std.org/jtc1/sc22/wg21/docs/cwg_defects.html#580
#if MAX_GNUC_VERSION(4, 8, 0)
            public:
#else
            template <typename T, typename Enabler>
            friend struct detail::is_value_type_impl;
    
            template <typename T, typename Enabler>
            friend struct detail::is_default_value_serializable_impl;

            template <typename T>
            friend struct get_value_name;
    
            template <typename T>
            friend struct get_value_specification;
    
            template <typename T, typename Derived>
            friend struct get_value_implementation;
    
            private:
#endif
                using serializable_value_enabler = void;
                using value_name = Name;
                using value_specification = Specification;
                enum {value_default = true};

                template <typename T>
                using value_implementation = Interface <T>;

            private:
                value_type () = default;
                ~value_type () = default;
        };
        
        template <
            typename Name, 
            typename Specification,
            template <typename> class Interface>
        struct value_type <
            Name, 
            Specification,
            Interface, 
            false>
        {
// GCC bug workaround - www.open-std.org/jtc1/sc22/wg21/docs/cwg_defects.html#580
#if MAX_GNUC_VERSION(4, 8, 0)
            public:
#else
            template <typename T, typename Enabler>
            friend struct detail::is_value_type_impl;
    
            template <typename T, typename Enabler>
            friend struct detail::is_default_value_serializable_impl;

            template <typename T>
            friend struct get_value_name;
    
            template <typename T>
            friend struct get_value_specification;
    
            template <typename T, typename Derived>
            friend struct get_value_implementation;
    
            private:
#endif
                using serializable_value_enabler = void;
                using value_name = Name;
                using value_specification = Specification;
                enum {value_default = false};

                template <typename T>
                using value_implementation = Interface <T>;

            private:
                value_type () = default;
                ~value_type () = default;
        };
    }
}

// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

#include <framework/serializable/utility/interface.hpp>
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
        
        template <
            typename Name, 
            typename Specification,
            template <typename> class Interface,
            typename Input,
            typename Output>
        bool read_dispatch (
            value_type <Name, Specification, Interface, true>*,
            Input&& in, Output&& out)
        {
            type_extractor <Specification> value{};
            if (!dispatch_read <Specification> (in, value))
                return false;

            interface <Name> (out).set(std::move(value));
            return true;
        }

        template <
            typename Name, 
            typename Specification,
            template <typename> class Interface,
            typename Input,
            typename Output>
        bool write_dispatch (
            value_type <Name, Specification, Interface, true>*,
            Input&& in, Output&& out)
        {
            auto const& value = interface <Name> (in).get();
            if (!dispatch_write <Specification> (value, out))
                return false;

            return true;
        }
    }
}

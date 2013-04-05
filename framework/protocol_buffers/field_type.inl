// Copyright (C) 2013 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

namespace framework
{
    namespace protocol_buffers
    {
        namespace detail
        {
            template <typename T, typename Enabler>
            struct is_field_type_impl
            {
                using type = std::false_type;
            };

            template <typename T>
            struct is_field_type_impl <T, typename T::protocol_buffers_field_enabler>
            {
                using type = std::true_type;
            };

            template <typename T>
            struct field_number_impl
            {
                using type =
                    typename std::enable_if <
                        is_field_type <T>::value,
                        typename T::protocol_buffers_field_number
                    >::type;
            };

            template <typename T>
            struct field_type_impl
            {
                using type =
                    typename std::enable_if <
                        is_field_type <T>::value,
                        typename T::protocol_buffers_field_type
                    >::type;
            };

            template <typename T>
            struct is_required_impl
            {
                using type =
                    typename std::conditional <
                        field_type <T>::value == e_field_type::required,
                        std::true_type,
                        std::false_type
                    >::type;
            };

            template <typename T>
            struct is_repeated_impl
            {
                using type =
                    typename std::conditional <
                        field_type <T>::value == e_field_type::repeated,
                        std::true_type,
                        std::false_type
                    >::type;
            };

            template <typename T>
            struct is_optional_impl
            {
                using type =
                    typename std::conditional <
                        field_type <T>::value == e_field_type::optional,
                        std::true_type,
                        std::false_type
                    >::type;
            };
        }
    }
}

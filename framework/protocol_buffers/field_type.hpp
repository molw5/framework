// Copyright (C) 2013 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Basic field type definitions.
*
* \copyright
* Copyright &copy; 2013 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

namespace framework
{
    namespace protocol_buffers
    {
        namespace detail
        {
            template <typename T, typename Enabler = void>
            struct is_field_type_impl;

            template <typename T>
            struct field_number_impl;

            template <typename T>
            struct field_type_impl;

            template <typename T>
            struct is_required_impl;

            template <typename T>
            struct is_repeated_impl;

            template <typename T>
            struct is_optional_impl;
        };

        /**
        * \brief Field type constants.
        */
        enum class e_field_type
        {
            optional,
            required,
            repeated
        };

        /**
        * \brief Common field type base.
        */
        template <typename Type, typename Number>
        struct field_base
        {
// GCC bug workaround - www.open-std.org/jtc1/sc22/wg21/docs/cwg_defects.html#580
#if MAX_GNUC_VERSION(4, 8, 0)
            public:
#else
            template <typename T, typename Enabler>
            friend struct detail::is_field_type_impl;

            template <typename T>
            friend struct detail::field_number_impl;

            template <typename T>
            friend struct detail::field_type_impl;

            private:
#endif
                using protocol_buffers_field_enabler = void;
                using protocol_buffers_field_number = Number;
                using protocol_buffers_field_type = Type;
        };

        /**
        * \brief Is field type.
        */
        template <typename T>
        using is_field_type = typename detail::is_field_type_impl <T>::type;

        /**
        * \brief Is required.
        */
        template <typename T>
        using is_required = typename detail::is_required_impl <T>::type;

        /**
        * \brief Is optional.
        */
        template <typename T>
        using is_optional = typename detail::is_optional_impl <T>::type;

        /**
        * \brief Is repeated.
        */
        template <typename T>
        using is_repeated = typename detail::is_repeated_impl <T>::type;

        /**
        * \brief Field number.
        */
        template <typename T>
        using field_number = typename detail::field_number_impl <T>::type;

        /**
        * \brief Field type.
        */
        template <typename T>
        using field_type = typename detail::field_type_impl <T>::type;

        /**
        * \brief Field less than.
        *
        * Compares the field number associated to Lhs with that of Rhs.  This expression evaluates to std::true_type
        * iff field_number <Lhs>::value < field_number <Rhs>::value - otherwise, evaluates to std::false_type.
        */
        template <typename Lhs, typename Rhs>
        using field_less_than = value_less_than <field_number <Lhs>, field_number <Rhs>>;
        
        /**
        * \brief Field matcher.
        *
        * Provides a child type (field_matcher <...>::type) that evaluates to std::true_type iff 
        * field_number <T>::value == Number - otherwise, evaluates to std::false_type.
        */
        template <uint64_t Number>
        struct field_matcher
        {
            template <typename T>
            using type =
                typename std::conditional <
                    field_number <T>::value == Number,
                    std::true_type,
                    std::false_type
                >::type;
        };
    }
}

#include <framework/protocol_buffers/field_type.inl>

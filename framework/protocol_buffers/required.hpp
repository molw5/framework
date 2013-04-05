// Copyright (C) 2013 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Required field type.
*
* \copyright
* Copyright &copy; 2013 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

#include <framework/protocol_buffers/base_types.hpp>
#include <framework/protocol_buffers/field_type.hpp>

namespace framework
{
    namespace protocol_buffers
    {
        /**
        * \brief Default required implementation.
        *
        * Provides a default value container implementation.
        *
        * \tparam T instantiation template definitions
        */
        template <typename T>
        struct default_required_value
        {
            private:
                using value_type = typename T::value_type;
                using reference_type =
                    typename std::conditional <
                        std::is_scalar <value_type>::value,
                        value_type,
                        value_type const&
                    >::type;

            public:
                default_required_value () = default;
                
                default_required_value (value_type value)
                    : p_tValue {std::move(value)}
                {
                }

                default_required_value (std::tuple <>)
                    : default_required_value ()
                {
                }
                
                template <typename... Args>
                default_required_value (std::tuple <Args...>&& args)
                    : default_required_value (
                        std::forward <std::tuple <Args...>> (args), 
                        static_cast <make_indices <sizeof... (Args)>*> (nullptr))
                {
                }

            private:
                template <typename... Args, typename... Indices>
                default_required_value (std::tuple <Args...> args, pack_container <Indices...>*)
                    : p_tValue{std::forward <Args> (std::get <Indices::value> (args))...}
                {
                }

            public:
                FRAMEWORK_ALWAYS_INLINE
                reference_type get () const
                {
                    return p_tValue;
                }

                FRAMEWORK_ALWAYS_INLINE
                void set (value_type value)
                {
                    p_tValue = std::move(value);
                }

                FRAMEWORK_ALWAYS_INLINE
                value_type& mutate ()
                {
                    return p_tValue;
                }

            protected:
                ~default_required_value () = default;

            protected:
                value_type p_tValue {};
        };

        /**
        * \brief Required field type.
        */
        template <
            typename Name, 
            uint32_t Number,
            typename Specification,
            template <typename> class Implementation = default_required_value>
        struct required : 
            public field_base <
                std::integral_constant <e_field_type, e_field_type::required>,
                std::integral_constant <uint32_t, Number>>,
            public serializable::value_type <Name, Specification,
                protocol_buffer_implementation_wrapper <
                    Name,
                    std::integral_constant <uint32_t, Number>,
                    Specification,
                    Implementation
                >::template type,
                false>
        {
        };
    }
}

#include <framework/protocol_buffers/required.inl>

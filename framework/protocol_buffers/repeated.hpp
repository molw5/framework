// Copyright (C) 2013 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Repeated field type.
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
        * \brief Default repeated implementation.
        *
        * Provides a default value container implementation.
        *
        * \tparam T instantiation template definitions
        */
        template <typename T>
        struct default_repeated_value
        {
            private:
                using container_type = std::vector <typename T::value_type>;

            public:
                using value_type = typename T::value_type;
                using const_iterator = typename container_type::const_iterator;

            public:
                default_repeated_value () = default;
                
                default_repeated_value (value_type value)
                    : p_tValue {std::move(value)}
                {
                }

                default_repeated_value (std::tuple <>)
                    : default_repeated_value ()
                {
                }
                
                template <typename... Args>
                default_repeated_value (std::tuple <Args...>&& args)
                    : default_repeated_value (
                        std::forward <std::tuple <Args...>> (args), 
                        static_cast <make_indices <sizeof... (Args)>*> (nullptr))
                {
                }

            private:
                template <typename... Args, typename... Indices>
                default_repeated_value (std::tuple <Args...> args, pack_container <Indices...>*)
                    : p_tValue{std::forward <Args> (std::get <Indices::value> (args))...}
                {
                }

            public:
                FRAMEWORK_ALWAYS_INLINE
                container_type const& get () const
                {
                    return p_tValue;
                }

                FRAMEWORK_ALWAYS_INLINE
                void set (container_type value)
                {
                    p_tValue = std::move(value);
                }

                FRAMEWORK_ALWAYS_INLINE
                container_type& mutate ()
                {
                    return p_tValue;
                }

            protected:
                ~default_repeated_value () = default;

            protected:
                container_type p_tValue {};
        };

        /**
        * \brief Repeated value type.
        */
        template <
            typename Name, 
            uint32_t Number,
            typename Specification,
            template <typename> class Implementation = default_repeated_value>
        struct repeated : 
            public field_base <
                std::integral_constant <e_field_type, e_field_type::repeated>,
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

#include <framework/protocol_buffers/repeated.inl>

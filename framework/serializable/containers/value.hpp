// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Basic value type.
*
* \copyright
* Copyright &copy; 2012 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

#include <tuple>

#include <framework/serializable/base_types.hpp>
#include <framework/serializable/mutator_type.hpp>
#include <framework/serializable/container_type.hpp>
#include <framework/common/containers/index_container.hpp>

namespace framework
{
    namespace serializable
    {
        /**
        * \headerfile value.hpp <framework/serializable/containers/value.hpp>
        * \brief Implementation wrapper.
        *
        * Provides a wrapper around an implementation template, used to forward
        * the relevant template parameters.
        */
        template <
            typename Name,
            typename Specification,
            template <typename> class Implementation>
        struct value_implementation_wrapper
        {
            /**
            * \brief Parameter pack.
            *
            * Passed to the implementation template through this wrapper.
            */
            template <typename Derived>
            struct parameters
            {
                /**
                * \brief Value name.
                */
                using value_name = Name;

                /**
                * \brief Value type.
                */
                using value_type = type_extractor <Specification>;

                /**
                * \brief Value specification.
                */
                using value_specification = Specification;

                /**
                * \brief Derived class.
                */
                using derived = Derived;

                /**
                * \brief Implementation class.
                */
                using implementation = Implementation <parameters <Derived>>;
            };

            /**
            * \brief Wrapper template.
            *
            * Provides the implementation with the required parameter pack.
            */
            template <typename Derived>
            using type = Implementation <parameters <Derived>>;
        };

        /**
        * \headerfile value.hpp <framework/serializable/containers/value.hpp>
        * \brief Default implementation.
        *
        * Provides a default value container implementation.
        *
        * \tparam T instantiation template definitions
        */
        template <typename T>
        struct default_value
        {
            private:
                /**
                * \brief Value type alias.
                */
                using value_type = typename T::value_type;

            public:
                /**
                * \brief Get value.
                */
                value_type const& get () const
                {
                    return p_tValue;
                }

                /**
                * \brief Set value.
                */
                void set (value_type value)
                {
                    p_tValue = std::move(value);
                }

            protected:
                /**
                * \brief Default destructor.
                */
                ~default_value () = default;

                /**
                * \brief Default constructor.
                */
                default_value ()
                    : p_tValue()
                {
                }

                /**
                * \brief Value constructor.
                */
                default_value (value_type value)
                    : p_tValue(std::move(value))
                {
                }

                /**
                * \brief Default constructor alias.
                *
                * Overrides the in-place constructor for the empty tuple type, delegating
                * construction to the default constructor.
                */
                default_value (std::tuple <>)
                    : default_value ()
                {
                }

                /**
                * \brief In-place constructor.
                *
                * Expands the input tuple into the contained value's constructor.  That is,
                * the value is initialized as:
                *
                * \code
                * value_type {std::get <0> (args), std::get <1> (args), ... }
                * \endcode
                *
                * \param args value constructor arguments
                */
                template <typename... Args>
                default_value (std::tuple <Args&&...>&& args)
                    : default_value (
                        std::forward <std::tuple <Args&&...>> (args), 
                        static_cast <make_indices <sizeof... (Args)>*> (nullptr))
                {
                }

            private:
                template <typename... Args, std::size_t... Indices>
                default_value (std::tuple <Args&&...> args, index_container <Indices...>*)
                    : p_tValue{std::forward <Args> (std::get <Indices> (args))...}
                {
                }

            protected:
                /**
                * \brief Container value.
                */
                value_type p_tValue;
        };

        /**
        * \headerfile value.hpp <framework/serializable/containers/value.hpp>
        * \brief Value container.
        *
        * Defines a basic value container type.
        *
        * \tparam Name unique value identifier
        * \tparam Specification value type specification, used to serialize the underlying value type
        * \tparam Implementation value type implementation, inherited by \c serializable_implementation
        */
        template <
            typename Name,
            typename Specification,
            template <typename> class Implementation = default_value>
        struct value : public value_type <
            Name,
            Specification,
            value_implementation_wrapper <
                Name,
                Specification,
                Implementation
            >::template type>
        {
        };
    }
}

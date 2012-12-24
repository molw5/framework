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
                using value_type = typename type_extractor <Specification>::type;

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
        struct value_implementation
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
                ~value_implementation () = default;

                /**
                * \brief Default constructor.
                */
                value_implementation ()
                    : p_tValue()
                {
                }

                /**
                * \brief Value constructor.
                */
                value_implementation (value_type value)
                    : p_tValue(std::move(value))
                {
                }

                /**
                * \brief Default constructor alias.
                *
                * Overrides the in-place constructor for the empty tuple type, delegating
                * construction to the default constructor.
                */
                value_implementation (std::tuple <>)
                    : value_implementation ()
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
                value_implementation (std::tuple <Args&&...>&& args)
                    : value_implementation (
                        std::forward <std::tuple <Args&&...>> (args), 
                        static_cast <typename make_indices <sizeof... (Args)>::type*> (nullptr))
                {
                }

            private:
                template <typename... Args, std::size_t... Indices>
                value_implementation (std::tuple <Args&&...> args, index_container <Indices...>*)
                    : p_tValue{std::forward <Args> (std::get <Indices> (args))...}
                {
                }

            protected:
                /**
                * \brief Container value.
                */
                value_type p_tValue {};
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
            template <typename> class Implementation = value_implementation>
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

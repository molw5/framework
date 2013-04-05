// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Inline object wrapper.
*
* \copyright
* Copyright &copy; 2012 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

#include <framework/serializable/serializable.hpp>
#include <framework/serializable/utility/comparable.hpp>
#include <framework/serializable/common_macros.hpp>

namespace framework
{
    namespace serializable
    {
        /**
        * \headerfile inline_object.hpp <framework/serializable/inline_object.hpp>
        * \brief Inline object wrapper.
        *
        * Provides a simple wrapper around a \c serializable_implementation, allowing
        * container value types to be naturally defined in a serializable type as follows:
        *
        * \code
        * class Object : serializable <Object,
        *     value <NAME("Field 1"), little_endian <float>>,
        *     value <NAME("Field 2"), little_endian <double>>,
        *     value <NAME("Field 3"), stl_vector <little_endian <int>, inline_object <
        *         value <NAME("Value Field 1"), little_endian <int>>,
        *         value <NAME("Value Field 2"), little_endian <double>>>>>,
        *     value <NAME("field 4"), stl_vector <little_endian <int>, little_endian <int>>>>
        * {
        * };
        * \endcode
        *
        * Aliases are provided by the wrapper for the most common accessor methods, 
        * get, set, check, and clear allowing nested access similar to
        *
        * \code
        * Object x;
        * get <NAME("Value Field 2")> (get <NAME("Field 3")> (x)[0]);
        * \endcode
        *
        * the following equivalent syntax
        *
        * \code
        * Object x;
        * get <NAME("Field 3")> (x)[0].get <NAME("Value Field 2")> ();
        * \endcode
        */
        template <typename... Types>
        class inline_object : 
            public serializable <inline_object <Types...>, Types...>,
            public comparable <inline_object <Types...>>
        {
            private:
                DEFINE_BASE_TEMPLATE(inline_object <Types...>);

            public:
                inline_object () = default;
                inline_object (inline_object const&) = default;
                inline_object (inline_object&&) = default;
                inline_object& operator= (inline_object const&) = default;
                inline_object& operator= (inline_object&&) = default;

                /**
                * \brief Base constructor forwarder.
                */
                template <typename... Args>
                inline_object (Args&&... args)
                    : serializable <inline_object <Types...>, Types...> (std::forward <Args> (args)...)
                {
                }

                /**
                * \brief Explicit default constructor overload.
                */
                inline_object (std::tuple <>)
                    : inline_object ()
                {
                }
                
                /**
                * \brief Tuple constructor.
                */
                template <typename... Args>
                inline_object (std::tuple <Args...>&& args)
                    : inline_object (
                        std::forward <std::tuple <Args...>> (args), 
                        static_cast <make_indices <sizeof... (Args)>*> (nullptr))
                {
                }

            private:
                template <typename... Args, typename... Indices>
                inline_object (std::tuple <Args...> args, pack_container <Indices...>*)
                    : serializable <inline_object <Types...>, Types...> (std::forward <Args> (std::get <Indices::value> (args))...)
                {
                }

           public:
                /**
                * \brief Get forwarder.
                */
                template <typename Name, typename... Args>
                auto get (Args&&... args) const ->
                decltype(std::declval <base <Name>> ().get(std::forward <Args> (args)...))
                {
                    return base <Name>::get(std::forward <Args> (args)...);
                }

                /**
                * \brief Set forwarder.
                */
                template <typename Name, typename... Args>
                auto set (Args&&... args) ->
                decltype(std::declval <base <Name>> ().set(std::forward <Args> (args)...))
                {
                    return base <Name>::set(std::forward <Args> (args)...);
                }

                /**
                * \brief Check forwarder.
                */
                template <typename Name, typename... Args>
                auto check (Args&&... args) const ->
                decltype(std::declval <base <Name>> ().check(std::forward <Args> (args)...))
                {
                    return base <Name>::check(std::forward <Args> (args)...);
                }

                /**
                * \brief Clear forwarder.
                */
                template <typename Name, typename... Args>
                auto clear (Args&&... args) ->
                decltype(std::declval <base <Name>> ().clear(std::forward <Args> (args)...))
                {
                    return base <Name>::clear(std::forward <Args> (args)...);
                }
        };
    }
}

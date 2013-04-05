// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Tuple container.
*
* \copyright
* Copyright &copy; 2012 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

#include <framework/serializable/mutator_type.hpp>
#include <framework/serializable/base_types.hpp>

namespace framework
{
    namespace serializable
    {
        namespace detail
        {
            template <std::size_t Index, typename... Types>
            struct tuple_container_impl;
        }

        /**
        * \headerfile tuple_container.hpp <framework/serializable/mutators/tuple_container.hpp>
        *
        * Serializes a tuple-like container using a provided list of value type specifications.
        * 
        * \note
        * The size of \c Container may exceed the number of arguments in the parameter pack;
        * any additional arguments will be ignored during serialization.
        *
        * \tparam Specification \c pack_container of values types, defining container element serialization
        * \tparam Container container type
        */
        template <typename Specification, typename Container>
        struct tuple_container : public mutator_type <Container>
        {
        };

        /**
        * \headerfile tuple_container.hpp <framework/serializable/mutators/tuple_container.hpp>
        * \brief Read overload.
        *
        * Reads the container's elements from an input stream using the specification list \c Parameters.
        *
        * \param in input stream
        * \param out output container
        * \return true on success, false on failure
        */
        template <
            typename... Parameters,
            typename Container,
            typename Input,
            typename Output>
        bool read_dispatch (
            tuple_container <pack_container <Parameters...>, Container>*,
            Input& in, Output& out)
        {
            Container result;
            if (!detail::tuple_container_impl <0, Parameters...>::read(in, result))
                return false;

            out = std::move(result);
            return true;
        }

        /**
        * \headerfile tuple_container.hpp <framework/serializable/mutators/tuple_container.hpp>
        * \brief Write overload.
        *
        * Wries the container's elements to an output stream using the specification list \c Parameters.
        *
        * \param in input container
        * \param out output stream
        * \return true on success, false on failure
        */
        template <
            typename... Parameters,
            typename Container,
            typename Input,
            typename Output>
        bool write_dispatch (
            tuple_container <pack_container <Parameters...>, Container>*,
            Input const& in, Output& out)
        {
            return detail::tuple_container_impl <0, Parameters...>::write(in, out);
        }
    }
}

#include <framework/serializable/mutators/tuple_container.inl>

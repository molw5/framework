// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Fixed size container.
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
        /**
        * \headerfile fixed_container.hpp <framework/serializable/mutators/fixed_contain.hpp>
        *
        * Serializes a fixed size container using the value type specification Value.
        *
        * \tparam Value value specification
        * \tparam Container container type
        */
        template <typename Value, typename Container>
        struct fixed_container : public mutator_type <Container>
        {
        };
        
        /**
        * \headerfile fixed_container.hpp <framework/serializable/mutators/fixed_contain.hpp>
        * \brief Read overload.
        * \param in input stream
        * \param out output container
        * \return true on success, false on failure
        */
        template <
            typename Value,
            typename Container,
            typename Input,
            typename Output>
        bool read_dispatch (
            fixed_container <Value, Container>*,
            Input& in, Output& out)
        {
            Container result;
            for (auto& x : result)
                if (!dispatch_read <Value> (in, x))
                    return false;

            out = std::move(result);
            return true;
        }

        /**
        * \headerfile fixed_container.hpp <framework/serializable/mutators/fixed_contain.hpp>
        * \brief Write overload.
        * \param in input container
        * \param out output stream
        * \return true on success, false on failure
        */
        template <
            typename Value,
            typename Container,
            typename Input,
            typename Output>
        bool write_dispatch (
            fixed_container <Value, Container>*,
            Input const& in, Output& out)
        {
            for (auto const& x : in)
                if (!dispatch_write <Value> (x, out))
                    return false;

            return true;
        }
    }
}

// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Value terminated container.
*
* \copyright
* Copyright &copy; 2012 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

#include <cassert>

#include <framework/serializable/mutator_type.hpp>
#include <framework/serializable/base_types.hpp>

namespace framework
{
    namespace serializable
    {
        /**
        * \headerfile terminated_container.hpp <framework/serializable/mutators/terminated_container.hpp>
        *
        * Serializes a fixed-size container using the value-type specification Value.
        *
        * \tparam Value value specification
        * \tparam Terminator terminating value
        * \tparam Container container type
        */
        template <
            typename Value,
            type_extractor <Value> Terminator,
            typename Container>
        struct terminated_container : public mutator_type <Container>
        {
        };

        /**
        * \headerfile terminated_container.hpp <framework/serializable/mutators/terminated_container.hpp>
        * \brief Read overload.
        *
        * Reads elements into \c out from the input stream using the specification given by \c Value 
        * until the value read matches \c Terminator.  The terminating character is not added to \c out.
        *
        * \param in input stream
        * \param out output container
        * \return true on success, false on failure
        */
        template <
            typename Input,
            typename Output,
            typename Value,
            type_extractor <Value> Terminator,
            typename Container>
        bool dispatch_read (Input& in, Output& out,
            terminated_container <Value, Terminator, Container>*,
            typename std::enable_if <
                std::is_same <Output, Container>::value,
                void
            >::type* = nullptr)
        {
            type_extractor <Value> x;
            if (!dispatch_read <Value> (in, x))
                return false;

            Container result;
            auto inserter = std::insert_iterator <Container> (result, result.begin());
            while (x != Terminator)
            {
                inserter = std::move(x);

                if (!dispatch_read <Value> (in, x))
                    return false;
            }

            out = std::move(result);
            return true;
        }

        /**
        * \headerfile terminated_container.hpp <framework/serializable/mutators/terminated_container.hpp>
        * \brief Write overload.
        *
        * Writes elements to the output stream from \c in using the specification given by \c Value 
        * until the value matches \c Terminator.  The terminating character is not added to \c out.
        *
        * \pre \f$x \ne \textbf{Terminator}, \forall x \in \textbf{in}\f$
        * \param in input container
        * \param out output stream
        * \return true on success, false on failure
        */
        template <
            typename Input,
            typename Output,
            typename Value,
            type_extractor <Value> Terminator,
            typename Container>
        bool dispatch_write (Input const& in, Output& out,
            terminated_container <Value, Terminator, Container>*,
            typename std::enable_if <
                std::is_same <Input, Container>::value,
                void
            >::type* = nullptr)
        {
            for (auto const& x : in)
            {
                assert(x != Terminator);
                if (!dispatch_write <Value> (x, out))
                    return false;
            }

            if (!dispatch_write <Value> (Terminator, out))
                return false;

            return true;
        }
    }
}

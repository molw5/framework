// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Variable size container.
*
* \copyright
* Copyright &copy; 2012 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

#include <iterator>

#include <framework/serializable/mutator_type.hpp>
#include <framework/serializable/base_types.hpp>

namespace framework
{
    namespace serializable
    {
        /**
        * \headerfile variable_container.hpp <framework/serializable/mutators/variable_container.hpp>
        *
        * Serializes a container as a size-delimited list.
        *
        * \tparam Size container size specification
        * \tparam Value container value specification
        * \tparam Container container value type
        * \tparam Continuous indicates the container stores it's values as a continuous block
        */
        template <
            typename Size, 
            typename Value, 
            typename Container,
            bool Continuous = false>
        struct variable_container : public mutator_type <Container>
        {
        };

        /**
        * \headerfile variable_container.hpp <framework/serializable/mutators/variable_container.hpp>
        * \brief Read overload.
        *
        * Reads the container from an input stream as a size delimited list.
        *
        * \param in input stream
        * \param out output container
        * \return true on success, false on failure
        */
        template <
            typename Input,
            typename Output,
            typename Size, 
            typename Value, 
            typename Container,
            bool Continuous>
        bool dispatch_read (Input& in, Output& out, 
            variable_container <Size, Value, Container, Continuous>*,
            typename std::enable_if <
                std::is_same <Output, Container>::value,
                void
            >::type* = nullptr)
        {
            type_extractor <Size> size;
            if (!dispatch_read <Size> (in, size))
                return false;

            Container result;
            auto inserter = std::insert_iterator <Container> (result, result.begin());
            for (type_extractor <Size> i=0; i < size; ++i)
            {
                type_extractor <Value> x;
                if (!dispatch_read <Value> (in, x))
                    return false;

                inserter = std::move(x);
            }

            out = std::move(result);
            return true;
        }

        /**
        * \headerfile variable_container.hpp <framework/serializable/mutators/variable_container.hpp>
        * \brief Block write overload.
        * \param in input stream
        * \param out output container
        * \return true on success, false on failure
        */
        template <
            typename Input,
            typename Output,
            typename Size, 
            typename Value, 
            typename Container>
        bool dispatch_write (Input const& in, Output& out, 
            variable_container <Size, Value, Container, true>*,
            typename std::enable_if <
                std::is_same <Input, Container>::value &&
                std::is_arithmetic <Value>::value,
                void
            >::type* = nullptr)
        {
            type_extractor <Size> const& size = std::distance(in.begin(), in.end());
            if (!dispatch_write <Size> (size, out))
                return false;

            if (!out.write(reinterpret_cast <char const*> (&in[0]), sizeof(in[0])*in.size()))
                return false;

            return true;
        }

        /**
        * \headerfile variable_container.hpp <framework/serializable/mutators/variable_container.hpp>
        * \brief Block read overload.
        * \param in input stream
        * \param out output container
        * \return true on success, false on failure
        */
        template <
            typename Input,
            typename Output,
            typename Size, 
            typename Value, 
            typename Container,
            bool Default>
        bool dispatch_read (Input& in, Output& out, 
            variable_container <Size, Value, Container, true>*,
            typename std::enable_if <
                std::is_same <Output, Container>::value &&
                std::is_arithmetic <Value>::value,
                void
            >::type* = nullptr)
        {
            type_extractor <Size> size;
            if (!dispatch_read <Size> (in, size))
                return false;

            Container result(size);
            if (!in.read(reinterpret_cast <char*> (&result[0]), sizeof(result[0])*result.size()))
                return false;

            out = std::move(result);
            return true;
        }

        /**
        * \headerfile variable_container.hpp <framework/serializable/mutators/variable_container.hpp>
        * \brief Write overload.
        *
        * Writes the container to an output stream as a size delimited list.
        *
        * \param in input stream
        * \param out output container
        * \return true on success, false on failure
        */
        template <
            typename Input,
            typename Output,
            typename Size, 
            typename Value, 
            typename Container,
            bool Default>
        bool dispatch_write (Input const& in, Output& out, 
            variable_container <Size, Value, Container, Default>*,
            typename std::enable_if <
                std::is_same <Input, Container>::value,
                void
            >::type* = nullptr)
        {
            type_extractor <Size> const& size = std::distance(in.begin(), in.end());
            if (!dispatch_write <Size> (size, out))
                return false;

            for (auto const& x : in)
                if (!dispatch_write <Value> (static_cast <type_extractor <Value> const&> (x), out))
                    return false;

            return true;
        }
    }
}

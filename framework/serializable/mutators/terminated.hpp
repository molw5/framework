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
        * \headerfile terminated.hpp <framework/serializable/mutators/terminated.hpp>
        *
        * Serializes a fixed-size container using the value-type specification Value.
        *
        * \tparam Value value specification
        * \tparam Terminator terminating value
        * \tparam Container container type
        */
        template <
            typename Value, 
            typename type_extractor <Value>::type Terminator, 
            typename Container>
        struct terminated : public mutator_type <Container>
        {
        };

        /**
        * \headerfile terminated.hpp <framework/serializable/mutators/terminated.hpp>
        * \brief \c terminated serialization.
        */
        template <typename Value, typename type_extractor <Value>::type Terminator, typename Container>
        struct serializable_specification <terminated <Value, Terminator, Container>>
        {
            private:
                using value_type = typename type_extractor <Value>::type;

            public:
                /**
                * \brief Reads the container values.
                *
                * Reads container elements into \c out from the input stream using the specification 
                * given by \c Value until the value read matches \c Terminator.  The terminating
                * character is not added to \c out.
                *
                * \param in input stream
                * \param out output value
                * \return true on success, false on failure
                */
                template <typename Input>
                static bool read (Input& in, Container& out)
                {
                    value_type x;
                    if (!serializable_specification <Value>::read(in, x))
                        return false;

                    Container result;
                    auto inserter = std::insert_iterator <Container> (result, result.begin());
                    while (x != Terminator)
                    {
                        inserter = std::move(x);

                        if (!serializable_specification <Value>::read(in, x))
                            return false;
                    }

                    out = std::move(result);
                    return true;
                }
    
                /**
                * \brief Writes the container values.
                *
                * Writes container elements from \c in into the output stream using the specification
                * given by \c Value, followed by the terminating element \c Terminator.
                *
                * \pre \f$x \ne \textbf{Terminator}, \forall x \in \textbf{in}\f$
                * \param in input stream
                * \param out output value
                * \return true on success, false on failure
                */
                template <typename Output>
                static bool write (Container const& in, Output& out)
                {
                    for (auto const& x : in)
                    {
                        assert(x != Terminator);
                        if (!serializable_specification <Value>::write(x, out))
                            return false;
                    }

                    if (!serializable_specification <Value>::write(Terminator, out))
                        return false;
    
                    return true;
                }
        };
    }
}

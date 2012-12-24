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
        * \brief \c fixed_container serialization.
        */
        template <typename Value, typename Container>
        struct serializable_specification <fixed_container <Value, Container>>
        {
            private:
                using value_type = typename type_extractor <Value>::type;

            public:
                /**
                * Reads in the elements of \c out from the input stream using the specification
                * given by \c Value.
                *
                * \param in input stream
                * \param out output value
                * \return true on success, false on failure
                */
                template <typename Input>
                static bool read (Input& in, Container& out)
                {
                    Container result;
                    for (auto& x : result)
                        if (!serializable_specification <Value>::read(in, x))
                            return false;

                    out = std::move(result);
                    return true;
                }
    
                /**
                * Writes the elements of \c in to the output stream using the specification given 
                * by \c Value.
                *
                * \param in input value
                * \param out output stream
                * \return true on success, false on failure
                */
                template <typename Output>
                static bool write (Container const& in, Output& out)
                {
                    for (auto const& x : in)
                        if (!serializable_specification <Value>::write(x, out))
                            return false;

                    return true;
                }
        };
    }
}

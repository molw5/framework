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
        * \tparam Value container value specification
        * \tparam Terminator container element terminator
        * \tparam Container container value type
        */
        template <
            typename Size, 
            typename Value, 
            typename Container>
        struct variable_container : public mutator_type <Container>
        {
        };

        /**
        * \headerfile variable_container.hpp <framework/serializable/mutators/variable_container.hpp>
        * \brief \c variable_container serialization.
        */
        template <typename Size, typename Value, typename Container>
        struct serializable_specification <variable_container <Size, Value, Container>>
        {
            private:
                using size_type = typename type_extractor <Size>::type;
                using value_type = typename type_extractor <Value>::type;

            public:
                /**
                * Reads the container as a size followed by size elements.
                *
                * \param in input stream
                * \param out output value
                * \return true on success, false on failure
                */
                template <typename Input>
                static bool read (Input& in, Container& out)
                {
                    size_type size;
                    if (!serializable_specification <Size>::read(in, size))
                        return false;
    
                    Container result;
                    auto inserter = std::insert_iterator <Container> (result, result.begin());
                    for (size_type i=0; i < size; ++i)
                    {
                        value_type x;
                        if (!serializable_specification <Value>::read(in, x))
                            return false;

                        inserter = std::move(x);
                    }
                    
                    out = std::move(result);
                    return true;
                }
    
                /**
                * Writes the container as a size followed by size elements.
                *
                * \param in input object
                * \param out output stream
                * \return true on success, false on failure
                */
                template <typename Output>
                static bool write (Container const& in, Output& out)
                {
                    size_type const& size = std::distance(in.begin(), in.end());
                    if (!serializable_specification <Size>::write(size, out))
                        return false;
    
                    for (auto const& x : in)
                        if (!serializable_specification <Value>::write(x, out))
                            return false;
    
                    return true;
                }
        };
    }
}

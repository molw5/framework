// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Bit container.
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
            template <std::size_t N>
            struct bit_value_default
            {
                using type = 
                    typename std::conditional <
                        (N <= 8),
                        uint8_t,
                        typename std::conditional <
                            (N <= 16),
                            uint16_t,
                            typename std::conditional <
                                (N <= 32),
                                uint32_t,
                                typename std::enable_if <
                                    (N <= 64),
                                    uint64_t
                                >::type
                            >::type
                        >::type
                    >::type;
            };
        }

        /**
        * \headerfile bit_value.hpp <framework/serializable/mutators/bit_value.hpp>
        *
        * Serializes a value type as a sequence of bits.  This mutator requires a top-level type, that is
        *
        * \code
        * std::is_same <Value, type_extractor <Value>>::value == true
        * \endcode
        *
        * \tparam Value value type
        * \tparam Container container type
        */
        template <std::size_t N, typename Value = typename detail::bit_value_default <N>::type>
        struct bit_value : public mutator_type <Value>
        {
        };

        /**
        * \headerfile bit_value.hpp <framework/serializable/mutators/bit_value.hpp>
        * \brief Read overload.
        * \param in input stream
        * \param out output value
        * \return true on success, false on failure
        */
        template <
            std::size_t N,
            typename Value,
            typename Input,
            typename Output>
        bool read_dispatch (
            bit_value <N, Value>*,
            Input& in, Output& out)
        {
            static_assert(std::is_same <type_extractor <Value>, Value>::value,
                "bit_value does not support nested mutator types");
        
            Value result{0};
            if (!in.template read_bits <N> (result))
                return false;

            out = std::move(result);
            return true;
        }

        /**
        * \headerfile bit_value.hpp <framework/serializable/mutators/bit_value.hpp>
        * \brief Write overload.
        * \param in input value
        * \param out output stream
        * \return true on success, false on failure
        */
        template <
            std::size_t N,
            typename Value,
            typename Input,
            typename Output>
        bool write_dispatch (
            bit_value <N, Value>*,
            Input const& in, Output& out)
        {
            static_assert(std::is_same <type_extractor <Value>, Value>::value,
                "bit_value does not support nested mutator types");
            
            return out.template write_bits <N> (in);
        }
    }
}

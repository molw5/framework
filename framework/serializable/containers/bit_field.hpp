// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Bit field container.
*
* \copyright
* Copyright &copy; 2012 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

#include <framework/serializable/base_types.hpp>
#include <framework/serializable/streams/bit_stream.hpp>

namespace framework
{
    namespace serializable
    {
        /**
        * \headerfile bit_field.hpp <framework/serializable/containers/bit_field.hpp>
        * \brief Bit field container.
        */
        template <typename Block, typename... Specification>
        struct bit_field : public container_type <
            pack_container <Specification...>,
            pack_container <Specification...>,
            false>
        {
        };

        /**
        * \headerfile bit_field.hpp <framework/serializable/containers/bit_field.hpp>
        * \brief Read overload.
        * \param in input stream
        * \param out output object
        * \return true on success, false on failure
        */
        template <
            typename Input,
            typename Output,
            typename Block, 
            typename... Specification>
        bool read_dispatch (
            bit_field <Block, Specification...>*,
            Input& in, Output& out)
        {
            bit_stream <Input, Block> wrapper(in);
            return dispatch_read <alias <Specification...>> (wrapper, out);
        }

        /**
        * \headerfile bit_field.hpp <framework/serializable/containers/bit_field.hpp>
        * \brief Write overload.
        * \param in input object
        * \param out output stream
        * \return true on success, false on failure
        */
        template <
            typename Input,
            typename Output,
            typename Block, 
            typename... Specification>
        bool write_dispatch (
            bit_field <Block, Specification...>*,
            Input const& in, Output& out)
        {
            bit_stream <Output, Block> wrapper(out);
            if (!dispatch_write <alias <Specification...>> (in, wrapper))
                return false;

            if (!wrapper.flush())
                return false;

            return true;
        }
    }
}

// Copyright (C) 2013 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Base types.
*
* \copyright
* Copyright &copy; 2013 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

#include <framework/serializable/mutator_type.hpp>

namespace framework
{
    namespace protocol_buffers
    {
        /**
        * \brief Implementation wrapper.
        *
        * Provides appropriate definitions to protocol buffer fields (optional, required, repeated).
        */
        template <
            typename Name,
            typename Number,
            typename Specification,
            template <typename> class Implementation>
        struct protocol_buffer_implementation_wrapper
        {
            template <typename Derived>
            struct parameters
            {
                using value_name = Name;
                using value_number = Number;
                using value_type = serializable::type_extractor <Specification>;
                using value_specification = Specification;
                using derived = Derived;
                using implementation = Implementation <parameters <Derived>>;
            };

            template <typename Derived>
            using type = Implementation <parameters <Derived>>;
        };

        /**
        * \brief Common constants.
        */
        enum class e_constants
        {
            /**
            * \brief Fixed varint length.
            *
            * Defines the size of the fixed-length varint used in various implementations to avoid pre-computing
            * the size of a given field.
            *
            * \note
            * In the worst case (a very large repeated varint field is a reasonable approximation here) computing
            * the size of the field object may be nearly as expensive as writing the object to a byte string.  Here
            * we avoid this cost by fixing the size field to five bytes, limiting the maximum length delimited field
            * size to 2^(5*7)-1 =~ 32 GiB.
            */
            fixed_varint_length = 5
        };

        /**
        * \brief Field tag.
        *
        * Used to transmit the instantiating field number to the various nested types (group, for example, requires
        * it's parent's field number).  This tag never automatically decays.
        */
        template <typename Number, typename T>
        struct field_tag : serializable::mutator_type <T> 
        {
        };

        /**
        * \brief Remove reference/cv.
        */
        template <typename T>
        using remove_reference_cv = typename std::remove_cv <typename std::remove_reference <T>::type>::type;
    }
}

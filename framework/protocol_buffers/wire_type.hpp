// Copyright (C) 2013 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Wire type definitions.
*
* \copyright
* Copyright &copy; 2013 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

#include <framework/serializable/mutators/endian.hpp>
#include <framework/serializable/mutators/stl_string.hpp>
#include <framework/serializable/mutators/stl_vector.hpp>

#include <framework/protocol_buffers/varint.hpp>
#include <framework/protocol_buffers/frame.hpp>

namespace framework
{
    namespace protocol_buffers
    {
        namespace detail
        {
            template <typename T, typename Enabler = void>
            struct is_wire_type_impl;

            template <typename T, typename Enabler = void>
            struct wire_type_impl;

            template <typename T>
            struct is_integer_type_impl;

            template <typename T>
            struct make_wire_type_impl;
        }

        /**
        * \brief Wire type constants.
        */
        enum class e_wire_type : uint8_t
        {
            varint = 0,
            fixed_64 = 1,
            length_delimited = 2,
            start_group = 3,
            end_group = 4,
            fixed_32 = 5
        };

        /**
        * \brief Basic wire type definitions.
        */
        template <e_wire_type Type>
        struct wire_type_base
        {
// GCC bug workaround - www.open-std.org/jtc1/sc22/wg21/docs/cwg_defects.html#580
#if     MAX_GNUC_VERSION(4, 8, 0)
            public:
#else
            template <typename T, typename Enabler>
            friend struct detail::is_wire_type_impl;

            template <typename T, typename Enabler>
            friend struct detail::wire_type_impl;

            private:
#endif
                using protocol_buffers_wire_enabler = void;
                using protocol_buffers_wire_type = std::integral_constant <e_wire_type, Type>;
        };

        /**
        * \brief Wire type wrapper.
        */
        template <e_wire_type Type, typename Specification>
        struct wire_type_wrapper :
            wire_type_base <Type>,
            Specification
        {
        };

        /**
        * \brief Length delimited type.
        *
        * Defines a generic length delimited wire type, used to wrap external types.  For example,
        * stl_vector <int, int> is implicitly serialized as part of a protocol buffer object as
        * length_delimited <stl_vector <int, int>>.  
        *
        * \note
        * This definition was explicitly separated from wire_type_tag to avoid accidentally 
        * serializing internal types like string_, bytes, and message using this wrapper.
        */
        template <typename Specification>
        struct length_delimited : 
            serializable::mutator_type <Specification>,
            wire_type_base <e_wire_type::length_delimited>
        {
        };

        /**
        * \brief Is wire type.
        */
        template <typename T>
        using is_wire_type = typename detail::is_wire_type_impl <T>::type;

        /**
        * \brief Is integer type.
        * \note
        * Is equivalent to std::true_type for internal integer types only (int32, int64, ...) - used to conditionally
        * enable packed representations of repeated fields.
        */
        template <typename T>
        using is_integer_type = typename detail::is_integer_type_impl <T>::type;

        /**
        * \brief Wire type.
        */
        template <typename T>
        using wire_type = typename detail::wire_type_impl <T>::type;

        /**
        * \brief Make wire type.
        */
        template <typename T>
        using make_wire_type = typename detail::make_wire_type_impl <T>::type;

        /**
        * \brief float primitive.
        */
        using float_ = wire_type_wrapper <e_wire_type::fixed_32, serializable::little_endian <float>>;
        
        /**
        * \brief double primitive.
        */
        using double_ = wire_type_wrapper <e_wire_type::fixed_64, serializable::little_endian <double>>;
        
        /**
        * \brief bool primitive.
        */
        using bool_ = wire_type_wrapper <e_wire_type::varint, varint <bool>>;

        /**
        * \brief string primitive.
        */
        using string_ = wire_type_wrapper <e_wire_type::length_delimited, serializable::stl_string <varint <std::size_t>>>;

        /**
        * \brief bytes primitive.
        */
        using bytes = string_;

        /**
        * \brief int32 primitive.
        */
        using int32 = wire_type_wrapper <e_wire_type::varint, varint <int32_t>>;
        
        /**
        * \brief int64 primitive.
        */
        using int64 = wire_type_wrapper <e_wire_type::varint, varint <int64_t>>;
        
        /**
        * \brief uint32 primitive.
        */
        using uint32 = wire_type_wrapper <e_wire_type::varint, varint <uint32_t>>;
        
        /**
        * \brief uint64 primitive.
        */
        using uint64 = wire_type_wrapper <e_wire_type::varint, varint <uint64_t>>;
        
        /**
        * \brief sint32 primitive.
        */
        using sint32 = wire_type_wrapper <e_wire_type::varint, zig_zag <int32_t>>;
        
        /**
        * \brief sint64 primitive.
        */
        using sint64 = wire_type_wrapper <e_wire_type::varint, zig_zag <int64_t>>;
        
        /**
        * \brief fixed32 primitive.
        */
        using fixed32 = wire_type_wrapper <e_wire_type::fixed_32, serializable::little_endian <uint32_t>>;
        
        /**
        * \brief fixed64 primitive.
        */
        using fixed64 = wire_type_wrapper <e_wire_type::fixed_64, serializable::little_endian <uint64_t>>;
        
        /**
        * \brief sfixed32 primitive.
        */
        using sfixed32 = wire_type_wrapper <e_wire_type::fixed_32, serializable::little_endian <int32_t>>;
        
        /**
        * \brief sfixed64 primitive.
        */
        using sfixed64 = wire_type_wrapper <e_wire_type::fixed_64, serializable::little_endian <int64_t>>;
    }
}

#include <framework/protocol_buffers/wire_type.inl>

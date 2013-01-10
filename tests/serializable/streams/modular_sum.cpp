// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

#include <iostream>
#include <sstream>
#include <UnitTest++/UnitTest++.h>

#include <framework/serializable/base_types.hpp>
#include <framework/serializable/inline_object.hpp>
#include <framework/serializable/containers/value.hpp>
#include <framework/serializable/mutators/little_endian.hpp>
#include <framework/serializable/mutators/big_endian.hpp>
#include <framework/serializable/common_macros.hpp>
#include <framework/serializable/streams/modular_sum.hpp>

SUITE(framework_serializable_streams_modular_sum_hpp)
{
    TEST(BigEndian)
    {
        using namespace framework::serializable;

        inline_object <
            value <NAME("Field 1"), big_endian <uint32_t>>,
            value <NAME("Field 2"), uint8_t>,
            value <NAME("Field 3"), big_endian <uint16_t>>,
            value <NAME("Field 4"), uint8_t>
        > in {1, 2, 3, 4};

        modular_sum <2, ::framework::byte_order::big_endian> out;
        CHECK(write(in, out));

        uint32_t test = 
            (((in.get <NAME("Field 1")> () >> 24) & 0xFF) << 8) + 
            (((in.get <NAME("Field 1")> () >> 16) & 0xFF) << 0) + 
            (((in.get <NAME("Field 1")> () >>  8) & 0xFF) << 8) + 
            (((in.get <NAME("Field 1")> () >>  0) & 0xFF) << 0) + 
            (static_cast <uint32_t> (in.get <NAME("Field 2")> ()) << 8) +
            (((in.get <NAME("Field 3")> () >> 8) & 0xFF) << 0) +
            (((in.get <NAME("Field 3")> () >> 0) & 0xFF) << 8) +
            (static_cast <uint32_t> (in.get <NAME("Field 4")> ()) << 0);

        while (test >> 16)
            test = (test & 0xFFFF) + (test >> 16);

        test = (~test) & 0xFFFF;

        CHECK(test == out.checksum());
    }

    TEST(LittleEndian)
    {
        using namespace framework::serializable;

        inline_object <
            value <NAME("Field 1"), little_endian <uint32_t>>,
            value <NAME("Field 2"), uint8_t>,
            value <NAME("Field 3"), little_endian <uint16_t>>,
            value <NAME("Field 4"), uint8_t>
        > in {1, 2, 3, 4};

        modular_sum <2, ::framework::byte_order::little_endian> out;
        CHECK(write(in, out));

        uint32_t test = 
            (((in.get <NAME("Field 1")> () >>  0) & 0xFF) << 0) + 
            (((in.get <NAME("Field 1")> () >>  8) & 0xFF) << 8) + 
            (((in.get <NAME("Field 1")> () >> 16) & 0xFF) << 0) + 
            (((in.get <NAME("Field 1")> () >> 24) & 0xFF) << 8) + 
            (static_cast <uint32_t> (in.get <NAME("Field 2")> ()) << 0) +
            (((in.get <NAME("Field 3")> () >> 0) & 0xFF) << 8) +
            (((in.get <NAME("Field 3")> () >> 8) & 0xFF) << 0) +
            (static_cast <uint32_t> (in.get <NAME("Field 4")> ()) << 8);

        while (test >> 16)
            test = (test & 0xFFFF) + (test >> 16);

        test = (~test) & 0xFFFF;

        CHECK(test == out.checksum());
    }
}

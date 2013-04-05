// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

#include <iostream>
#include <sstream>
#include <UnitTest++/UnitTest++.h>

#include <framework/serializable/base_types.hpp>
#include <framework/serializable/inline_object.hpp>
#include <framework/serializable/containers/value.hpp>
#include <framework/serializable/mutators/endian.hpp>
#include <framework/serializable/common_macros.hpp>
#include <framework/serializable/streams/bit_stream.hpp>

SUITE(framework_serializable_streams_bit_stream_hpp)
{
    TEST(BasicRead)
    {
        using namespace framework::serializable;

        inline_object <
            value <NAME("Field 1"), big_endian <uint32_t>>,
            value <NAME("Field 2"), uint8_t>,
            value <NAME("Field 3"), big_endian <uint16_t>>,
            value <NAME("Field 4"), unsigned char>
        > in {1, 2, 3, 4}, out;

        std::stringstream ss;
        bit_stream <std::stringstream, uint8_t> bs(ss);

        CHECK(write(in, ss));
        CHECK(read(bs, out));
        CHECK(in == out);
    }

    TEST(BasicWrite)
    {
        using namespace framework::serializable;

        inline_object <
            value <NAME("Field 1"), big_endian <uint32_t>>,
            value <NAME("Field 2"), uint8_t>,
            value <NAME("Field 3"), big_endian <uint16_t>>,
            value <NAME("Field 4"), uint8_t>
        > in {1, 2, 3, 4}, out;

        std::stringstream ss;
        bit_stream <std::stringstream, uint8_t> bs(ss);

        CHECK(write(in, bs));
        CHECK(bs.flush());
        CHECK(read(ss, out));
        CHECK(in == out);
    }
}

// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <UnitTest++/UnitTest++.h>

#include <framework/serializable/inline_object.hpp>
#include <framework/serializable/containers/value.hpp>
#include <framework/serializable/mutators/endian.hpp>
#include <framework/serializable/common_macros.hpp>

using namespace framework::serializable;

template <typename T, typename Stream>
type_extractor <T> inline_reader (Stream&& stream)
{
    type_extractor <T> value;
    if (!dispatch_read <T> (stream, value))
        throw std::runtime_error("Inline reader failed");

    return value;
}

SUITE(framework_serializable_mutators_big_endian_hpp_1)
{
    TEST(BigEndian)
    {
        inline_object <
            value <NAME("x1"), big_endian <uint8_t>>,
            value <NAME("x2"), big_endian <uint16_t>>,
            value <NAME("x3"), big_endian <uint32_t>>,
            value <NAME("x4"), big_endian <uint64_t>>
        > out, in {0x01, 0x0102, 0x01020304, 0x0102030405060708};
        
        std::stringstream ss;
        CHECK(write(in, ss));
        CHECK(read(ss, out));
        CHECK(in == out);

        CHECK(dispatch_write <big_endian <uint64_t>> ((uint64_t)0x0102030405060708, ss));
        CHECK(inline_reader <uint8_t> (ss) == 0x01);
        CHECK(inline_reader <uint8_t> (ss) == 0x02);
        CHECK(inline_reader <uint8_t> (ss) == 0x03);
        CHECK(inline_reader <uint8_t> (ss) == 0x04);
        CHECK(inline_reader <uint8_t> (ss) == 0x05);
        CHECK(inline_reader <uint8_t> (ss) == 0x06);
        CHECK(inline_reader <uint8_t> (ss) == 0x07);
        CHECK(inline_reader <uint8_t> (ss) == 0x08);
    }

    TEST(LittleEndian)
    {
        inline_object <
            value <NAME("x1"), little_endian <uint8_t>>,
            value <NAME("x2"), little_endian <uint16_t>>,
            value <NAME("x3"), little_endian <uint32_t>>,
            value <NAME("x4"), little_endian <uint64_t>>
        > out, in {0x01, 0x0102, 0x01020304, 0x0102030405060708};
        
        std::stringstream ss;
        CHECK(write(in, ss));
        CHECK(read(ss, out));
        CHECK(in == out);

        CHECK(dispatch_write <little_endian <uint64_t>> ((uint64_t)0x0102030405060708, ss));
        CHECK(inline_reader <uint8_t> (ss) == 0x08);
        CHECK(inline_reader <uint8_t> (ss) == 0x07);
        CHECK(inline_reader <uint8_t> (ss) == 0x06);
        CHECK(inline_reader <uint8_t> (ss) == 0x05);
        CHECK(inline_reader <uint8_t> (ss) == 0x04);
        CHECK(inline_reader <uint8_t> (ss) == 0x03);
        CHECK(inline_reader <uint8_t> (ss) == 0x02);
        CHECK(inline_reader <uint8_t> (ss) == 0x01);
    }

    TEST(PdpEndian)
    {
        inline_object <
            value <NAME("x1"), pdp_endian <uint8_t>>,
            value <NAME("x2"), pdp_endian <uint16_t>>,
            value <NAME("x3"), pdp_endian <uint32_t>>,
            value <NAME("x4"), pdp_endian <uint64_t>>
        > out, in {0x01, 0x0102, 0x01020304, 0x0102030405060708};
        
        std::stringstream ss;
        CHECK(write(in, ss));
        CHECK(read(ss, out));
        CHECK(in == out);

        CHECK(dispatch_write <pdp_endian <uint64_t>> ((uint64_t)0x0102030405060708, ss));
        CHECK(inline_reader <uint8_t> (ss) == 0x02);
        CHECK(inline_reader <uint8_t> (ss) == 0x01);
        CHECK(inline_reader <uint8_t> (ss) == 0x04);
        CHECK(inline_reader <uint8_t> (ss) == 0x03);
        CHECK(inline_reader <uint8_t> (ss) == 0x06);
        CHECK(inline_reader <uint8_t> (ss) == 0x05);
        CHECK(inline_reader <uint8_t> (ss) == 0x08);
        CHECK(inline_reader <uint8_t> (ss) == 0x07);
    }
}

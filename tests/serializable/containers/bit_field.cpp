// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

#include <iostream>
#include <sstream>
#include <array>
#include <UnitTest++/UnitTest++.h>

#include <framework/serializable/base_types.hpp>
#include <framework/serializable/inline_object.hpp>
#include <framework/serializable/containers/value.hpp>
#include <framework/serializable/containers/bit_field.hpp>
#include <framework/serializable/mutators/bit_value.hpp>
#include <framework/serializable/common_macros.hpp>

SUITE(framework_serializable_containers_bit_field_hpp)
{
    TEST(Basic)
    {
        using namespace framework::serializable;

        inline_object <
            bit_field <uint8_t,
                value <NAME("Field 1"), bit_value <4, uint8_t>>,
                value <NAME("Field 2"), bit_value <4, uint8_t>>>
        > in {1, 2}, out;

        std::stringstream ss;
        CHECK(write(in, ss));
        CHECK(read(ss, out));
        CHECK(in == out);
    }

    TEST(Order)
    {
        using namespace framework::serializable;

        inline_object <
            bit_field <uint8_t,
                value <NAME("Field 1"), bit_value <4, uint8_t>>,
                value <NAME("Field 2"), bit_value <4, uint8_t>>>
        > in {1, 2};

        inline_object <
            value <NAME("Fields"), uint8_t>
        > out;

        std::stringstream ss;
        CHECK(write(in, ss));
        CHECK(read(ss, out));
        CHECK(in.get <NAME("Field 1")> () == (out.get <NAME("Fields")> () >> 4));
        CHECK(in.get <NAME("Field 2")> () == (out.get <NAME("Fields")> () & 0xF));
    }
}


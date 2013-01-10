// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

#include <iostream>
#include <sstream>
#include <tuple>
#include <utility>
#include <UnitTest++/UnitTest++.h>

#include <framework/serializable/inline_object.hpp>
#include <framework/serializable/containers/value.hpp>
#include <framework/serializable/mutators/tuple_container.hpp>
#include <framework/serializable/mutators/big_endian.hpp>
#include <framework/serializable/mutators/little_endian.hpp>
#include <framework/serializable/common_macros.hpp>

SUITE(framework_serializable_mutators_tuple_container_hpp)
{
    TEST(Basic)
    {
        using namespace framework;
        using namespace framework::serializable;

        inline_object <
            value <NAME("Field 1"), tuple_container <
                pack_container <
                    little_endian <uint16_t>,
                    big_endian <uint16_t>>,
                std::tuple <uint16_t, uint16_t>>>,
            value <NAME("Field 2"), tuple_container <
                pack_container <
                    little_endian <uint16_t>,
                    big_endian <uint16_t>>,
                std::pair <uint16_t, uint16_t>>>
        > out, in { std::make_tuple(uint16_t(0x0102), uint16_t(0x0304)),
                    std::make_pair(uint16_t(0x0506), uint16_t(0x0708)) };

        std::stringstream ss;
        CHECK(write(in, ss));
        CHECK(read(ss, out));
        CHECK(in == out);
    }
}

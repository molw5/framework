// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

#include <framework/serializable.hpp>
#include <framework/serializable/mutators/fixed_container.hpp>
#include <type_traits>
#include <algorithm>
#include <array>
#include <sstream>

#include <UnitTest++/UnitTest++.h>
/*
#include <iostream>
#include <sstream>
#include <array>

#include <framework/serializable/inline_object.hpp>
#include <framework/serializable/containers/value.hpp>
#include <framework/serializable/mutators/little_endian.hpp>
#include <framework/serializable/mutators/big_endian.hpp>
#include <framework/serializable/mutators/fixed_container.hpp>
#include <framework/serializable/common_macros.hpp>
#include <framework/serializable/utility/common.hpp>
*/

SUITE(framework_serializable_mutators_array_hpp)
{
    TEST(Basic)
    {
        using namespace framework::serializable;

        inline_object <
            value <NAME("Field 1"), fixed_container <uint16_t, std::array <uint16_t, 3>>>,
            value <NAME("Field 2"), fixed_container <little_endian <uint16_t>, std::array <uint16_t, 3>>>,
            value <NAME("Field 3"), fixed_container <big_endian <uint16_t>, std::array <uint16_t, 3>>>
        > out, in {std::array <uint16_t, 3> {{0x0102, 0x0304, 0x0506}},
              std::array <uint16_t, 3> {{0x0102, 0x0304, 0x0506}},
              std::array <uint16_t, 3> {{0x0102, 0x0304, 0x0506}}};

        std::stringstream ss;
        CHECK(write(in, ss));
        CHECK(read(ss, out));
        CHECK(in.get <NAME("Field 1")> () == out.get <NAME("Field 1")> ());
        //CHECK(in.get <NAME("Field 2")> () == out.get <NAME("Field 2")> () || in.get <NAME("Field 3")> () == out.get <NAME("Field 3")> ());
        //CHECK(in.get <NAME("Field 2")> () != out.get <NAME("Field 2")> () || in.get <NAME("Field 3")> () != out.get <NAME("Field 3")> ());
    }
}

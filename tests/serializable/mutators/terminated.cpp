// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <UnitTest++/UnitTest++.h>

#include <framework/serializable/inline_object.hpp>
#include <framework/serializable/containers/value.hpp>
#include <framework/serializable/mutators/big_endian.hpp>
#include <framework/serializable/mutators/terminated.hpp>
#include <framework/serializable/common_macros.hpp>

SUITE(framework_serializable_mutators_terminated_hpp)
{
    TEST(Basic)
    {
        using namespace framework::serializable;

        std::string buffer1;
        for (int i=0; i < 100; ++i)
            buffer1.push_back(std::rand() % 0xFF);

        std::vector <uint32_t> buffer2;
        for (int i=0; i < 100; ++i)
            buffer2.push_back(
                (static_cast <uint32_t> (std::rand() % 0x7FFF) << 30) |
                (static_cast <uint32_t> (std::rand() % 0x7FFF) << 15) |
                (static_cast <uint32_t> (std::rand() % 0x7FFF) <<  0));

        inline_object <
            value <NAME("Field 1"), terminated <char, '\0', std::string>>,
            value <NAME("Field 2"), terminated <big_endian <uint32_t>, 0, std::vector <uint32_t>>>
        > in {buffer1, buffer2}, out;

        std::stringstream ss;
        CHECK(write(in, ss));
        CHECK(read(ss, out));
        CHECK(in.get <NAME("Field 1")> () == buffer1);
        CHECK(in.get <NAME("Field 1")> () == out.get <NAME("Field 1")> ());
        CHECK(in.get <NAME("Field 2")> () == buffer2);
        CHECK(in.get <NAME("Field 2")> () == out.get <NAME("Field 2")> ());
        CHECK(in == out);
    }
}

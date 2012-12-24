// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

#include <iostream>
#include <sstream>
#include <string>
#include <UnitTest++/UnitTest++.h>

#include <framework/serializable/inline_object.hpp>
#include <framework/serializable/containers/value.hpp>
#include <framework/serializable/mutators/big_endian.hpp>
#include <framework/serializable/mutators/variable_container.hpp>
#include <framework/serializable/common_macros.hpp>

SUITE(framework_serializable_mutators_variable_container_hpp)
{
    TEST(Basic)
    {
        using namespace framework::serializable;

        std::string buffer;
        for (int i=0; i < 100; ++i)
            buffer.push_back(std::rand() % 0xFF);

        inline_object <
            value <NAME("Field 1"), variable_container <big_endian <uint32_t>, char, std::string>>
        > out, in {std::move(buffer)};

        std::stringstream ss;
        CHECK(write(in, ss));
        CHECK(read(ss, out));
        CHECK(in.get <NAME("Field 1")> () == out.get <NAME("Field 1")> ());
        CHECK(in == out);
    }
}

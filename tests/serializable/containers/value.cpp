// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

#include <iostream>
#include <sstream>
#include <array>
#include <tuple>
#include <UnitTest++/UnitTest++.h>

#include <framework/serializable/base_types.hpp>
#include <framework/serializable/inline_object.hpp>
#include <framework/serializable/containers/value.hpp>
#include <framework/serializable/common_macros.hpp>

SUITE(framework_serializable_containers_value_hpp)
{
    TEST(Basic)
    {
        using namespace framework::serializable;

        inline_object <
            value <NAME("Field 1"), uint64_t>,
            value <NAME("Field 2"), float>
        > in {std::forward_as_tuple(uint64_t(1)), 2.0f}, out;

        std::stringstream ss;
        
        CHECK(write(in, ss));
        CHECK(read(ss, out));
        CHECK(in.get <NAME("Field 1")> () == out.get <NAME("Field 1")> ());
        CHECK(in.get <NAME("Field 2")> () == out.get <NAME("Field 2")> ());
        CHECK(in == out);
        CHECK(!read(ss, out));
    }
}

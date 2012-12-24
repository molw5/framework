// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

#include <iostream>
#include <sstream>
#include <array>
#include <UnitTest++/UnitTest++.h>

#include <framework/serializable/base_types.hpp>
#include <framework/serializable/inline_object.hpp>
#include <framework/serializable/containers/value.hpp>
#include <framework/serializable/containers/optional_value.hpp>
#include <framework/serializable/containers/optional_field.hpp>
#include <framework/serializable/common_macros.hpp>

template <typename... Types> struct print_pack;

SUITE(framework_serializable_containers_optional_field_hpp)
{
    TEST(Basic)
    {
        using namespace framework::serializable;

        inline_object <
            optional_field <uint8_t,
                optional_value <0x01, NAME("Field 1"), uint8_t>,
                optional_value <0x02, NAME("Field 2"), uint32_t>>
        > in {1, 2}, out;

        CHECK(in.get <NAME("Field 1")> () == 1);
        CHECK(in.get <NAME("Field 2")> () == 2);

        std::stringstream ss;
        CHECK(write(in, ss));
        CHECK(read(ss, out));
        CHECK(in.get <NAME("Field 1")> () == out.get <NAME("Field 1")> ());
        CHECK(in.get <NAME("Field 2")> () == out.get <NAME("Field 2")> ());
        CHECK(in == out);
    }

    TEST(ConstructorFlags)
    {
        using namespace framework::serializable;

        inline_object <
            optional_field <uint8_t,
                optional_value <0x01, NAME("Field 1"), int>,
                optional_value <0x02, NAME("Field 2"), float>,
                optional_value <0x04, NAME("Field 3"), double>>
        > x {1, std::forward_as_tuple(), 3};

        CHECK(x.check <NAME("Field 1")> () == true);
        CHECK(x.check <NAME("Field 2")> () == false);
        CHECK(x.check <NAME("Field 3")> () == true);
    }

    TEST(UpdateFlags)
    {
        using namespace framework::serializable;

        inline_object <
            optional_field <uint8_t,
                optional_value <0x01, NAME("Field 1"), int>,
                optional_value <0x02, NAME("Field 2"), float>,
                optional_value <0x04, NAME("Field 3"), double>>
        > x {1, std::forward_as_tuple(), 3};
        
        CHECK(x.check <NAME("Field 1")> () == true);
        CHECK(x.check <NAME("Field 2")> () == false);
        CHECK(x.check <NAME("Field 3")> () == true);

        x.clear <NAME("Field 1")> ();
        x.set <NAME("Field 2")> (2);
        x.clear <NAME("Field 3")> ();
        
        CHECK(x.check <NAME("Field 1")> () == false);
        CHECK(x.check <NAME("Field 2")> () == true);
        CHECK(x.check <NAME("Field 3")> () == false);
    }

    TEST(Mixed)
    {
        using namespace framework::serializable;

        inline_object <
            optional_field <uint8_t,
                optional_value <0x01, NAME("Field 1"), int>,
                value <NAME("Field 2"), float>,
                optional_field <uint16_t,
                    optional_value <0x01, NAME("Field 3"), int>,
                    optional_value <0x02, NAME("Field 4"), int>>,
                value <NAME("Field 5"), double>,
                optional_value <0x02, NAME("Field 6"), char>>
        > in {1, 2, 3, 4, 5, 6}, out;

        std::stringstream ss;
        CHECK(write(in, ss));
        CHECK(read(ss, out));
        CHECK(in == out);
    }
}

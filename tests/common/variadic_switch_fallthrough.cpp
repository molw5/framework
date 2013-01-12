// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

#include <UnitTest++/UnitTest++.h>

#include <framework/common/variadic_switch_fallthrough.hpp>

void switch_check (std::size_t i, int& x)
{
    switch (i)
    {
        case 0: x += 1;
        case 1: x += 2;
        case 2: x += 3;
        case 3: x += 4;
        case 4: x += 5;
            break;

        default:
            x = 999;
    }
}

struct switch_value
{
    template <std::size_t N>
    void operator() (int& x)
    {
        x += N+1;
    }

    void operator() (int& x)
    {
        x = 999;
    }
};

SUITE(framework_variadic_switch_fallthrough_hpp)
{
    TEST(Basic)
    {
        using namespace framework;

        for (int i=0; i < 10; ++i)
        {
            int x1{0}, x2{0};

            switch_check(i, x1);
            variadic_switch_fallthrough <make_values <std::size_t, 5>> (i, switch_value(), x2);

            CHECK(x1 == x2);
        }
    }
}
